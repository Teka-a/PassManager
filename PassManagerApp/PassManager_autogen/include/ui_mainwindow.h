/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QGridLayout *gridLayout;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_2;
    QSpacerItem *horizontalSpacer_4;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *enterBtn;
    QSpacerItem *verticalSpacer_4;
    QSpacerItem *verticalSpacer;
    QLineEdit *pin;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *locked;
    QLabel *wrongPinInfo;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_3;
    QWidget *page_2;
    QGridLayout *gridLayout_3;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_7;
    QLineEdit *searchline;
    QPushButton *searchBtn;
    QSpacerItem *horizontalSpacer_6;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *addBtn;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *genSettingsBtn;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QLabel *noCreds;
    QWidget *page_3;
    QGridLayout *gridLayout_5;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *horizontalSpacer_8;
    QSpacerItem *horizontalSpacer_9;
    QGridLayout *gridLayout_4;
    QLineEdit *loginLine;
    QPushButton *echoLogin;
    QLineEdit *passwordLine;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *insertBtn;
    QLineEdit *siteLine;
    QPushButton *echoPass;
    QSpacerItem *verticalSpacer_6;
    QPushButton *retHome;
    QWidget *page_4;
    QGridLayout *gridLayout_7;
    QSpacerItem *horizontalSpacer_11;
    QSpacerItem *horizontalSpacer_10;
    QGridLayout *gridLayout_6;
    QHBoxLayout *horizontalLayout_6;
    QPushButton *updBtn;
    QPushButton *echoPass_2;
    QPushButton *echoLogin_2;
    QLineEdit *loginChange;
    QLabel *info;
    QLineEdit *passwordChange;
    QLabel *label_2;
    QSpacerItem *verticalSpacer_7;
    QSpacerItem *verticalSpacer_8;
    QPushButton *retHome_2;
    QLabel *label;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 620);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        gridLayout = new QGridLayout(centralwidget);
        gridLayout->setObjectName("gridLayout");
        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName("stackedWidget");
        page = new QWidget();
        page->setObjectName("page");
        page->setMaximumSize(QSize(488, 364));
        gridLayout_2 = new QGridLayout(page);
        gridLayout_2->setObjectName("gridLayout_2");
        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 3, 0, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_3, 3, 2, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        enterBtn = new QPushButton(page);
        enterBtn->setObjectName("enterBtn");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(enterBtn->sizePolicy().hasHeightForWidth());
        enterBtn->setSizePolicy(sizePolicy);
        enterBtn->setMinimumSize(QSize(100, 30));

        horizontalLayout_2->addWidget(enterBtn);


        gridLayout_2->addLayout(horizontalLayout_2, 5, 1, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 0, 1, 1, 1);

        verticalSpacer = new QSpacerItem(20, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(verticalSpacer, 2, 1, 1, 1);

        pin = new QLineEdit(page);
        pin->setObjectName("pin");
        pin->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_2->addWidget(pin, 3, 1, 1, 1);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        locked = new QLabel(page);
        locked->setObjectName("locked");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(locked->sizePolicy().hasHeightForWidth());
        locked->setSizePolicy(sizePolicy1);
        locked->setMinimumSize(QSize(20, 20));
        locked->setMaximumSize(QSize(20, 20));
        locked->setScaledContents(true);

        horizontalLayout->addWidget(locked);

        wrongPinInfo = new QLabel(page);
        wrongPinInfo->setObjectName("wrongPinInfo");

        horizontalLayout->addWidget(wrongPinInfo);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        gridLayout_2->addLayout(horizontalLayout, 1, 1, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 6, 1, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 10, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        gridLayout_2->addItem(verticalSpacer_3, 4, 1, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName("page_2");
        gridLayout_3 = new QGridLayout(page_2);
        gridLayout_3->setObjectName("gridLayout_3");
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        searchline = new QLineEdit(page_2);
        searchline->setObjectName("searchline");

        horizontalLayout_4->addWidget(searchline);

        searchBtn = new QPushButton(page_2);
        searchBtn->setObjectName("searchBtn");
        searchBtn->setMinimumSize(QSize(30, 40));
        searchBtn->setMaximumSize(QSize(30, 40));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/rc/icons/search.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        searchBtn->setIcon(icon);
        searchBtn->setIconSize(QSize(20, 20));
        searchBtn->setFlat(true);

        horizontalLayout_4->addWidget(searchBtn);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_6);


        gridLayout_3->addLayout(horizontalLayout_4, 1, 0, 1, 2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        addBtn = new QPushButton(page_2);
        addBtn->setObjectName("addBtn");
        addBtn->setMinimumSize(QSize(30, 40));
        addBtn->setMaximumSize(QSize(30, 40));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/rc/icons/add.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        addBtn->setIcon(icon1);
        addBtn->setIconSize(QSize(20, 20));
        addBtn->setFlat(true);

        horizontalLayout_3->addWidget(addBtn);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_5);

        genSettingsBtn = new QPushButton(page_2);
        genSettingsBtn->setObjectName("genSettingsBtn");
        genSettingsBtn->setMinimumSize(QSize(30, 40));
        genSettingsBtn->setMaximumSize(QSize(30, 40));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/rc/icons/settings.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        genSettingsBtn->setIcon(icon2);
        genSettingsBtn->setIconSize(QSize(20, 20));
        genSettingsBtn->setFlat(true);

        horizontalLayout_3->addWidget(genSettingsBtn);


        gridLayout_3->addLayout(horizontalLayout_3, 0, 0, 1, 2);

        scrollArea = new QScrollArea(page_2);
        scrollArea->setObjectName("scrollArea");
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName("scrollAreaWidgetContents");
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 762, 462));
        scrollArea->setWidget(scrollAreaWidgetContents);

        gridLayout_3->addWidget(scrollArea, 3, 0, 1, 2);

        noCreds = new QLabel(page_2);
        noCreds->setObjectName("noCreds");
        noCreds->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_3->addWidget(noCreds, 2, 0, 1, 2);

        stackedWidget->addWidget(page_2);
        page_3 = new QWidget();
        page_3->setObjectName("page_3");
        gridLayout_5 = new QGridLayout(page_3);
        gridLayout_5->setObjectName("gridLayout_5");
        verticalSpacer_5 = new QSpacerItem(20, 202, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_5->addItem(verticalSpacer_5, 4, 2, 1, 1);

        horizontalSpacer_8 = new QSpacerItem(248, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_8, 3, 3, 1, 1);

        horizontalSpacer_9 = new QSpacerItem(100, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_5->addItem(horizontalSpacer_9, 3, 0, 1, 1);

        gridLayout_4 = new QGridLayout();
        gridLayout_4->setObjectName("gridLayout_4");
        loginLine = new QLineEdit(page_3);
        loginLine->setObjectName("loginLine");
        loginLine->setMinimumSize(QSize(0, 30));
        loginLine->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_4->addWidget(loginLine, 3, 1, 1, 1);

        echoLogin = new QPushButton(page_3);
        echoLogin->setObjectName("echoLogin");
        echoLogin->setMinimumSize(QSize(30, 40));
        echoLogin->setMaximumSize(QSize(30, 40));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/rc/icons/show.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        echoLogin->setIcon(icon3);
        echoLogin->setIconSize(QSize(17, 17));
        echoLogin->setFlat(true);

        gridLayout_4->addWidget(echoLogin, 3, 2, 1, 1);

        passwordLine = new QLineEdit(page_3);
        passwordLine->setObjectName("passwordLine");
        passwordLine->setMinimumSize(QSize(0, 30));
        passwordLine->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_4->addWidget(passwordLine, 4, 1, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        insertBtn = new QPushButton(page_3);
        insertBtn->setObjectName("insertBtn");
        insertBtn->setMinimumSize(QSize(100, 30));
        insertBtn->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_5->addWidget(insertBtn);


        gridLayout_4->addLayout(horizontalLayout_5, 5, 1, 1, 1);

        siteLine = new QLineEdit(page_3);
        siteLine->setObjectName("siteLine");
        siteLine->setMinimumSize(QSize(0, 30));

        gridLayout_4->addWidget(siteLine, 2, 1, 1, 1);

        echoPass = new QPushButton(page_3);
        echoPass->setObjectName("echoPass");
        echoPass->setMinimumSize(QSize(30, 40));
        echoPass->setMaximumSize(QSize(30, 40));
        echoPass->setIcon(icon3);
        echoPass->setIconSize(QSize(17, 17));
        echoPass->setFlat(true);

        gridLayout_4->addWidget(echoPass, 4, 2, 1, 1);


        gridLayout_5->addLayout(gridLayout_4, 3, 2, 1, 1);

        verticalSpacer_6 = new QSpacerItem(20, 202, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_5->addItem(verticalSpacer_6, 1, 2, 1, 1);

        retHome = new QPushButton(page_3);
        retHome->setObjectName("retHome");
        retHome->setMinimumSize(QSize(30, 40));
        retHome->setMaximumSize(QSize(30, 40));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/rc/icons/home.png"), QSize(), QIcon::Mode::Normal, QIcon::State::Off);
        retHome->setIcon(icon4);
        retHome->setIconSize(QSize(20, 20));
        retHome->setAutoDefault(false);
        retHome->setFlat(true);

        gridLayout_5->addWidget(retHome, 0, 0, 1, 1);

        stackedWidget->addWidget(page_3);
        page_4 = new QWidget();
        page_4->setObjectName("page_4");
        gridLayout_7 = new QGridLayout(page_4);
        gridLayout_7->setObjectName("gridLayout_7");
        horizontalSpacer_11 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_11, 2, 2, 1, 1);

        horizontalSpacer_10 = new QSpacerItem(40, 20, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        gridLayout_7->addItem(horizontalSpacer_10, 2, 0, 1, 1);

        gridLayout_6 = new QGridLayout();
        gridLayout_6->setObjectName("gridLayout_6");
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName("horizontalLayout_6");
        updBtn = new QPushButton(page_4);
        updBtn->setObjectName("updBtn");
        updBtn->setMinimumSize(QSize(100, 30));
        updBtn->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_6->addWidget(updBtn);


        gridLayout_6->addLayout(horizontalLayout_6, 6, 1, 1, 2);

        echoPass_2 = new QPushButton(page_4);
        echoPass_2->setObjectName("echoPass_2");
        echoPass_2->setMinimumSize(QSize(30, 40));
        echoPass_2->setMaximumSize(QSize(30, 40));
        echoPass_2->setIcon(icon3);
        echoPass_2->setIconSize(QSize(17, 17));
        echoPass_2->setFlat(true);

        gridLayout_6->addWidget(echoPass_2, 5, 2, 1, 1);

        echoLogin_2 = new QPushButton(page_4);
        echoLogin_2->setObjectName("echoLogin_2");
        echoLogin_2->setMinimumSize(QSize(30, 40));
        echoLogin_2->setMaximumSize(QSize(30, 40));
        echoLogin_2->setIcon(icon3);
        echoLogin_2->setIconSize(QSize(17, 17));
        echoLogin_2->setFlat(true);

        gridLayout_6->addWidget(echoLogin_2, 4, 2, 1, 1);

        loginChange = new QLineEdit(page_4);
        loginChange->setObjectName("loginChange");
        loginChange->setMinimumSize(QSize(0, 30));
        loginChange->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_6->addWidget(loginChange, 4, 1, 1, 1);

        info = new QLabel(page_4);
        info->setObjectName("info");
        QFont font;
        font.setPointSize(13);
        info->setFont(font);
        info->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_6->addWidget(info, 3, 1, 1, 2);

        passwordChange = new QLineEdit(page_4);
        passwordChange->setObjectName("passwordChange");
        passwordChange->setMinimumSize(QSize(0, 30));
        passwordChange->setEchoMode(QLineEdit::EchoMode::Password);

        gridLayout_6->addWidget(passwordChange, 5, 1, 1, 1);

        label_2 = new QLabel(page_4);
        label_2->setObjectName("label_2");
        label_2->setFont(font);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout_6->addWidget(label_2, 2, 1, 1, 2);


        gridLayout_7->addLayout(gridLayout_6, 2, 1, 1, 1);

        verticalSpacer_7 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_7->addItem(verticalSpacer_7, 3, 1, 1, 1);

        verticalSpacer_8 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout_7->addItem(verticalSpacer_8, 1, 1, 1, 1);

        retHome_2 = new QPushButton(page_4);
        retHome_2->setObjectName("retHome_2");
        retHome_2->setMinimumSize(QSize(30, 40));
        retHome_2->setMaximumSize(QSize(30, 40));
        retHome_2->setIcon(icon4);
        retHome_2->setIconSize(QSize(20, 20));
        retHome_2->setFlat(true);

        gridLayout_7->addWidget(retHome_2, 0, 0, 1, 1);

        label = new QLabel(page_4);
        label->setObjectName("label");
        QFont font1;
        font1.setPointSize(9);
        label->setFont(font1);

        gridLayout_7->addWidget(label, 0, 2, 1, 1);

        stackedWidget->addWidget(page_4);

        gridLayout->addWidget(stackedWidget, 1, 0, 1, 1);

        MainWindow->setCentralWidget(centralwidget);

        retranslateUi(MainWindow);

        stackedWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        enterBtn->setText(QCoreApplication::translate("MainWindow", "\320\222\321\205\320\276\320\264", nullptr));
        pin->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 PIN-\320\272\320\276\320\264...", nullptr));
        locked->setText(QString());
        wrongPinInfo->setText(QCoreApplication::translate("MainWindow", "\320\235\320\265\320\262\320\265\321\200\320\275\321\213\320\271 \320\277\320\270\320\275", nullptr));
        searchline->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\235\320\260\321\207\320\275\320\270\321\202\320\265 \320\277\320\270\321\201\320\260\321\202\321\214...", nullptr));
        searchBtn->setText(QString());
        addBtn->setText(QString());
        genSettingsBtn->setText(QString());
        noCreds->setText(QCoreApplication::translate("MainWindow", "\320\235\320\265\321\202 \320\264\320\260\320\275\320\275\321\213\321\205", nullptr));
        loginLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\273\320\276\320\263\320\270\320\275", nullptr));
        echoLogin->setText(QString());
        passwordLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        insertBtn->setText(QCoreApplication::translate("MainWindow", "\320\224\320\276\320\261\320\260\320\262\320\270\321\202\321\214", nullptr));
        siteLine->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\222\320\262\320\265\320\264\320\270\321\202\320\265 \320\275\320\260\320\267\320\262\320\260\320\275\320\270\320\265 \321\201\320\260\320\271\321\202\320\260", nullptr));
        echoPass->setText(QString());
        retHome->setText(QString());
        updBtn->setText(QCoreApplication::translate("MainWindow", "\320\230\320\267\320\274\320\265\320\275\320\270\321\202\321\214", nullptr));
        echoPass_2->setText(QString());
        echoLogin_2->setText(QString());
        loginChange->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \320\273\320\276\320\263\320\270\320\275", nullptr));
        info->setText(QString());
        passwordChange->setPlaceholderText(QCoreApplication::translate("MainWindow", "\320\235\320\276\320\262\321\213\320\271 \320\277\320\260\321\200\320\276\320\273\321\214", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "\320\243\321\207\320\265\321\202\320\275\321\213\320\265 \320\264\320\260\320\275\320\275\321\213\320\265 \320\264\320\273\321\217 \321\201\320\260\320\271\321\202\320\260", nullptr));
        retHome_2->setText(QString());
        label->setText(QCoreApplication::translate("MainWindow", "\320\236\321\201\321\202\320\260\320\262\321\214\321\202\320\265 \320\277\320\276\320\273\320\265 \320\277\321\203\321\201\321\202\321\213\320\274, \320\265\321\201\320\273\320\270 \320\275\320\265 \321\205\320\276\321\202\320\270\321\202\320\265 \320\265\320\263\320\276 \320\270\320\267\320\274\320\265\320\275\321\217\321\202\321\214", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
