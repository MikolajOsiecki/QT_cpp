#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "functions.h"
#include "thien_lin_shadows.h"
#include <opencv2/highgui/highgui.hpp>
#include <QMessageBox>  // Ensure to include QMessageBox
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Define the stylesheet
    QString styleSheet = "QMainWindow { background-color: lightgray; color: black; }"
                         "QWidget { background-color: lightgray; color: black; }"
                         "QPushButton { background-color: lightgray; color: black; }"
                         "QLineEdit { background-color: lightgray; color: black; }"
                         "QLabel { background-color: lightgray; color: black; }"
                         "QListWidget { background-color: white; color: black; }"
                         "QComboBox { background-color: lightgray; color: black; }"
                         "QTextEdit { background-color: lightgray; color: black; }";
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
        // std::cout << "generatedShadows.size(): " << generatedShadows.size() << std::endl;
        // std::cout << "selectedShadows.size(): " << selectedShadows.size() << std::endl;

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
        // std::cout << "generatedShadows.size(): " << generatedShadows.size() << std::endl;
        // std::cout << "selectedShadows.size(): " << selectedShadows.size() << std::endl;


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
            // std::cout << "generatedShadows.size(): " << generatedShadows.size() << std::endl;
            // std::cout << "selectedShadows.size(): " << selectedShadows.size() << std::endl;

}


void MainWindow::on_btnGenerateShadows_clicked()
{
    QString ShadowsNumber = ui->txtNumberOfShadows->text();
    bool ok = false;
    shadowsAmount = ShadowsNumber.toInt(&ok);
    QString ShadowsThreshold = ui->txtShadowThreshold->text();
    bool ok2 = false;
    shadowsThreshold = ShadowsThreshold.toInt(&ok2);
    QString EssentialNumber = ui->txtNumberOfEssential->text();
    bool ok3 = false;
    essentialNumber = EssentialNumber.toInt(&ok3);
    QString EssentialThreshold = ui->txtEssentialThreshold->text();
    bool ok4 = false;
    essentialThreshold = EssentialThreshold.toInt(&ok4);
    bool usePadding = ui->checkBoxCropPadImage->isChecked();

    if (loadedImage.empty()) {
        QMessageBox::warning(this, "Error", "No image loaded or image is empty.");
        return;  // Stop execution of the function if image is not loaded
    }

    int selectedIndex = ui->dropdownEncodingType->currentIndex(); // Get the selected index from the dropdown
    ui->listGeneratedSh->clear();  // Clear old shadows
    ui->listSelectedSh->clear();  // Clear selected shadows
    generatedShadows.clear(); // Clear the selectedShadows vector
    selectedShadows.clear(); // Clear the selectedShadows vector

    switch (selectedIndex) {

        case 0:
            if (ok && ok2 && shadowsAmount > 0 && shadowsThreshold >= 2 && shadowsAmount >= shadowsThreshold) {
                generatedShadows = generateShadowsTL(loadedImage, shadowsThreshold, shadowsAmount);
                convertShadowsToStr(generatedShadows);
                changeShadowEssentialValue(generatedShadows, true);
                // Add each shadow string to the QListWidget
                for (const auto& shadow : generatedShadows) {
                    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(shadow.text), ui->listGeneratedSh);

                    if (shadow.isEssential) {
                        newItem->setBackground(Qt::red);  // Set background color to red
                    }

                    ui->listGeneratedSh->addItem(newItem);
                }
            } else {
                QMessageBox::warning(this, "Error", "Invalid parameters!");
            }
            break;

        case 1:
            if (ok && ok2 && shadowsAmount > 0 && shadowsThreshold >= 2 && shadowsAmount >= shadowsThreshold) {

                std::vector<cv::Mat> slices = sliceImageVertically(loadedImage, shadowsAmount, usePadding);
                std::vector<Shadow> allSubShadows;
                int WangLinAmount = 2 * shadowsAmount - shadowsThreshold;

                for (int i = 0; i < slices.size(); ++i) {
                    std::vector<Shadow> sliceShadows = generateShadowsTL(slices[i], shadowsAmount, WangLinAmount, i + 1);
                    allSubShadows.insert(allSubShadows.end(), sliceShadows.begin(), sliceShadows.end());
                }

                // Compose the shadows using the new function
                std::vector<Shadow> composedShadows = composeShadows(allSubShadows, shadowsAmount, shadowsThreshold);

                // Merge composed shadows into the main shadow list
                generatedShadows.insert(generatedShadows.end(), composedShadows.begin(), composedShadows.end());
                convertShadowsToStr(generatedShadows);
                changeShadowEssentialValue(generatedShadows, true);

                // Add each shadow string to the QListWidget
                for (const auto& shadow : generatedShadows) {
                    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(shadow.text), ui->listGeneratedSh);

                    if (shadow.isEssential) {
                        newItem->setBackground(Qt::red);  // Set background color to red
                    }

                    ui->listGeneratedSh->addItem(newItem);
                }

            } else {
                QMessageBox::warning(this, "Error", "Invalid parameters!");
            }
            break;

        case 2:
            // algorithm:
            // 1. use WangLin (k, s+k-t) to generate s+k-t temporary shadows (W1.....Ws+k-t)
            // 2. for each k-t temp shadow Wj (j=s+1,....,s+k-t) generate n subshadows wj,1 .....wj,n using ThienLin (k,n)
            // 3. for i=1,....,s essenital shadows Si are Si = Wi || ws+1,i || .... || ws+k-t,i (subshadow i form each of the k-t subsets of temp shadows)
            //    for i = s+1, .... n normal shadows Si are Si =ws+1,i || .... || ws+k-t,i (same as before but without main temp shadow)
            if (ok && ok2 && ok3 && ok4
                && shadowsAmount > 0 && shadowsThreshold >= 1 && shadowsAmount >= shadowsThreshold
                && essentialNumber > 0  && essentialThreshold >= 1 && essentialNumber >= essentialThreshold
                && essentialNumber <= shadowsAmount) {


                ///////////////// STEP 1 /////////////////////////
                int sktAmount = essentialNumber + shadowsThreshold - essentialThreshold;
                int sktWangLingAmount = 2*sktAmount - shadowsThreshold;
                std::cout << "sktAmount: " << sktAmount << std::endl;
                std::cout << "sktWangLingAmount: " << sktWangLingAmount << std::endl;

                std::vector<cv::Mat> slices = sliceImageVertically(loadedImage, sktAmount, usePadding);
                std::vector<Shadow> allTempShadows;

                for (int i = 0; i < slices.size(); ++i) {
                    std::vector<Shadow> sliceShadows = generateShadowsTL(slices[i], shadowsThreshold, sktWangLingAmount, i + 1);
                    // int k = 1;
                    // for (const auto& shadow : allSubShadows) {
                    //     std::string windowName = cv::format("sliceShadow %d", k);
                    //     cv::imshow(windowName, shadow.image);
                    //     k+=1;
                    // }
                    allTempShadows.insert(allTempShadows.end(), sliceShadows.begin(), sliceShadows.end());
                }
                // std::cout << "allSubShadows size: " << allSubShadows.size() << std::endl;
                // int k = 1;
                // for (const auto& shadow : allSubShadows) {
                //     std::string windowName = cv::format("Partition %d", k);
                    // cv::imshow(windowName, shadow.image);
                //     k+=1;
                // }
                // std::cout << "COMPOSING SHADOWS: " << sktAmount << std::endl;
                std::vector<Shadow> temporaryShadows = composeShadows(allTempShadows, sktAmount, shadowsThreshold);
                // std::cout << "composedShadows size: " << composedShadows.size() << std::endl;

                ///////////////// STEP 2 ////////////////////////
                std::vector<Shadow> allSubShadows;
                for(int i = essentialNumber + 1; i <=sktAmount; i++){
                    // iterator i could be without +1 but this way it is same as in original article (there it is s+1)
                    std::vector<Shadow> subShadows = generateShadowsTL(temporaryShadows[i-1].image, shadowsThreshold, shadowsAmount, i);
                    allSubShadows.insert(allSubShadows.end(), subShadows.begin(), subShadows.end());

                    for (const auto& shadow : subShadows) {
                        std::cout << "subShadow number: " << shadow.number << " subShadow sliceNumber: " << shadow.sliceNumber << std::endl;
                        // cv::imshow("subshadow", shadow.image);
                        // cv::waitKey();
                    }
                }

                ///////////////// STEP 3 ////////////////////////
                //Essential Shadows
                std::cout << "Essential Shadows " << std::endl;
                for(int i = 0; i < essentialNumber; i++){
                    std::vector<cv::Mat> essentialShadowComponents;
                    essentialShadowComponents.push_back(temporaryShadows[i].image);

                    std::vector<Shadow> selectedShadowNumber = copyShadowsWithNumber(allSubShadows, i+1);

                    for (const auto& shadow : selectedShadowNumber) {
                        std::cout << "selectedShadowNumber number: " << shadow.number << " selectedShadowNumber sliceNumber: " << shadow.sliceNumber << std::endl;
                        essentialShadowComponents.push_back(shadow.image);
                    }

                    cv::Mat essentialShadow = mergeSubImages(essentialShadowComponents);
                    generatedShadows.push_back({essentialShadow, true, "", i+1, -1});
                }

                //Normal Shadows
                std::cout << "Normal Shadows " << std::endl;
                for(int i = essentialNumber; i < shadowsAmount; i++){
                    std::vector<cv::Mat> normalShadowComponents;

                    std::vector<Shadow> selectedShadowNumber = copyShadowsWithNumber(allSubShadows, i+1);

                    for (const auto& shadow : selectedShadowNumber) {
                        std::cout << "selectedShadowNumber nromal number: " << shadow.number << " selectedShadowNumber nromal sliceNumber: " << shadow.sliceNumber << std::endl;
                        normalShadowComponents.push_back(shadow.image);
                    }

                    cv::Mat normalShadow = mergeSubImages(normalShadowComponents);
                    generatedShadows.push_back({normalShadow, false, "", i+1, -1});
                }

                // generatedShadows.insert(generatedShadows.end(), allSubShadows.begin(), allSubShadows.end());
                convertShadowsToStr(generatedShadows);

                // Add each shadow string to the QListWidget
                for (const auto& shadow : generatedShadows) {
                    QListWidgetItem* newItem = new QListWidgetItem(QString::fromStdString(shadow.text), ui->listGeneratedSh);

                    if (shadow.isEssential) {
                        newItem->setBackground(Qt::red);  // Set background color to red
                    }

                    ui->listGeneratedSh->addItem(newItem);
                }

            } else {
                QMessageBox::warning(this, "Error", "Invalid parameters!");
            }
            break;

        default:
            QMessageBox::warning(this, "Error", "Invalid selection in encoding type!");
            break;
        }

}


void MainWindow::on_btnDecode_clicked() {
    if (selectedShadows.empty()) {
        QMessageBox::warning(this, "Error", "No shadows selected for decoding.");
        return;  // Stop execution of the function if no shadows are selected
    }
    QString ShadowsNumber = ui->txtNumberOfShadows->text();
    bool ok = false;
    shadowsAmount = ShadowsNumber.toInt(&ok);
    QString ShadowsThreshold = ui->txtShadowThreshold->text();
    bool ok2 = false;
    shadowsThreshold = ShadowsThreshold.toInt(&ok2);

    int selectedIndex = ui->dropdownEncodingType->currentIndex(); // Get the selected index from the dropdown
    switch(selectedIndex){
        case 0:
            if (ok2 && shadowsThreshold >= 2) {
                cv::Mat reconstructed = decodeShadowsTL(selectedShadows, shadowsThreshold);
                // cv::imshow("Reconstructed Image", reconstructed);
                QImage img((uchar*)reconstructed.data, reconstructed.cols, reconstructed.rows, reconstructed.step, QImage::Format_Grayscale8);
                ui->picDecoded->setPixmap(QPixmap::fromImage(img.scaled(ui->picSelected->width(), ui->picSelected->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
            }
            break;

        case 1:
            if (ok && ok2 && shadowsAmount > 0 && shadowsThreshold >= 2 && shadowsAmount >= shadowsThreshold) {
                partitionedShadows.clear();
                decodedPartitions.clear();
                partitionedShadows = decomposeShadows(selectedShadows, shadowsAmount, shadowsThreshold);
                // int k =1;
                for(int i = 0; i < shadowsAmount; i++){
                    copiedPartitions.clear();
                    copiedPartitions = copyShadowsWithSliceNumber(partitionedShadows, i+1);


                    cv::Mat reconstructedPartition = decodeShadowsTL(copiedPartitions, shadowsAmount);
                    decodedPartitions.push_back(reconstructedPartition);


                    // if (i == 3){
                        // for (const auto& shadow : copiedPartitions) {
                            // std::cout << "Shadow Number: " << shadow.number << ", Slice Number: " << shadow.sliceNumber << std::endl;
                            // std::string fname = "DECOMPOSED_SHADOWS/DS" + std::to_string(shadow.number) + "_" + std::to_string(shadow.sliceNumber) + ".bmp";
                            // std::string windowName = cv::format("Partition %d", k);
                            // k+=1;
                            // cv::imshow(windowName, shadow.image);
                            // if (!cv::imwrite(fname, shadow.image)) {
                                // std::cout << "Error saving image: " << fname << std::endl;
                            // }
                        // }
                        // std::string windowName = cv::format("Partition %d", i + 1);
                        // cv::imshow(windowName, reconstructedPartition);
                        // cv::waitKey(0);
                    // }
                }
                cv::Mat reconstructed = mergeSubImages(decodedPartitions);
                QImage img((uchar*)reconstructed.data, reconstructed.cols, reconstructed.rows, reconstructed.step, QImage::Format_Grayscale8);
                ui->picDecoded->setPixmap(QPixmap::fromImage(img.scaled(ui->picSelected->width(), ui->picSelected->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
                // for (const auto& shadow : partitionedShadows) {
                //     std::cout << "Shadow Number: " << shadow.number << ", Slice Number: " << shadow.sliceNumber << std::endl;
                //     std::string fname = "DECOMPOSED_SHADOWS/DS" + std::to_string(shadow.number) + "_" + std::to_string(shadow.sliceNumber) + ".bmp";
                //     if (!cv::imwrite(fname, shadow.image)) {
                //         std::cerr << "Error saving image: " << fname << std::endl;
                //     }
                // }
                // std::cout << "=============================" << std::endl;
            }
            break;


        case 3:
            // algorithm (decode), input m shadows with r essential shadows (m>=k, r>=t):
            // 1. assume that essentail shadows r are S1,...Sr and normal m-r are Ss+1,...,Ss+m-r
            // 2. reconstruc temp shadows Wj j=s+1,...s+k-t using ThienLin(m) from m inputs wj,1....wj,s+1,....wj,s+m-r
            // 3. reconstruct r temp shadows W1,...Wr form r essential shadows S1,...Sr (notice Si = Wi || ws+1,i || .... || ws+k-t,i)
            // 4. generate secret image I using WangLin(r+k-t) from r+k-t input temp shadows W1,....Wr,....Ws+1,...Ws+k-t
            break;


        default:
            QMessageBox::warning(this, "Error", "Invalid selection in encoding type!");
            break;
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
            // Thresholding bec Thein-Lin (2002) scheme does not work for higher value, this will get rid of black pixels in decoding output for pixels with higher values on original image at the cost of changing of the original image
            cv::threshold(loadedImage, loadedImage, 250, 250, cv::THRESH_TRUNC);

            QImage qimg(loadedImage.data, loadedImage.cols, loadedImage.rows, loadedImage.step, QImage::Format_Grayscale8);
            ui->picSelected->setPixmap(QPixmap::fromImage(qimg.scaled(ui->picSelected->width(), ui->picSelected->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        }
        else
        {
            // Handle the invalid image case
        }
    }
}

void MainWindow::on_listGeneratedSh_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->listGeneratedSh->row(item);
    // std::cout<< "index: " << index << " ,generatedShadows.size(): " << generatedShadows.size() ;
    // std::cout << std::endl;
    // Check if the index is within the valid range of generatedShadows
    if (index >= 0 && index < generatedShadows.size()) {
        // Open the image in a new Qt window
        ImageViewer *viewer = new ImageViewer();
        viewer->setImage(generatedShadows[index].image);
        viewer->setWindowTitle("Share Display");
        viewer->setAttribute(Qt::WA_DeleteOnClose); // Ensure the window is deleted when closed
        viewer->show();
    } else {
        QMessageBox::warning(this, "Error", "Invalid share index.");
    }
}

void MainWindow::on_listSelectedSh_itemDoubleClicked(QListWidgetItem *item)
{
    int index = ui->listSelectedSh->row(item);
    // Check if the index is within the valid range of selectedShadows
    if (index >= 0 && index < selectedShadows.size()) {
        // Open the image in a new Qt window
        ImageViewer *viewer = new ImageViewer();
        viewer->setImage(selectedShadows[index].image);
        viewer->setWindowTitle("Share Display");
        viewer->setAttribute(Qt::WA_DeleteOnClose); // Ensure the window is deleted when closed
        viewer->show();
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
    QMessageBox::information(this, "About Author", "This program was created by <b>inż. Mikołaj Osiecki</b> <i>mdosiecki@gmail.com</i> for the purpose of the master thesis"
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
