#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <filesystem>
#include "thien_lin_shadows.h"

std::vector<Shadow> generateShadows(const cv::Mat& inputImage, int K, int N);
std::vector<std::vector<std::vector<uint8_t>>> initializeB(const cv::Mat& A, int H, int W, int K);
std::vector<std::vector<std::vector<uint8_t>>> initializeC(const std::vector<std::vector<std::vector<uint8_t>>>& B, int H, int W, int K, int N);
void saveImages(const std::vector<std::vector<std::vector<uint8_t>>>& C, int H, int W, int N, std::vector<Shadow>& shadows);


std::vector<Shadow> generateShadowsTL(const cv::Mat& inputImage, int K, int N) {
    int H = inputImage.rows;
    int W = inputImage.cols / K;

    auto B = initializeB(inputImage, H, W, K);
    auto C = initializeC(B, H, W, K, N);

    std::vector<Shadow> shadows;
    saveImages(C, H, W, N, shadows);

    return shadows;
}


std::vector<std::vector<std::vector<uint8_t>>> initializeB(const cv::Mat& A, int H, int W, int K) {
    std::vector<std::vector<std::vector<uint8_t>>> B(H, std::vector<std::vector<uint8_t>>(W, std::vector<uint8_t>(K, 0)));
    for (int P = 0; P < H; ++P) {
        for (int Q = 0; Q < W; ++Q) {
            for (int R = 0; R < K; ++R) {
                B[P][Q][R] = A.at<uint8_t>(P, Q + (W * R));
            }
        }
    }
    return B;
}


std::vector<std::vector<std::vector<uint8_t>>> initializeC(const std::vector<std::vector<std::vector<uint8_t>>>& B, int H, int W, int K, int N) {
    std::vector<std::vector<std::vector<uint8_t>>> C(H, std::vector<std::vector<uint8_t>>(W, std::vector<uint8_t>(N, 0)));
    for (int P = 0; P < H; ++P) {
        for (int Q = 0; Q < W; ++Q) {
            std::vector<uint8_t> Y(K);
            for (int R = 0; R < K; ++R) {
                Y[R] = B[P][Q][R];
            }

            for (int R = 0; R < N; ++R) {
                int poly_val = 0;
                for (int i = 0; i < K; ++i) {
                    poly_val = poly_val * (R + 1) + Y[i];
                }
                C[P][Q][R] = poly_val % 251;
            }
        }
    }
    return C;
}

void saveImages(const std::vector<std::vector<std::vector<uint8_t>>>& C, int H, int W, int N, std::vector<Shadow>& shadows) {
    std::filesystem::create_directory("KEYS_CPP");

    for (int R = 0; R < N; ++R) {
        cv::Mat img(H, W, CV_8UC1);
        for (int P = 0; P < H; ++P) {
            for (int Q = 0; Q < W; ++Q) {
                img.at<uint8_t>(P, Q) = C[P][Q][R];
            }
        }

        std::string fname = "KEYS_CPP/K" + std::to_string(R + 1) + ".bmp";
        if (!cv::imwrite(fname, img)) {
            std::cerr << "Error saving image: " << fname << std::endl;
        }

        shadows.push_back({img, true, ""});
    }
}
