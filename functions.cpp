#include "functions.h"
#include <utility>
#include "shadows.h"


std::vector<std::string> generatePlaceholder(int amount, int selected_encoding) {
    std::vector<std::string> generated_shadows_list;

    for (int i = 1; i <= amount; ++i) {
        generated_shadows_list.push_back("Type: " + std::to_string(selected_encoding+1) + " Number: " + std::to_string(i));
    }

    return generated_shadows_list;
}


std::string pixelToBase36(unsigned char value) {
    const char* digits = "0123456789abcdefghijklmnopqrstuvwxyz";
    std::string result;
    do {
        result = digits[value % 36] + result;
        value /= 36;
    } while (value != 0);
    return result;
}


std::vector<std::string> sharesToStrings(const std::vector<cv::Mat>& shares) {
    std::vector<std::string> shareStrings;
    for (const auto& share : shares) {
        std::string shareData;
        for (int y = 0; y < share.rows; y++) {
            for (int x = 0; x < share.cols; x++) {
                uchar pixelValue = share.at<uchar>(y, x);
                shareData += pixelToBase36(pixelValue); // Convert each pixel value to base36 and append
                // Check if the current string length has reached the maximum allowed length
                if (shareData.length() >= 200) {
                    shareData.resize(200); // Resize to 200 characters to meet the requirement
                    break;
                }
            }
            if (shareData.length() >= 200) break;
        }
        shareStrings.push_back(shareData);
    }
    return shareStrings;
}


std::pair<std::vector<cv::Mat>, std::vector<std::string>> generateShadows(const cv::Mat& secretImage, int amount, int threshold, int selected_encoding) {
    auto shares = createShares(secretImage, amount, threshold); // Create shares from the image
    auto shareStrings = sharesToStrings(shares);                // Convert shares to strings
    return {shares, shareStrings};                              // Return both shares and their string representations
}


cv::Mat reconstructImage(const std::vector<cv::Mat> shadows, int threshold){
    return decodeShares(shadows, threshold);
}
