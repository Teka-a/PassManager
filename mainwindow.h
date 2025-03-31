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
└─$ sqlite3 info.db .dump > dump.sql
└─$ openssl enc -aes-256-cbc -in dump.sql -out info.enc -K b340947333ac4783f8d88683acc45f0165834e14160df964ffa5d3aee6b9c8dc -iv 65834e14160df964ffa5d3aee6b9c8dc

└─$ echo -n "username1" | openssl enc -aes-256-cbc -K 966d3ce7f52d62a493b9d800aa90f25cfd923dce7d4845a674b6d6361c130a42 -iv fd923dce7d4845a674b6d6361c130a42 | hexdump -v  -e '/1 "%02x"'
└─$ echo -n "password1" | openssl enc -aes-256-cbc -K 9f6cbe2962e098807ab6fa09028c8fd4e9f4b88ca49c339bc0a226faafca0562 -iv e9f4b88ca49c339bc0a226faafca0562  | hexdump -v  -e '/1 "%02x"'
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

    void filterByHostname();


private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    bool isChanged = true;

    QByteArray keyPass;
    QByteArray ivPass;

    QByteArray keyLogin;
    QByteArray ivLogin;

    QByteArray keyDatabase;
    QByteArray ivDatabase;

    //QString dbName = "../../info.db";
    QString encDbName = "../../info.enc";
    //QString tempFile = "../../temp.db";

    QStringList readSQLStatements(const QByteArray &data);
    bool restoreDatabaseInMemory(const QByteArray &decryptedData);


    bool decryptAndLoadDatabase(const QByteArray &key, const QByteArray &iv);

    void checkPin();

    void setCredentials(QVector<Credentials> &creds);
    QVector<Credentials> readCredentials(const QString &hostname = NULL);

    QString encryptedPin;

    void wipe(QByteArray& mem);

    bool applyChanges();

};


#endif // MAINWINDOW_H
