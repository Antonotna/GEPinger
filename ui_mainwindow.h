/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon 9. Nov 22:08:57 2015
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QComboBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpinBox>
#include <QtGui/QWidget>
#include <mylistwidget.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QLineEdit *ipAddr;
    QLabel *label;
    QPushButton *ping;
    QGroupBox *groupBox;
    QLineEdit *ipTos;
    QLabel *label_2;
    QLabel *label_8;
    QLineEdit *ipTtl;
    QCheckBox *checkBox;
    QComboBox *dscpBox;
    QRadioButton *tos_dscp;
    QRadioButton *tos_custom;
    QGroupBox *groupBox_2;
    QLabel *label_9;
    QLineEdit *sid;
    QLabel *label_10;
    QLineEdit *sn;
    QGroupBox *groupBox_3;
    QLabel *label_3;
    QLineEdit *count;
    QLabel *label_4;
    QLineEdit *timeout;
    QLabel *label_5;
    QLineEdit *inter;
    QLabel *label_7;
    QSpinBox *datasize;
    QLabel *label_11;
    MyListWidget *Output;
    QPushButton *pause;
    QLabel *ip;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(835, 562);
        MainWindow->setMinimumSize(QSize(835, 562));
        MainWindow->setMaximumSize(QSize(835, 562));
        QFont font;
        font.setBold(false);
        font.setItalic(false);
        font.setUnderline(false);
        font.setWeight(50);
        MainWindow->setFont(font);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        ipAddr = new QLineEdit(centralWidget);
        ipAddr->setObjectName(QString::fromUtf8("ipAddr"));
        ipAddr->setGeometry(QRect(70, 20, 131, 20));
        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 20, 44, 13));
        ping = new QPushButton(centralWidget);
        ping->setObjectName(QString::fromUtf8("ping"));
        ping->setGeometry(QRect(20, 530, 75, 23));
        groupBox = new QGroupBox(centralWidget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 170, 401, 121));
        ipTos = new QLineEdit(groupBox);
        ipTos->setObjectName(QString::fromUtf8("ipTos"));
        ipTos->setGeometry(QRect(100, 50, 51, 20));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 20, 31, 21));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(190, 20, 21, 21));
        ipTtl = new QLineEdit(groupBox);
        ipTtl->setObjectName(QString::fromUtf8("ipTtl"));
        ipTtl->setGeometry(QRect(210, 20, 41, 20));
        checkBox = new QCheckBox(groupBox);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(260, 20, 61, 21));
        dscpBox = new QComboBox(groupBox);
        dscpBox->setObjectName(QString::fromUtf8("dscpBox"));
        dscpBox->setGeometry(QRect(100, 20, 71, 22));
        tos_dscp = new QRadioButton(groupBox);
        tos_dscp->setObjectName(QString::fromUtf8("tos_dscp"));
        tos_dscp->setGeometry(QRect(40, 20, 51, 17));
        tos_custom = new QRadioButton(groupBox);
        tos_custom->setObjectName(QString::fromUtf8("tos_custom"));
        tos_custom->setGeometry(QRect(40, 50, 61, 17));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(10, 300, 401, 71));
        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(10, 20, 31, 21));
        sid = new QLineEdit(groupBox_2);
        sid->setObjectName(QString::fromUtf8("sid"));
        sid->setGeometry(QRect(40, 20, 61, 20));
        sid->setMaxLength(5);
        label_10 = new QLabel(groupBox_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(110, 20, 46, 21));
        sn = new QLineEdit(groupBox_2);
        sn->setObjectName(QString::fromUtf8("sn"));
        sn->setGeometry(QRect(160, 20, 41, 20));
        groupBox_3 = new QGroupBox(centralWidget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 50, 401, 101));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 20, 51, 21));
        count = new QLineEdit(groupBox_3);
        count->setObjectName(QString::fromUtf8("count"));
        count->setGeometry(QRect(60, 20, 71, 20));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(140, 20, 41, 21));
        timeout = new QLineEdit(groupBox_3);
        timeout->setObjectName(QString::fromUtf8("timeout"));
        timeout->setGeometry(QRect(190, 20, 41, 20));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(250, 20, 21, 21));
        inter = new QLineEdit(groupBox_3);
        inter->setObjectName(QString::fromUtf8("inter"));
        inter->setGeometry(QRect(280, 20, 31, 20));
        label_7 = new QLabel(groupBox_3);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(320, 20, 31, 21));
        datasize = new QSpinBox(groupBox_3);
        datasize->setObjectName(QString::fromUtf8("datasize"));
        datasize->setGeometry(QRect(60, 60, 71, 22));
        label_11 = new QLabel(groupBox_3);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 60, 46, 21));
        Output = new MyListWidget(centralWidget);
        Output->setObjectName(QString::fromUtf8("Output"));
        Output->setGeometry(QRect(430, 10, 391, 541));
        Output->setSelectionMode(QAbstractItemView::ExtendedSelection);
        Output->setSelectionBehavior(QAbstractItemView::SelectItems);
        pause = new QPushButton(centralWidget);
        pause->setObjectName(QString::fromUtf8("pause"));
        pause->setGeometry(QRect(110, 530, 75, 23));
        ip = new QLabel(centralWidget);
        ip->setObjectName(QString::fromUtf8("ip"));
        ip->setGeometry(QRect(220, 21, 151, 20));
        MainWindow->setCentralWidget(centralWidget);
        QWidget::setTabOrder(ipAddr, count);
        QWidget::setTabOrder(count, timeout);
        QWidget::setTabOrder(timeout, inter);
        QWidget::setTabOrder(inter, datasize);
        QWidget::setTabOrder(datasize, ipTos);
        QWidget::setTabOrder(ipTos, ipTtl);
        QWidget::setTabOrder(ipTtl, checkBox);
        QWidget::setTabOrder(checkBox, sid);
        QWidget::setTabOrder(sid, sn);
        QWidget::setTabOrder(sn, ping);
        QWidget::setTabOrder(ping, pause);
        QWidget::setTabOrder(pause, Output);

        retranslateUi(MainWindow);
        QObject::connect(ping, SIGNAL(clicked()), MainWindow, SLOT(on_click()));
        QObject::connect(ipAddr, SIGNAL(returnPressed()), MainWindow, SLOT(on_click()));
        QObject::connect(pause, SIGNAL(clicked()), MainWindow, SLOT(pause_click()));
        QObject::connect(tos_dscp, SIGNAL(toggled(bool)), MainWindow, SLOT(radio_chg(bool)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "GEPinger", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("MainWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'MS Shell Dlg 2'; font-size:8.25pt; font-weight:400; font-style:normal;\">\n"
"<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:8pt; font-weight:600;\">Host/IP</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        ping->setText(QApplication::translate("MainWindow", "Ping", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("MainWindow", "IP Header", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("MainWindow", "TOS", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("MainWindow", "TTL", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("MainWindow", "DF bit", 0, QApplication::UnicodeUTF8));
        dscpBox->clear();
        dscpBox->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "ef(46)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs0(0)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs1(8)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af11(10)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af12(12)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af13(14)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs2(16)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af21(18)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af22(20)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af23(22)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs3(24)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af31(26)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af32(28)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af33(30)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs4(32)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af41(34)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af42(36)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "af43(38)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs5(40)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs6(48)", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("MainWindow", "cs7(56)", 0, QApplication::UnicodeUTF8)
        );
        tos_dscp->setText(QApplication::translate("MainWindow", "dscp", 0, QApplication::UnicodeUTF8));
        tos_custom->setText(QApplication::translate("MainWindow", "Custom", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QApplication::translate("MainWindow", "ICMP Header", 0, QApplication::UnicodeUTF8));
        label_9->setText(QApplication::translate("MainWindow", "SID", 0, QApplication::UnicodeUTF8));
        sid->setText(QApplication::translate("MainWindow", "1234", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("MainWindow", "SN start", 0, QApplication::UnicodeUTF8));
        sn->setText(QApplication::translate("MainWindow", "1", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "Options", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("MainWindow", "Count", 0, QApplication::UnicodeUTF8));
        count->setText(QApplication::translate("MainWindow", "10", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("MainWindow", "Timeout", 0, QApplication::UnicodeUTF8));
        timeout->setText(QApplication::translate("MainWindow", "3", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("MainWindow", "inter", 0, QApplication::UnicodeUTF8));
        inter->setText(QApplication::translate("MainWindow", "0", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("MainWindow", "ms", 0, QApplication::UnicodeUTF8));
        label_11->setText(QApplication::translate("MainWindow", "PktSize", 0, QApplication::UnicodeUTF8));
        pause->setText(QApplication::translate("MainWindow", "Pause ||", 0, QApplication::UnicodeUTF8));
        ip->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
