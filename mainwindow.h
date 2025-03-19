#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QDir>
#include <QFile>
#include <QBuffer>
#include <QByteArray>

#include <cstdlib>
#include <ctime>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QClipboard>

#include <QCryptographicHash>

#include <openssl/evp.h>
#include <openssl/sha.h>

/*
└─$ echo -n "your_string_to_encrypt" | openssl enc -aes-256-cbc -K e62039c06ff24236411e763e505c4198ee2deda410715f82476d82481878d78d -iv ee2deda410715f82476d82481878d78d  | hexdump -v  -e '/1 "%02x"'
└─$ openssl enc -aes-256-cbc -in info.db -out info.enc -K b340947333ac4783f8d88683acc45f0165834e14160df964ffa5d3aee6b9c8dc -iv 65834e14160df964ffa5d3aee6b9c8dc
*/


#include "credentials.h"
#include "crypto.h"
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
    QSqlDatabase db;

    QByteArray keyPass;
    QByteArray ivPass;

    QByteArray keyLogin;
    QByteArray ivLogin;

    //QString dbName = "../../info.db";
    QString encDbName = "../../info.enc";
    QString tempFile = "../../temp.db";

    QStringList readSQLStatements(const QByteArray &data);
    bool restoreDatabaseInMemory(const QByteArray &decryptedData);


    bool decryptAndLoadDatabase(const QByteArray &key, const QByteArray &iv);

    void checkPin();

    void setCredentials(QVector<Credentials> &creds);
    QVector<Credentials> readCredentials(const QString &hostname = NULL);

    QString encryptedPin;

    void wipe(QByteArray& mem);

};


#endif // MAINWINDOW_H
