/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QMainWindow>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTextEdit>
#include <QtGui/QToolBar>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton;
    QPushButton *pushButton_ros;
    QPushButton *pushButton_init;
    QPushButton *pushButton_arm;
    QPushButton *pushButton_offboard;
    QTextEdit *textEdit;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_naruto;
    QLabel *label__larr;
    QLabel *label;
    QMenuBar *menuBar;
    QMenu *menuPx4_code;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(691, 347);
        MainWindow->setStyleSheet(QString::fromUtf8("background-color: rgb(255, 255, 255);\n"
"selection-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0.4689 rgba(230, 230, 230, 255), stop:1 rgba(255, 255, 255, 255));\n"
""));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(20, 0, 582, 271));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        pushButton = new QPushButton(verticalLayoutWidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);

        horizontalLayout->addWidget(pushButton);

        pushButton_ros = new QPushButton(verticalLayoutWidget);
        pushButton_ros->setObjectName(QString::fromUtf8("pushButton_ros"));
        sizePolicy.setHeightForWidth(pushButton_ros->sizePolicy().hasHeightForWidth());
        pushButton_ros->setSizePolicy(sizePolicy);
        pushButton_ros->setStyleSheet(QString::fromUtf8("border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
""));
        pushButton_ros->setCheckable(true);

        horizontalLayout->addWidget(pushButton_ros);

        pushButton_init = new QPushButton(verticalLayoutWidget);
        pushButton_init->setObjectName(QString::fromUtf8("pushButton_init"));
        sizePolicy.setHeightForWidth(pushButton_init->sizePolicy().hasHeightForWidth());
        pushButton_init->setSizePolicy(sizePolicy);
        pushButton_init->setStyleSheet(QString::fromUtf8("border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
""));

        horizontalLayout->addWidget(pushButton_init);

        pushButton_arm = new QPushButton(verticalLayoutWidget);
        pushButton_arm->setObjectName(QString::fromUtf8("pushButton_arm"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(pushButton_arm->sizePolicy().hasHeightForWidth());
        pushButton_arm->setSizePolicy(sizePolicy1);
        pushButton_arm->setStyleSheet(QString::fromUtf8("border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
""));
        pushButton_arm->setCheckable(true);

        horizontalLayout->addWidget(pushButton_arm);

        pushButton_offboard = new QPushButton(verticalLayoutWidget);
        pushButton_offboard->setObjectName(QString::fromUtf8("pushButton_offboard"));
        sizePolicy.setHeightForWidth(pushButton_offboard->sizePolicy().hasHeightForWidth());
        pushButton_offboard->setSizePolicy(sizePolicy);
        pushButton_offboard->setStyleSheet(QString::fromUtf8("border-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(0, 0, 0, 255), stop:1 rgba(255, 255, 255, 255));\n"
"\n"
""));
        pushButton_offboard->setCheckable(false);

        horizontalLayout->addWidget(pushButton_offboard);


        verticalLayout->addLayout(horizontalLayout);

        textEdit = new QTextEdit(verticalLayoutWidget);
        textEdit->setObjectName(QString::fromUtf8("textEdit"));
        sizePolicy1.setHeightForWidth(textEdit->sizePolicy().hasHeightForWidth());
        textEdit->setSizePolicy(sizePolicy1);

        verticalLayout->addWidget(textEdit);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_naruto = new QLabel(verticalLayoutWidget);
        label_naruto->setObjectName(QString::fromUtf8("label_naruto"));

        horizontalLayout_2->addWidget(label_naruto);

        label__larr = new QLabel(verticalLayoutWidget);
        label__larr->setObjectName(QString::fromUtf8("label__larr"));

        horizontalLayout_2->addWidget(label__larr);

        label = new QLabel(verticalLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout_2->addWidget(label);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalLayout->setStretch(0, 2);
        verticalLayout->setStretch(1, 3);
        verticalLayout->setStretch(2, 2);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 691, 25));
        menuPx4_code = new QMenu(menuBar);
        menuPx4_code->setObjectName(QString::fromUtf8("menuPx4_code"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuPx4_code->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "px4_gcs", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("MainWindow", "MAV_WRAPPER", 0, QApplication::UnicodeUTF8));
        pushButton_ros->setText(QApplication::translate("MainWindow", "ROS connect", 0, QApplication::UnicodeUTF8));
        pushButton_init->setText(QApplication::translate("MainWindow", "Initialize homing", 0, QApplication::UnicodeUTF8));
        pushButton_arm->setText(QApplication::translate("MainWindow", "Disarmed", 0, QApplication::UnicodeUTF8));
        pushButton_offboard->setText(QApplication::translate("MainWindow", "Set offboard", 0, QApplication::UnicodeUTF8));
        label_naruto->setText(QString());
        label__larr->setText(QString());
        label->setText(QString());
        menuPx4_code->setTitle(QApplication::translate("MainWindow", "px4_code", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
