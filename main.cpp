#include "mainwindow.h"

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/ssl.h>

#include <QApplication>
#include <QStyleFactory>

#include <sys/ptrace.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        if (errno == EPERM) {
            qDebug() << "Debugger attached!";
        }
        qDebug() << "no debugger attached!";
    }

    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(11, 3, 23));
    darkPalette.setColor(QPalette::WindowText, QColor(196, 196, 196));
    darkPalette.setColor(QPalette::Base, Qt::white);
    darkPalette.setColor(QPalette::AlternateBase, Qt::gray);
    darkPalette.setColor(QPalette::ToolTipBase, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::ToolTipText, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::Text, Qt::gray);
    darkPalette.setColor(QPalette::ButtonText, Qt::gray);
    darkPalette.setColor(QPalette::BrightText, Qt::red);
    darkPalette.setColor(QPalette::Link, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::Highlight, QColor(65, 63, 167));
    darkPalette.setColor(QPalette::HighlightedText, Qt::white);

    qApp->setPalette(darkPalette);

    qApp->setStyleSheet("QPushButton { border: none; background: transparent; } "
                        "QLineEdit { border: none; border-bottom: 1px solid #C4C4C4; padding: 5px; background: transparent;  } "
                        "QLabel {font: 12px; } "
                        "QLineEdit:focus { border-bottom: 1px solid #413FA7; }"
                        "QPushButton:hover { border-bottom: 3px solid #0B0317; padding: 5px; }"
                        "QLineEdit[text=\"\"] {color: gray;}"
                        );

    MainWindow w;
    w.show();
    return a.exec();
}
