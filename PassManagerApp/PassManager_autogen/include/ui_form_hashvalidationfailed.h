/********************************************************************************
** Form generated from reading UI file 'form_hashvalidationfailed.ui'
**
** Created by: Qt User Interface Compiler version 6.8.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_HASHVALIDATIONFAILED_H
#define UI_FORM_HASHVALIDATIONFAILED_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form_HashValidationFailed
{
public:
    QGridLayout *gridLayout;
    QLabel *label_3;
    QLabel *label_2;
    QLabel *label;
    QSpacerItem *verticalSpacer;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QWidget *Form_HashValidationFailed)
    {
        if (Form_HashValidationFailed->objectName().isEmpty())
            Form_HashValidationFailed->setObjectName("Form_HashValidationFailed");
        Form_HashValidationFailed->resize(400, 300);
        gridLayout = new QGridLayout(Form_HashValidationFailed);
        gridLayout->setObjectName("gridLayout");
        label_3 = new QLabel(Form_HashValidationFailed);
        label_3->setObjectName("label_3");
        label_3->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_3, 3, 0, 1, 1);

        label_2 = new QLabel(Form_HashValidationFailed);
        label_2->setObjectName("label_2");
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label_2, 2, 0, 1, 1);

        label = new QLabel(Form_HashValidationFailed);
        label->setObjectName("label");
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);

        gridLayout->addWidget(label, 1, 0, 1, 1);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer, 4, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Expanding);

        gridLayout->addItem(verticalSpacer_2, 0, 0, 1, 1);


        retranslateUi(Form_HashValidationFailed);

        QMetaObject::connectSlotsByName(Form_HashValidationFailed);
    } // setupUi

    void retranslateUi(QWidget *Form_HashValidationFailed)
    {
        Form_HashValidationFailed->setWindowTitle(QCoreApplication::translate("Form_HashValidationFailed", "Form", nullptr));
        label_3->setText(QCoreApplication::translate("Form_HashValidationFailed", "\320\222\320\265\321\200\320\276\321\217\321\202\320\275\320\276, \321\215\320\272\320\267\320\265\320\274\320\277\320\273\321\217\321\200 \320\261\321\213\320\273 \320\270\320\267\320\274\320\265\320\275\320\265\320\275!", nullptr));
        label_2->setText(QCoreApplication::translate("Form_HashValidationFailed", " \320\237\320\265\321\200\320\265\321\203\321\201\321\202\320\260\320\275\320\276\320\262\320\270\321\202\320\265 \320\277\321\200\320\276\320\263\321\200\320\260\320\274\320\274\321\203", nullptr));
        label->setText(QCoreApplication::translate("Form_HashValidationFailed", "\320\241\320\262\320\265\321\200\320\272\320\260 \321\205\321\215\321\210-\320\272\320\276\320\264\320\260 \320\277\321\200\320\276\320\262\320\260\320\273\320\270\320\273\320\260\321\201\321\214!", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Form_HashValidationFailed: public Ui_Form_HashValidationFailed {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_HASHVALIDATIONFAILED_H
