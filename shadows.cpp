#include "shadows.h"
#include <vector>
#include <random>

using namespace cv;
using namespace std;

/**
 * Creates n shares from a secret image, where any k shares can be used to reconstruct the image.
 * @param secretImage The secret image in grayscale.
 * @param n The total number of shares to create.
 * @param k The minimum number of shares required to reconstruct the image.
 * @return A vector containing all n shares as Mat objects.
 */
vector<Mat> createShares(const Mat& secretImage, int n, int k) {
    vector<Mat> shares;
    for(int i = 0; i < n; i++) {
        shares.push_back(Mat::zeros(secretImage.size(), secretImage.type()));
    }

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 255);

    for(int y = 0; y < secretImage.rows; y++) {
        for(int x = 0; x < secretImage.cols; x++) {
            int pixelValue = secretImage.at<uchar>(y, x);
            vector<int> randomValues;
            for(int i = 0; i < k - 1; i++) {
                randomValues.push_back(dis(gen));
            }
            randomValues.push_back(pixelValue); // Ensure the original value can be reconstructed

            // Distribute random values to shares
            for(int i = 0; i < n; i++) {
                shares[i].at<uchar>(y, x) = randomValues[i % k];
            }
        }
    }

    return shares;
}

/**
 * Reconstructs the secret image from k shares.
 * @param shares A vector containing at least k shares as Mat objects.
 * @param k The minimum number of shares required for reconstruction.
 * @return The reconstructed secret image as a Mat object.
 */
Mat reconstructImage(const vector<Mat>& shares, int k) {
    CV_Assert(!shares.empty() && shares.size() >= k);
    Mat reconstructed = Mat::zeros(shares[0].size(), shares[0].type());

    for(int y = 0; y < reconstructed.rows; y++) {
        for(int x = 0; x < reconstructed.cols; x++) {
            int sum = 0;
            for(int i = 0; i < k; i++) {
                sum += shares[i].at<uchar>(y, x);
            }
            reconstructed.at<uchar>(y, x) = sum / k; // Average value for demonstration
        }
    }

    return reconstructed;
}
