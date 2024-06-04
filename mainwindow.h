#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPixmap>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <opencv2/core/mat.hpp> // Make sure to include the necessary header for cv::Mat
#include <QListWidgetItem>
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

    void on_btnDecode_clicked();

    void on_listGeneratedSh_itemDoubleClicked(QListWidgetItem *item);

    void on_listSelectedSh_itemDoubleClicked(QListWidgetItem *item);

    void on_actionHelp_triggered();

    void on_actionAbout_Gui_triggered();

    void on_actionAuthor_triggered();

private:
    Ui::MainWindow *ui;
    cv::Mat loadedImage;
    std::vector<cv::Mat> generatedShadows; // Hold the generated shadow images
    std::vector<cv::Mat> selectedShadows; // Hold the generated shadow images
    int shadowsThreshold;
    int shadowsAmount;
};
#endif // MAINWINDOW_H
