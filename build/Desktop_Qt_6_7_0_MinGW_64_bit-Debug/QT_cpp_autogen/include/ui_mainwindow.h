/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QPushButton *btnGenerateShadows;
    QLabel *picSelected;
    QLabel *picDecoded;
    QComboBox *comboEncodingType;
    QLineEdit *txtNumberOfShadows;
    QLabel *label;
    QPushButton *btnSelectImage;
    QWidget *layoutWidget;
    QHBoxLayout *ShadowsListGroup;
    QListWidget *listGeneratedSh;
    QVBoxLayout *ShadowsMoveGroup;
    QPushButton *btnSelectShadows;
    QPushButton *btnMoveBack;
    QPushButton *btnClear;
    QListWidget *listSelectedSh;
    QMenuBar *menubar;
    QStatusBar *statusbar;
    QToolBar *toolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1293, 834);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        btnGenerateShadows = new QPushButton(centralwidget);
        btnGenerateShadows->setObjectName("btnGenerateShadows");
        btnGenerateShadows->setGeometry(QRect(590, 440, 111, 24));
        picSelected = new QLabel(centralwidget);
        picSelected->setObjectName("picSelected");
        picSelected->setGeometry(QRect(50, 20, 531, 441));
        picSelected->setPixmap(QPixmap(QString::fromUtf8("../../../podanie_skan.jpg")));
        picDecoded = new QLabel(centralwidget);
        picDecoded->setObjectName("picDecoded");
        picDecoded->setGeometry(QRect(710, 20, 541, 441));
        picDecoded->setPixmap(QPixmap(QString::fromUtf8("C:/Users/Nosfreat/Desktop/voucher kurs fotografii 2024 Pawe\305\202 Sieci\305\204ski.jpg")));
        comboEncodingType = new QComboBox(centralwidget);
        comboEncodingType->addItem(QString());
        comboEncodingType->addItem(QString());
        comboEncodingType->addItem(QString());
        comboEncodingType->addItem(QString());
        comboEncodingType->setObjectName("comboEncodingType");
        comboEncodingType->setGeometry(QRect(610, 410, 72, 24));
        txtNumberOfShadows = new QLineEdit(centralwidget);
        txtNumberOfShadows->setObjectName("txtNumberOfShadows");
        txtNumberOfShadows->setGeometry(QRect(590, 380, 113, 24));
        txtNumberOfShadows->setFrame(true);
        txtNumberOfShadows->setClearButtonEnabled(true);
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(590, 360, 111, 20));
        btnSelectImage = new QPushButton(centralwidget);
        btnSelectImage->setObjectName("btnSelectImage");
        btnSelectImage->setGeometry(QRect(600, 80, 91, 24));
        layoutWidget = new QWidget(centralwidget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(30, 470, 1231, 321));
        ShadowsListGroup = new QHBoxLayout(layoutWidget);
        ShadowsListGroup->setObjectName("ShadowsListGroup");
        ShadowsListGroup->setContentsMargins(0, 0, 0, 0);
        listGeneratedSh = new QListWidget(layoutWidget);
        listGeneratedSh->setObjectName("listGeneratedSh");
        listGeneratedSh->setSelectionMode(QAbstractItemView::ExtendedSelection);

        ShadowsListGroup->addWidget(listGeneratedSh);

        ShadowsMoveGroup = new QVBoxLayout();
        ShadowsMoveGroup->setObjectName("ShadowsMoveGroup");
        btnSelectShadows = new QPushButton(layoutWidget);
        btnSelectShadows->setObjectName("btnSelectShadows");

        ShadowsMoveGroup->addWidget(btnSelectShadows);

        btnMoveBack = new QPushButton(layoutWidget);
        btnMoveBack->setObjectName("btnMoveBack");

        ShadowsMoveGroup->addWidget(btnMoveBack);

        btnClear = new QPushButton(layoutWidget);
        btnClear->setObjectName("btnClear");

        ShadowsMoveGroup->addWidget(btnClear);


        ShadowsListGroup->addLayout(ShadowsMoveGroup);

        listSelectedSh = new QListWidget(layoutWidget);
        listSelectedSh->setObjectName("listSelectedSh");
        listSelectedSh->setSelectionMode(QAbstractItemView::ExtendedSelection);

        ShadowsListGroup->addWidget(listSelectedSh);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1293, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName("toolBar");
        MainWindow->addToolBar(Qt::ToolBarArea::TopToolBarArea, toolBar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        btnGenerateShadows->setText(QCoreApplication::translate("MainWindow", "Generate Shadows", nullptr));
        picSelected->setText(QString());
        picDecoded->setText(QString());
        comboEncodingType->setItemText(0, QCoreApplication::translate("MainWindow", "Type1", nullptr));
        comboEncodingType->setItemText(1, QCoreApplication::translate("MainWindow", "Type2", nullptr));
        comboEncodingType->setItemText(2, QCoreApplication::translate("MainWindow", "Type3", nullptr));
        comboEncodingType->setItemText(3, QCoreApplication::translate("MainWindow", "Type4", nullptr));

        txtNumberOfShadows->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "Number of Shadows", nullptr));
        btnSelectImage->setText(QCoreApplication::translate("MainWindow", "Select Image", nullptr));
        btnSelectShadows->setText(QCoreApplication::translate("MainWindow", "Select", nullptr));
        btnMoveBack->setText(QCoreApplication::translate("MainWindow", "Move back", nullptr));
        btnClear->setText(QCoreApplication::translate("MainWindow", "Clear", nullptr));
        toolBar->setWindowTitle(QCoreApplication::translate("MainWindow", "toolBar", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
