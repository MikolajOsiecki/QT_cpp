#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "functions.h"
#include <opencv2/highgui/highgui.hpp>
#include <QMessageBox>  // Ensure to include QMessageBox
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Define the stylesheet
    QString styleSheet = "QMainWindow { background-color: lightgray; color: black; }"
                         "QWidget { background-color: lightblue; color: black; }"
                         "QPushButton { background-color: lightblue; color: black; }"
                         "QLineEdit { background-color: lightblue; color: black; }"
                         "QLabel { background-color: lightblue; color: black; }"
                         "QListWidget { background-color: lightblue; color: black; }"
                         "QComboBox { background-color: lightblue; color: black; }"
                         "QTextEdit { background-color: lightblue; color: black; }";
                         // "QMenuBar { background-color: lightgray; color: black; }";

    // Set the stylesheet to the main window
    this->setStyleSheet(styleSheet);
}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_btnClear_clicked()
{
    // Get the total number of items in listSelectedSh
    int itemCount = ui->listSelectedSh->count();

    // Iterate over all items in listSelectedSh
    for (int i = 0; i < itemCount; i++) {
        // Take the first item from listSelectedSh
        QListWidgetItem* item = ui->listSelectedSh->takeItem(0);

        // Check if the item is not nullptr
        if (item) {
            // Add the item to listGeneratedSh
            ui->listGeneratedSh->addItem(item);

            // Also add the corresponding cv::Mat and X value from selectedShadows to generatedShadows and generatedXValues
            generatedShadows.push_back(selectedShadows.front());

            // Remove the corresponding cv::Mat and X value from selectedShadows and selectedXValues
            selectedShadows.erase(selectedShadows.begin());
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

        // Get the index of the current item
        int index = ui->listSelectedSh->row(item);

        // Add the corresponding cv::Mat and X value from selectedShadows to generatedShadows and generatedXValues
        generatedShadows.push_back(selectedShadows[index]);

        // Remove the item from the original list
        delete ui->listSelectedSh->takeItem(index);

        // Also remove the corresponding cv::Mat and X value from selectedShadows and selectedXValues
        selectedShadows.erase(selectedShadows.begin() + index);
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

        // Get the index of the current item
        int index = ui->listGeneratedSh->row(item);

        // Add the corresponding cv::Mat and X value from generatedShadows to selectedShadows and selectedXValues
        selectedShadows.push_back(generatedShadows[index]);

        // Remove the item from the original list
        delete ui->listGeneratedSh->takeItem(index);

        // Also remove the corresponding cv::Mat and X value from generatedShadows and generatedXValues
        generatedShadows.erase(generatedShadows.begin() + index);
    }
}



void MainWindow::on_btnGenerateShadows_clicked()
{
    QString ShadowsNumber = ui->txtNumberOfShadows->text();
    bool ok = false;
    shadowsAmount = ShadowsNumber.toInt(&ok);
    QString ShadowsThreshold = ui->txtShadowThreshold->text();
    bool ok2 = false;
    shadowsThreshold = ShadowsThreshold.toInt(&ok2);
    std::cout << "shadowsThreshold: " << shadowsThreshold << std::endl;
    // Check if loadedImage is empty
    if (loadedImage.empty()) {
        QMessageBox::warning(this, "Error", "No image loaded or image is empty.");
        return;  // Stop execution of the function if image is not loaded
    }

    if (ok && ok2 && shadowsAmount > 0 && shadowsThreshold >= 2) {
        ui->listGeneratedSh->clear();  // Clear old shadows

        // int CurrentIndex = ui->comboEncodingType->currentIndex();  // Read encoding type from widget

        auto [GeneratedShadows, GeneratedShadowStrings] = generateShadows(loadedImage, shadowsAmount, shadowsThreshold);  // Generate shadows and convert to strings
        generatedShadows = GeneratedShadows;

        // Adding each shadow string to the QListWidget
        for (const auto& shadow : GeneratedShadowStrings) {
            QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(shadow), ui->listGeneratedSh);
            ui->listGeneratedSh->addItem(newItem);
        }
    } else {
        ui->txtNumberOfShadows->setText(ok ? "Must be >0" : "Wrong number");
        ui->txtShadowThreshold->setText(ok2 ? "Must be >=2" : "Wrong number");
    }
}



void MainWindow::on_btnSelectImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    if (QString::compare(filename, QString()) != 0)
    {
        // Load the image using OpenCV in grayscale
        loadedImage = cv::imread(filename.toStdString(), cv::IMREAD_GRAYSCALE);

        if (!loadedImage.empty())
        {
            // Since the image is grayscale, each pixel is represented by a single byte.
            QImage qimg(loadedImage.data, loadedImage.cols, loadedImage.rows, loadedImage.step, QImage::Format_Grayscale8);
            ui->picSelected->setPixmap(QPixmap::fromImage(qimg.scaled(ui->picSelected->width(), ui->picSelected->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        }
        else
        {
            // Handle the invalid image case
        }
    }
}

void MainWindow::on_btnDecode_clicked() {
    if (selectedShadows.empty()) {
        QMessageBox::warning(this, "Error", "No shadows selected for decoding.");
        return;  // Stop execution of the function if no shadows are selected
    }
    for (const auto& shadow : selectedShadows) {
        // print the shadow
        std::cout << shadow.size() << std::endl;
    }
    std::cout << "shadowsThreshold: " << shadowsThreshold << std::endl;
    cv::Mat reconstructed = reconstructImage(selectedShadows, shadowsThreshold);
    // cv::imshow("org", loadedImage);
    // Convert the cv::Mat to QImage for display (assuming the Mat is in grayscale)
    QImage img((uchar*)reconstructed.data, reconstructed.cols, reconstructed.rows, reconstructed.step, QImage::Format_Grayscale8);

    // Display the QImage in a QLabel or other suitable Qt widget
    ui->picDecoded->setPixmap(QPixmap::fromImage(img.scaled(ui->picSelected->width(), ui->picSelected->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
}


void MainWindow::on_listGeneratedSh_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->listGeneratedSh->row(item);
    // Check if the index is within the valid range of generatedShadows
    if (index >= 0 && index < generatedShadows.size()) {
        cv::Mat share = generatedShadows[index];
        cv::imshow("Share Display", share);
    } else {
        QMessageBox::warning(this, "Error", "Invalid share index.");
    }
}


void MainWindow::on_listSelectedSh_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->listSelectedSh->row(item);
    // Check if the index is within the valid range of generatedShadows
    if (index >= 0 && index < selectedShadows.size()) {
        cv::Mat share = selectedShadows[index];
        cv::imshow("Share Display", share);
    } else {
        QMessageBox::warning(this, "Error", "Invalid share index.");
    }
}

////////////////////////////////////////////
// Menu bar actions
////////////////////////////////////////////


void MainWindow::on_actionHelp_triggered()
{
    //TODO: Implement help dialog
}


void MainWindow::on_actionAuthor_triggered()
{
    QMessageBox::information(this, "About Author", "This GUI was created by <b>inż. Mikołaj Osiecki</b> <i>mdosiecki@gmail.com</i> for the purpose of the master thesis" 
    "<br>"
    "<b>Scalable and multi-level algorithms for secret image data sharing</b>"
    "<br>"
    "at the"
    "<br>"
    "<b>AGH University of Krakow, Faculty of Electrical Engineering, Automatics, Computer Science and Biomedical Engineering</b>"
    "<br>"
    "Supervisor: <b>Prof Dr. Marek R. Ogiela</b>");
}


void MainWindow::on_actionAbout_Gui_triggered()
{
    QApplication::aboutQt();
}
