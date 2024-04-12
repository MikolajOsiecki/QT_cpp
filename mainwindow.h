#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <opencv2/core/mat.hpp> // Make sure to include the necessary header for cv::Mat

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnClear_clicked();

    void on_btnMoveBack_clicked();

    void on_btnSelectShadows_clicked();

    void on_btnGenerateShadows_clicked();

    void on_btnSelectImage_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat loadedImage; // Declare the cv::Mat variable here

};
#endif // MAINWINDOW_H
