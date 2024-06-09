#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "mainwindow.h"
#include <QByteArray>
#include <QBuffer>
#include <QString>
#include <QImage>
#include <opencv2/opencv.hpp>
#include <vector>
#include <string>

// Function to process a vector of Shadow objects and update their text fields
void convertShadowsToStr(std::vector<Shadow>& shadows);


#endif // FUNCTIONS_H
