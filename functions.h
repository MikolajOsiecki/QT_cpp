#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <QByteArray>
#include <QBuffer>
#include <QString>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

// Declaration of generateShadows
std::pair<std::vector<cv::Mat>, std::vector<std::string>> generateShadows(const cv::Mat& secretImage, int amount, int threshold, int selected_encoding) ;

cv::Mat reconstructImage(const std::vector<cv::Mat> shadows, int threshold);
#endif // FUNCTIONS_H
