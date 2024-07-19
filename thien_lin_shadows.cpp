#include <iostream>
#include <filesystem>
#include "thien_lin_shadows.h"


std::vector<std::vector<std::vector<uint8_t>>> initializeB(const cv::Mat& A, int H, int W, int K);
std::vector<std::vector<std::vector<uint8_t>>> initializeC(const std::vector<std::vector<std::vector<uint8_t>>>& B, int H, int W, int K, int N);
void saveImages(const std::vector<std::vector<std::vector<uint8_t>>>& C, int H, int W, int N, std::vector<Shadow>& shadows);
void saveImages(const std::vector<std::vector<std::vector<uint8_t>>>& C, int H, int W, int N, std::vector<Shadow>& shadows, int sliceNumber);

std::vector<Shadow> generateShadowsTL(const cv::Mat& inputImage, int K, int N) {

    int H = inputImage.rows;
    int W = inputImage.cols / K;

    auto B = initializeB(inputImage, H, W, K);
    auto C = initializeC(B, H, W, K, N);

    std::vector<Shadow> shadows;
    saveImages(C, H, W, N, shadows);

    return shadows;
}


std::vector<Shadow> generateShadowsTL(const cv::Mat& inputImage, int K, int N, int sliceNumber) {
    // std::cout << "generateShadowsTL input Size: " << inputImage.size() << std::endl;

    int H = inputImage.rows;
    int W = inputImage.cols / K;

    auto B = initializeB(inputImage, H, W, K);
    auto C = initializeC(B, H, W, K, N);

    std::vector<Shadow> shadows;
    saveImages(C, H, W, N, shadows, sliceNumber);
    // for (const auto& shadow : shadows) {
        // std::cout << "generateShadowsTL shadow Size: " << shadow.image.size() << std::endl;
    // }
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

        shadows.push_back({img, false, "", R + 1});

        // Print the Shadow struct
        // std::cout << "Shadow " << R + 1 << " added: "
        //           << "isEssential=" << true
        //           << ", text=\"" << "" << "\""
        //           << ", number=" << R + 1
        //           << std::endl;
    }
}

void saveImages(const std::vector<std::vector<std::vector<uint8_t>>>& C, int H, int W, int N, std::vector<Shadow>& shadows, int sliceNumber) {
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

        shadows.push_back({img, false, "", R + 1, sliceNumber});

        // Print the Shadow struct
        // std::cout << "Shadow " << R + 1 << " added: "
        //           << "isEssential=" << true
        //           << ", text=\"" << "" << "\""
        //           << ", number=" << R + 1
        //           << ", sliceNumber=" << sliceNumber
        //           << std::endl;
    }
}

////////////////////////////////////////////////
////////////////////DECODE//////////////////////
////////////////////////////////////////////////

std::vector<double> lj_num(const std::vector<double>& x, int j, int m) {
    std::vector<double> f = {1.0};
    for (int p = 1; p <= m; ++p) {
        if (p == j) {
            continue;
        }
        std::vector<double> temp = {1.0, -x[p-1]};
        std::vector<double> result(f.size() + temp.size() - 1, 0.0);
        
        for (size_t i = 0; i < f.size(); ++i) {
            for (size_t k = 0; k < temp.size(); ++k) {
                result[i + k] += f[i] * temp[k];
            }
        }
        f = result;
    }
    return f;
}

double lj_den(const std::vector<double>& x, int j, int m) {
    double f = 1.0;
    for (int p = 1; p <= m; ++p) {
        if (p == j) {
            continue;
        }
        f *= (x[j-1] - x[p-1]);
    }
    return f;
}

int invMod(int x, int y, int m) {
    if (y == 0) {
        return 1;
    }
    
    int p = invMod(x, y / 2, m) % m;
    p = (p * p) % m;
    
    if (y % 2 == 0) {
        return p;
    } else {
        return (x * p) % m;
    }
}

int modFrac(int a, int b, int m) {
    if (b < 0) {
        b = -b;
        a = -a;
    }
    b = invMod(b, m - 2, m);
    int a_mod_m = (a % m + m) % m;
    int retval = (a_mod_m * b % m) % m;
    return retval;
}

std::vector<int> modLagPol(const std::vector<int>& y, const std::vector<double>& x, int v) {
    int m = y.size();
    std::vector<std::vector<double>> a(m);
    std::vector<double> b(m);
    
    for (int p = 1; p <= m; ++p) {
        a[p-1] = lj_num(x, p, m);
        b[p-1] = lj_den(x, p, m);
    }
    
    std::vector<int> num(m, 0);
    double den = *std::max_element(b.begin(), b.end());
    
    for (int p = 1; p <= m; ++p) {
        std::vector<double> t(a[p-1].size());
        std::transform(a[p-1].begin(), a[p-1].end(), t.begin(), [den, &b, p](double val) { return val * (den / b[p-1]); });
        if((den / b[p-1]) == 0 || den == 0 || b[p-1] == 0){
            std::cout << "Division by 0!!!" << std::endl;
        }
        for (size_t i = 0; i < t.size(); ++i) {
            num[i] += y[p-1] * t[i];
        }
    }
    
    std::vector<int> f(m);
    for (int p = 1; p <= m; ++p) {
        f[p-1] = modFrac(num[p-1], den, v);
    }
    
    return f;
}

cv::Mat decodeShadowsTL(const std::vector<Shadow>& selectedShadows, int K) {
    int R = selectedShadows.size();
    if (R < K) {
        std::cout << "-- Insufficient Number of Keys" << std::endl;
        // return cv::Mat();
    }

    std::vector<int> X(R);
    for (int i = 0; i < R; ++i) {
        X[i] = selectedShadows[i].number;  // Use the number field from the Shadow struct
    }

    // Load the first image to get the dimensions
    cv::Mat firstImage = selectedShadows[0].image;
    int H = firstImage.rows;
    int W = firstImage.cols;

    // Create a 3D matrix to store all images
    std::vector<cv::Mat> A(R, cv::Mat(H, W, CV_8UC1));

    // Load all images into the 3D matrix
    for (int P = 0; P < R; ++P) {
        A[P] = selectedShadows[P].image;
    }

    // Process the images
    cv::Mat I = cv::Mat::zeros(H, W * K, CV_64FC1);

    for (int M = 0; M < H; ++M) {
        for (int N = 0; N < W; ++N) {
            std::vector<int> Y(R);
            for (int O = 0; O < R; ++O) {
                Y[O] = A[O].at<uchar>(M, N);
            }

            std::vector<int> Z = modLagPol(Y, std::vector<double>(X.begin(), X.end()), 251);
            Z = std::vector<int>(Z.begin() + (R - K), Z.end());

            for (int O = 0; O < K; ++O) {
                I.at<double>(M, N + (W * O)) = Z[O];
            }
        }
    }

    I.convertTo(I, CV_8UC1);
    imwrite("Message_new.jpg", I);

    std::cout << "-- Message Successfully Recovered" << std::endl;

    return I;
}

// cv::Mat decodeShadowsTLdebug(const std::vector<Shadow>& selectedShadows, int K) {
//     int R = selectedShadows.size();
//     if (R < K) {
//         std::cout << "-- Insufficient Number of Keys" << std::endl;
//         // return cv::Mat();
//     }

//     std::vector<int> X(R);
//     for (int i = 0; i < R; ++i) {
//         X[i] = selectedShadows[i].number;  // Use the number field from the Shadow struct
//     }

//     // Load the first image to get the dimensions
//     cv::Mat firstImage = selectedShadows[0].image;
//     int H = firstImage.rows;
//     int W = firstImage.cols;

//     // Create a 3D matrix to store all images
//     std::vector<cv::Mat> A(R, cv::Mat(H, W, CV_8UC1));

//     // Load all images into the 3D matrix
//     for (int P = 0; P < R; ++P) {
//         A[P] = selectedShadows[P].image;
//     }

//     // Process the images
//     cv::Mat I = cv::Mat::zeros(H, W * K, CV_64FC1);

//     for (int M = 0; M < H; ++M) {
//         for (int N = 0; N < W; ++N) {
//             std::vector<int> Y(R);
//             for (int O = 0; O < R; ++O) {
//                 Y[O] = A[O].at<uchar>(M, N);
//             }

//             std::vector<int> Z = modLagPol(Y, std::vector<double>(X.begin(), X.end()), 251);
//             Z = std::vector<int>(Z.begin() + (R - K), Z.end());

//             for (int O = 0; O < K; ++O) {
//                 I.at<double>(M, N + (W * O)) = Z[O];
//             }
//         }
//     }

//     I.convertTo(I, CV_8UC1);
//     imwrite("Message_new.jpg", I);

//     std::cout << "-- Message Successfully Recovered" << std::endl;

//     return I;
// }
