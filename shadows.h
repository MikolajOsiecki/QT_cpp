#ifndef SHADOWS_H
#define SHADOWS_H



#include <opencv2/opencv.hpp>
#include <vector>

std::vector<cv::Mat> createShares(const cv::Mat& secretImage, int n, int k);

cv::Mat decodeShares(const std::vector<cv::Mat>& shares, int k);

#endif // SHADOWS_H
