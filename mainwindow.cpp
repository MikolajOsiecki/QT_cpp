#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "functions.h"

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
    if (ok){
        ok = number > 0 ;
        if (ok){

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
        }
    }
    else{
        ui->txtNumberOfShadows->setText("Wrong number");
    }
}


void MainWindow::on_btnSelectImage_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Choose file"), "");
    if (QString::compare(filename, QString()) != 0 )
    {
        QImage image;
        bool valid = image.load(filename);

        if (valid)
        {
            image = image.scaled(ui->picSelected->width(), ui->picSelected->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
            ui->picSelected->setPixmap(QPixmap::fromImage(image));
        }
        else
        {

        }
    }
}
