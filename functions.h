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
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QImage>
#include <QPixmap>

// Function to process a vector of Shadow objects and update their text fields
void convertShadowsToStr(std::vector<Shadow>& shadows);

class ImageViewer : public QWidget
{
    Q_OBJECT

public:
    explicit ImageViewer(QWidget *parent = nullptr);
    void setImage(const cv::Mat &image);

private:
    QLabel *imageLabel;
};


#endif // FUNCTIONS_H
