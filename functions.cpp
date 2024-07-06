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


std::vector<cv::Mat> sliceImageVertically(const cv::Mat& image, int n, bool usePadding) {
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

std::vector<Shadow> composeShadows(const std::vector<Shadow>& allSubShadows, int shadowsAmount, int shadowsThreshold) {
    std::vector<Shadow> composedShadows(shadowsAmount);

    for (int i = 0; i < shadowsAmount; ++i) {
        std::cout << "Now processing i == " << i << std::endl;
        Shadow composedShadow;
        composedShadow.isEssential = true;
        composedShadow.number = i + 1;
        composedShadow.sliceNumber = -1; // This is a composed shadow, not from a specific slice

        std::vector<cv::Mat> subImages;
        for (int j = 0; j < shadowsAmount; ++j) {
            std::cout << "Now processing j == " << j << std::endl;
            int WangLinAmount = 2 * shadowsAmount - shadowsThreshold;
            for (int t = 0; t < WangLinAmount; ++t) {
                if (j == i) {
                    subImages.push_back(allSubShadows[j * WangLinAmount + t].image);
                    std::cout << "Shadow Number: " << allSubShadows[j * WangLinAmount + t].number << " , slice number: " << allSubShadows[j * WangLinAmount + t].sliceNumber << std::endl;
                } else if (j > i) {
                    subImages.push_back(allSubShadows[j * WangLinAmount + i].image);
                    std::cout << "Shadow Number: " << allSubShadows[j * WangLinAmount + i].number << " , slice number: " << allSubShadows[j * WangLinAmount + i].sliceNumber << std::endl;
                } else {
                    subImages.push_back(allSubShadows[j * WangLinAmount + (i + shadowsAmount - t)].image);
                    std::cout << "Shadow Number: " << allSubShadows[j * WangLinAmount + (i + shadowsAmount - t)].number << " , slice number: " << allSubShadows[j * WangLinAmount + (i + shadowsAmount - t)].sliceNumber << std::endl;
                }
            }
        }

        // Merge all selected sub-images into one composed shadow image
        composedShadow.image = mergeSubImages(subImages);
        composedShadow.text = "Composed Shadow " + std::to_string(i + 1);
        composedShadows[i] = composedShadow;
    }

    return composedShadows;
}
