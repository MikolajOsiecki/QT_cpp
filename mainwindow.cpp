#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "functions.h"
#include <opencv2/highgui/highgui.hpp>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnClear_clicked()
{
    // Get the total number of items in listSelectedSh
    int itemCount = ui->listSelectedSh->count();

    // Iterate over all items in listGeneratedSh
    for (int i = 0; i < itemCount; i++) {
        // Take the first item from listGeneratedSh
        QListWidgetItem* item = ui->listSelectedSh->takeItem(0);

        // Check if the item is not nullptr
        if (item) {
            // Add the item to listSelectedSh
            ui->listGeneratedSh->addItem(item);
        }
    }
}

void MainWindow::on_btnMoveBack_clicked()
{
    // Retrieve the list of selected items from listSelectedSh
    QList<QListWidgetItem*> selectedItems = ui->listSelectedSh->selectedItems();

    // Iterate over the list of selected items
    for (QListWidgetItem* item : selectedItems) {
        // For each selected item, create a new QListWidgetItem for listGeneratedSh
        QListWidgetItem* newItem = new QListWidgetItem(*item); // Copy the item
        ui->listGeneratedSh->addItem(newItem); // Add it to listGeneratedSh

        // Delete from original list
        delete ui->listSelectedSh->takeItem(ui->listSelectedSh->row(item));
    }
}


void MainWindow::on_btnSelectShadows_clicked()
{
    // Retrieve the list of selected items from listGeneratedSh
    QList<QListWidgetItem*> selectedItems = ui->listGeneratedSh->selectedItems();

    // Iterate over the list of selected items
    for (QListWidgetItem* item : selectedItems) {
        // For each selected item, create a new QListWidgetItem for listSelectedSh
        QListWidgetItem* newItem = new QListWidgetItem(*item); // Copy the item
        ui->listSelectedSh->addItem(newItem); // Add it to listSelectedSh

        // Delete from original list
        delete ui->listGeneratedSh->takeItem(ui->listGeneratedSh->row(item));
    }
}



void MainWindow::on_btnGenerateShadows_clicked()
{
    QString ShadowsNumber = ui->txtNumberOfShadows->text();
    bool ok = false;
    int number = ShadowsNumber.toInt(&ok);
    QString ShadowsThreshold = ui->txtShadowThreshold->text();
    bool ok2 = false;
    int threshold = ShadowsThreshold.toInt(&ok2);
    if (ok && ok2){
        ok = number > 0 ;
        ok2 = threshold >= 2;
        if (ok && ok2){

            ui->listGeneratedSh->clear(); // Clear old shadows

            int CurrentIndex = ui->comboEncodingType->currentIndex(); // Read encoding type from widget
            std::vector<std::string> GeneratedShadows(number);
            GeneratedShadows = generateShadows(number, CurrentIndex); // Generate shadows

            // Adding each shadow to the QListWidget
            for (const auto& shadow : GeneratedShadows) {
                QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(shadow), ui->listGeneratedSh);
                ui->listGeneratedSh->addItem(newItem); // Optional here
            }
        }
        else {
            ui->txtNumberOfShadows->setText("Must be >0");
            ui->txtShadowThreshold->setText("Must be >=2");
        }
    }
    else{
        ui->txtNumberOfShadows->setText("Wrong number");
        ui->txtShadowThreshold->setText("Wrong number");
    }
}


void MainWindow::on_btnSelectImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (QString::compare(filename, QString()) != 0 )
    {
        // Load the image using OpenCV
        loadedImage = cv::imread(filename.toStdString(), cv::IMREAD_COLOR);

        if (!loadedImage.empty())
        {
            // Display the image using OpenCV in a separate window
            cv::namedWindow("Selected Image", cv::WINDOW_AUTOSIZE); // Create a window for display.
            cv::imshow("Selected Image", loadedImage); // Show our image inside it.

            // Optionally, you can convert the cv::Mat to QImage and then display it in the Qt widget as before.
            // This step requires conversion from cv::Mat to QImage.
            QImage qimg(loadedImage.data, loadedImage.cols, loadedImage.rows, loadedImage.step, QImage::Format_RGB888);
            qimg = qimg.rgbSwapped(); // Convert BGR to RGB
            ui->picSelected->setPixmap(QPixmap::fromImage(qimg.scaled(ui->picSelected->width(), ui->picSelected->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        }
        else
        {
            // Handle the invalid image case
        }
    }
}
