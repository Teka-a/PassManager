#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QClipboard>



#include "credentials.h"
//#include "sqlite/sqlite3.h"

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
    void copyClipboard();

    void addCreds();
    void delCreds();

    void openAddForm();

    void changeViewMode();

    void openHomePage();


private:
    Ui::MainWindow *ui;

    QString dbName = "../../info.db";

    void checkPin();

    void setAllCredentials();
    QVector<Credentials> readCredentials();

    QString encryptedPin;


};
#endif // MAINWINDOW_H
