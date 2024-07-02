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
