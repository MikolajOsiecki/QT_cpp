#include "shadows.h"
#include <random>


/**
 * Creates n shares from a secret image, where any k shares can be used to reconstruct the image.
 * @param secretImage The secret image in grayscale.
 * @param n The total number of shares to create.
 * @param k The minimum number of shares required to reconstruct the image.
 * @return A vector containing all n shares as Mat objects.
 */
std::vector<cv::Mat> createShares(const cv::Mat& secretImage, int n, int k) {
    std::vector<cv::Mat> shares;
    for(int i = 0; i < n; i++) {
        shares.push_back(cv::Mat::zeros(secretImage.size(), secretImage.type()));
    }

    std::random_device rd;
    std::mt19937 gen(rd());

    for(int y = 0; y < secretImage.rows; y++) {
        for(int x = 0; x < secretImage.cols; x++) {
            std::vector<int> coefficients;
            coefficients.push_back(secretImage.at<uchar>(y, x)); // Secret is the constant term

            for(int i = 1; i < k; i++) {
                coefficients.push_back(std::uniform_int_distribution<>(0, 255)(gen));
            }

            for(int i = 0; i < n; i++) {
                int shareValue = 0;
                for (int j = 0; j < k; j++) {
                    shareValue += coefficients[j] * std::pow(i+1, j); // Evaluate polynomial at different points
                }
                shares[i].at<uchar>(y, x) = shareValue % 256; // Keep within byte range
            }
        }
    }

    return shares;
}


double lagrangeInterpolate(const std::vector<int>& xs, const std::vector<double>& ys, int at_x, int mod) {
    int n = xs.size();
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        double term = ys[i];
        for (int j = 0; j < n; j++) {
            if (i != j) {
                term *= (at_x - xs[j]);
                term /= (xs[i] - xs[j]);
            }
        }
        result += term;
    }
    return fmod(result, mod);  // Ensure the result wraps around 'mod'
}


/**
 * Reconstructs the secret image from k shares.
 * @param shares A vector containing at least k shares as Mat objects.
 * @param k The minimum number of shares required for reconstruction.
 * @return The reconstructed secret image as a Mat object.
 */
cv::Mat decodeShares(const std::vector<cv::Mat>& shares, int k) {
    assert(!shares.empty() && shares.size() >= k);
    cv::Mat reconstructed = cv::Mat::zeros(shares[0].size(), shares[0].type());

    std::vector<int> xs(k);
    for (int i = 0; i < k; ++i) {
        xs[i] = i + 1;  // X values at which the polynomial is evaluated
    }

    for (int y = 0; y < reconstructed.rows; y++) {
        for (int x = 0; x < reconstructed.cols; x++) {
            std::vector<double> ys(k);
            for (int i = 0; i < k; i++) {
                ys[i] = static_cast<double>(shares[i].at<uchar>(y, x));
            }
            double interpolated_value = lagrangeInterpolate(xs, ys, 0, 256);  // Interpolating at x = 0, modulo 256
            reconstructed.at<uchar>(y, x) = static_cast<uchar>(interpolated_value);
        }
    }

    return reconstructed;
}
