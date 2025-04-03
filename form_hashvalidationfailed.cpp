#include "form_hashvalidationfailed.h"
#include "ui_form_hashvalidationfailed.h"

Form_HashValidationFailed::Form_HashValidationFailed(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Form_HashValidationFailed)
{
    ui->setupUi(this);
}

Form_HashValidationFailed::~Form_HashValidationFailed()
{
    delete ui;
}
