#ifndef FORM_HASHVALIDATIONFAILED_H
#define FORM_HASHVALIDATIONFAILED_H

#include <QWidget>

namespace Ui {
class Form_HashValidationFailed;
}

class Form_HashValidationFailed : public QWidget
{
    Q_OBJECT

public:
    explicit Form_HashValidationFailed(QWidget *parent = nullptr);
    ~Form_HashValidationFailed();

private:
    Ui::Form_HashValidationFailed *ui;
};

#endif // FORM_HASHVALIDATIONFAILED_H
