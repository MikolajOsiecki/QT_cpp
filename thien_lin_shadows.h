#ifndef THIEN_LIN_SHADOWS_H
#define THIEN_LIN_SHADOWS_H

#include "mainwindow.h"
#include <opencv2/opencv.hpp>
#include <vector>

std::vector<Shadow> generateShadowsTL(const cv::Mat& inputImage, int K, int N);

void saveImages(const std::vector<std::vector<std::vector<uint8_t>>>& C, int H, int W, int N, std::vector<Shadow>& shadows);


#endif // THIEN_LIN_SHADOWS_H
