#include "functions.h"
#include "shadows.h"

std::vector<std::string> generatePlaceholder(int amount, int selected_encoding) {
    std::vector<std::string> generated_shadows_list;

    for (int i = 1; i <= amount; ++i) {
        generated_shadows_list.push_back("Type: " + std::to_string(selected_encoding+1) + " Number: " + std::to_string(i));
    }

    return generated_shadows_list;
}


std::vector<std::string> generateShadows(const cv::Mat& secretImage, int amount, int threshold, int selected_encoding) {

    return generatePlaceholder(amount, selected_encoding);
}


QString matToBase64String(const cv::Mat &mat, const std::string &format) {
    QByteArray byteArray;
    QBuffer buffer(&byteArray);
    buffer.open(QIODevice::WriteOnly);
    // Convert cv::Mat to QImage
    QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
    // Write the QImage to the buffer in the specified format
    img.save(&buffer, format.c_str());
    // Convert to Base64 and return as QString
    return byteArray.toBase64();
}
