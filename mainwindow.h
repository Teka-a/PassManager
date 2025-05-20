#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QString>
#include <QVector>
#include <QDir>
#include <QChar>
#include <QFile>
#include <QBuffer>
#include <QByteArray>

#include <cstdlib>
#include <ctime>

#include <QTimer>
#include <QProcess>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QClipboard>

#include <QRandomGenerator>

#include <QCryptographicHash>

#include <openssl/evp.h>
#include <openssl/sha.h>

/*
└─$ echo -n "your_string_to_encrypt" | openssl enc -aes-256-cbc -K e62039c06ff24236411e763e505c4198ee2deda410715f82476d82481878d78d -iv ee2deda410715f82476d82481878d78d  | hexdump -v  -e '/1 "%02x"'
└─$ openssl enc -aes-256-cbc -in info.db -out info.enc -K b340947333ac4783f8d88683acc45f0165834e14160df964ffa5d3aee6b9c8dc -iv 65834e14160df964ffa5d3aee6b9c8dc
└─$ sqlite3 info.db .dump > dump.sql
└─$ openssl enc -aes-256-cbc -in dump.sql -out info.enc -K b340947333ac4783f8d88683acc45f0165834e14160df964ffa5d3aee6b9c8dc -iv 65834e14160df964ffa5d3aee6b9c8dc

└─$ openssl enc -aes-256-cbc -in dump.sql -out info.enc -K 51ed88b8522f957fd592555e779c44528444e378f2383671480a368245745041 -iv 8444e378f2383671480a368245745041



└─$ echo -n "username1" | openssl enc -aes-256-cbc -K 966d3ce7f52d62a493b9d800aa90f25cfd923dce7d4845a674b6d6361c130a42 -iv fd923dce7d4845a674b6d6361c130a42 | hexdump -v  -e '/1 "%02x"'
└─$ echo -n "password1" | openssl enc -aes-256-cbc -K 9f6cbe2962e098807ab6fa09028c8fd4e9f4b88ca49c339bc0a226faafca0562 -iv e9f4b88ca49c339bc0a226faafca0562  | hexdump -v  -e '/1 "%02x"'
*/


#include "credentials.h"


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
signals:
    void successPin();
private slots:
    void copyClipboard();

    void copySite();
    void copyCreds();

    void addCreds();
    void delCreds();
    void updCreds();

    void openLoginForm();
    void openAddForm();
    void openUpdForm();
    void openHomePage();

    void loginForAdd();
    void loginForUpd();
    void loginForDel();
    void loginForCopy();

    void changeViewMode();

    void filterByHostname();

    //void checkPin();

    void processPin();

    void checkScreenshooter();
    void hideOverlay();

    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::MainWindow *ui;
    QSqlDatabase db;

    QWidget *overlayWidget;

    QTimer *checkTimer;
    QTimer *overlayHideTimer;

    bool isChanged = false;

    int idToUpd = -1;
    int idToDel = -1;
    int idToCopy = -1;
    QString dataToCopy = "";


    QString encDbName = "./info.enc";
    QString passwordSalt = "093ls902hsdfd";
    QString loginSalt = "dfdbt89tr.";
    //QString databaseSalt = "../../info.db";
    QString databaseSalt = "390smgochs!ltuv";

    const QString chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                          "abcdefghijklmnopqrstuvwxyz"
                          "0123456789"
                          ".!/+=_*&@#$%^{},[]-()";



    QStringList readSQLStatements(const QByteArray &data);
    bool restoreDatabaseInMemory(const QByteArray &decryptedData);

    bool decryptAndLoadDatabase(const QByteArray &key, const QByteArray &iv);

    void setCredentials(QVector<Credentials> &creds);
    QVector<Credentials> readCredentials(const QString &hostname = NULL);

    void wipe(QByteArray& mem);
    void wipe(QString& mem);

    bool applyChanges(QByteArray &key, QByteArray &iv);

    void getSalt(QString id, QString &salt);
    QString getEncLogin(QString id);
    QString getEncPassword(QString id);

    bool isPinCorrect();

    void generateSalt(QString &salt);

    void clearDataLoginForm();
    void clearDataAddForm();
    void clearDataUpdForm();
    void clearDataHomePage();

};


#endif // MAINWINDOW_H
