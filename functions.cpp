#include "functions.h"
#include "mainwindow.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
// #include <iostream>


std::vector<std::string> generatePlaceholder(int amount, int selected_encoding) {
    std::vector<std::string> generated_shadows_list;

    for (int i = 1; i <= amount; ++i) {
        generated_shadows_list.push_back("Type: " + std::to_string(selected_encoding+1) + " Number: " + std::to_string(i));
    }

    return generated_shadows_list;
}


// Function to convert a shadow image to a string of size 50
std::string convertImageToString(const cv::Mat& image) {
    const std::string charset = "0123456789abcdefghijklmnopqrstuvwxyz";
    const int charsetSize = charset.size();
    std::string result;
    result.reserve(50);

    // We will take the first 50 pixels from the image in row-major order
    int count = 0;
    for (int i = 0; i < image.rows && count < 100; ++i) {
        for (int j = 0; j < image.cols && count < 100; ++j) {
            int pixelValue = image.at<uchar>(i, j);
            char charValue = charset[pixelValue * charsetSize / 256];
            result += charValue;
            ++count;
        }
    }

    // If the image has less than 50 pixels, pad the result with '0'
    while (result.size() < 50) {
        result += '0';
    }

    return result;
}


// Function to process a vector of Shadow objects and update their text fields
void convertShadowsToStr(std::vector<Shadow>& shadows) {
    for (auto& shadow : shadows) {
        shadow.text = convertImageToString(shadow.image);
    }
}


ImageViewer::ImageViewer(QWidget *parent) : QWidget(parent)
{
    imageLabel = new QLabel(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(imageLabel);
    setLayout(layout);
}


void ImageViewer::setImage(const cv::Mat &image)
{
    // Check if the image is grayscale
    if (image.channels() == 1) {
        // Convert cv::Mat to QImage (grayscale)
        QImage qImage(image.data, image.cols, image.rows, image.step, QImage::Format_Grayscale8);
        QPixmap pixmap = QPixmap::fromImage(qImage);
        imageLabel->setPixmap(pixmap);
        imageLabel->adjustSize();
    } else {
        // Handle the case where the image is not grayscale (optional)

    }
}


cv::Mat padImage(const cv::Mat& image, int n) {
    int newWidth = ((image.cols + n - 1) / n) * n;  // Find the next multiple of n
    cv::Mat paddedImage;
    int rightPadding = newWidth - image.cols;
    cv::copyMakeBorder(image, paddedImage, 0, 0, 0, rightPadding, cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));
    return paddedImage;
}


cv::Mat cropImage(const cv::Mat& image, int n) {
    int newWidth = (image.cols / n) * n;  // Largest multiple of n less than image.cols
    cv::Rect cropRegion(0, 0, newWidth, image.rows);
    return image(cropRegion);
}


std::vector<cv::Mat> sliceImageVertically(const cv::Mat& image, int n, bool usePadding = false) {
    cv::Mat processedImage;
    if (usePadding) {
        processedImage = padImage(image, n);
    } else {
        processedImage = cropImage(image, n);
    }

    std::vector<cv::Mat> slices;
    int sliceWidth = processedImage.cols / n;

    for (int i = 0; i < n; ++i) {
        int startCol = i * sliceWidth;
        int endCol = startCol + sliceWidth;

        cv::Rect sliceRegion(startCol, 0, endCol - startCol, processedImage.rows);
        cv::Mat slice = processedImage(sliceRegion);
        slices.push_back(slice);

        std::string fname = "KEYS_CPP/I" + std::to_string(i + 1) + ".bmp";
        cv::imwrite(fname, slice);
    }

    return slices;
}


cv::Mat mergeSubImages(const std::vector<cv::Mat>& subImages) {
    // Assuming all sub-images are of the same size and should be concatenated vertically
    cv::Mat composedImage;
    cv::hconcat(subImages, composedImage);
    return composedImage;
}


/**
 * Function to compose shadows from all sub-shadows
 * @param allSubShadows: A vector containing all sub-shadows
 * @param shadowsAmount: The total number of shadows
 * @param shadowsThreshold: The threshold for decoding shadows
 * @return A vector containing the composed shadows (shadowsAmount in total)
 * @note Function assumes that all sub-shadows are of the same size, and that the shadows are concatenated vertically
 *
 * The function will compose shadows based on Wang-Lin's method (2013).
*/
std::vector<Shadow> composeShadows(const std::vector<Shadow>& allSubShadows, int shadowsAmount, int shadowsThreshold) {
    std::vector<Shadow> composedShadows(shadowsAmount);

    for (int i = 0; i < shadowsAmount; ++i) {
        // std::cout << "Now processing i == " << i << std::endl;
        Shadow composedShadow;
        composedShadow.isEssential = false;
        composedShadow.number = i + 1;
        composedShadow.sliceNumber = -1; // This is a composed shadow, not from a specific slice
        int WangLinAmount = 2 * shadowsAmount - shadowsThreshold;
        std::vector<cv::Mat> subImages;
        for (int j = 0; j < shadowsAmount; ++j) {
            // std::cout << "Now processing j == " << j << std::endl;

                if (j == i) {
                    for (int k = j; k < j + shadowsAmount - shadowsThreshold + 1; ++k) {
                        // std::cout << "Adding j==i, k == " << k << std::endl;
                        subImages.push_back(allSubShadows[j*WangLinAmount + k].image);
                        // std::cout << "Shadow Number: " << allSubShadows[j*WangLinAmount + k].number << " , slice number: " << allSubShadows[j*WangLinAmount + k].sliceNumber << std::endl;
                    }
                } else if (j > i) {
                    // std::cout << "Adding j>i, i == " << i << std::endl;
                    subImages.push_back(allSubShadows[j * WangLinAmount + i].image);
                    // std::cout << "Shadow Number: " << allSubShadows[j * WangLinAmount + i].number << " , slice number: " << allSubShadows[j * WangLinAmount + i].sliceNumber << std::endl;
                } else {
                    // std::cout << "Adding j<i, i+n-t == " << i+shadowsAmount-shadowsThreshold << std::endl;
                    subImages.push_back(allSubShadows[j * WangLinAmount + (i + shadowsAmount - shadowsThreshold)].image);
                    // std::cout << "Shadow Number: " << allSubShadows[j * WangLinAmount + (i + shadowsAmount - shadowsThreshold)].number << " , slice number: " << allSubShadows[j * WangLinAmount + (i + shadowsAmount - shadowsThreshold)].sliceNumber << std::endl;
                }
        }
        // std::cout << "=================" << std::endl;
        // Merge all selected sub-images into one composed shadow image
        composedShadow.image = mergeSubImages(subImages);
        composedShadow.text = "Composed Shadow " + std::to_string(i + 1);
        composedShadows[i] = composedShadow;

        std::string fname = "KEYS_CPP/K" + std::to_string(i + 1) + ".bmp";
        if (!cv::imwrite(fname, composedShadow.image)) {
            std::cout << "Error saving image: " << fname << std::endl;
        }
    }

    return composedShadows;
}


std::vector<Shadow> decomposeShadows(const std::vector<Shadow>& composedShadows, int shadowsAmount, int shadowsThreshold) {
    std::vector<Shadow> allSubShadows;
    int WangLinAmount = 2 * shadowsAmount - shadowsThreshold;

    for (const auto& composedShadow : composedShadows) {
        std::vector<cv::Mat> slices = sliceImageVertically(composedShadow.image, WangLinAmount, false);
        // std::cout << "composedShadow.number: " << composedShadow.number << std::endl;

        int i = composedShadow.number - 1;
        for (int j = 0; j < shadowsAmount; ++j) {
            if (j == i) {
                for (int k = j; k < j +  shadowsAmount - shadowsThreshold + 1; ++k) {
                    Shadow subShadow;
                    subShadow.isEssential = false;
                    subShadow.sliceNumber = j + 1;
                    subShadow.number = k + 1;
                    subShadow.image = slices[k];
                    // std::cout << "slice.number j==i: " << k << std::endl;
                    subShadow.text = "Sub Shadow " + std::to_string(j + 1) + "-" + std::to_string(k + 1);
                    allSubShadows.push_back(subShadow);
                }
            // i have no idea why these 2 cases have to have selectors (shadow.image) like that
            // j==i has same as in composeShadows minus the offset and these two dont. it just works
            } else if (j > i) {
                Shadow subShadow;
                subShadow.isEssential = false;
                subShadow.sliceNumber = j + 1;
                subShadow.number = i + 1;
                subShadow.image = slices[j+ shadowsAmount - shadowsThreshold];
                // std::cout << "slice.number j > i: " << j+ shadowsAmount - shadowsThreshold << std::endl;
                subShadow.text = "Sub Shadow " + std::to_string(j + 1) + "-" + std::to_string(i + 1);
                allSubShadows.push_back(subShadow);
            } else {
                Shadow subShadow;
                subShadow.isEssential = false;
                subShadow.sliceNumber = j + 1;
                subShadow.number = i + 1 + shadowsAmount - shadowsThreshold;
                subShadow.image = slices[j];
                // std::cout << "slice.number j < i: " << j << std::endl;
                subShadow.text = "Sub Shadow " + std::to_string(j + 1) + "-" + std::to_string(i + 1);
                allSubShadows.push_back(subShadow);
            }
        }
    }

    return allSubShadows;
}


std::vector<Shadow> copyShadowsWithSliceNumber(const std::vector<Shadow>& shadows, int specifiedNumber) {
    std::vector<Shadow> result;

    for (const auto& shadow : shadows) {
        if (shadow.sliceNumber == specifiedNumber) {
            // std::cout << "Copied Shadow Number: " << shadow.number << ", Slice Number: " << shadow.sliceNumber << std::endl;
            result.push_back(shadow);
        }
    }

    return result;
}


std::vector<Shadow> copyShadowsWithNumber(const std::vector<Shadow>& shadows, int specifiedNumber) {
    std::vector<Shadow> result;

    for (const auto& shadow : shadows) {
        if (shadow.number == specifiedNumber) {
            // std::cout << "Copied Shadow Number: " << shadow.number << ", Slice Number: " << shadow.sliceNumber << std::endl;
            result.push_back(shadow);
        }
    }

    return result;
}

/**
 * @param shadows: A vector containing shadows
 * @param returnEssential: If true return only essential, otherwise only non-essential
 * @return A vector containing the selected shadows (shadowsAmount in total)
*/
std::vector<Shadow> copyEssentialShadows (const std::vector<Shadow>& shadows, bool returnEssential) {
    std::vector<Shadow> result;

    for (const auto& shadow : shadows) {
        if (shadow.isEssential == returnEssential) {
            result.push_back(shadow);
        }
    }

    return result;
}


void changeShadowEssentialValue(std::vector<Shadow>& shadows, bool isEssential) {
    for (auto& shadow : shadows) {
        shadow.isEssential = isEssential;
    }
    return;
}

std::vector<Shadow> getSubTempShadows (const std::vector<Shadow>& shadows, int essentialThreshold, int essentialNumber, int shadowsThreshold) {
    int sktAmount = essentialNumber + shadowsThreshold - essentialThreshold;
    int sktWangLingAmount = 2*sktAmount - shadowsThreshold;
    std::vector<Shadow> result;
    std::cout << "Partitioning "  << std::endl;
    for(const auto& shadow : shadows){
        if(shadow.isEssential == true){
            std::cout << "Slicing "  << std::endl;
            std::vector<cv::Mat> slices = sliceImageVertically(shadow.image,sktWangLingAmount);
            int k = 1;
            std::cout << "slcies size= "<< slices.size()  << std::endl;
            for(const auto& image : slices){
                std::cout << "k=  "  << k << std::endl;
                std::string windowName = cv::format("slice %d", k);
                std::cout << "windowname=  "  << windowName << std::endl;
                cv::imshow(windowName, image);
                k++;
            }
        }
    }
    return result;
}
