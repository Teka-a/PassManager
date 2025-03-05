#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setMinimumSize(488, 364);
    this->setMaximumSize(488, 364);

    ui->stackedWidget->setCurrentWidget(ui->page);

    ui->pin->setFocus();

    ui->wrongPinInfo->setVisible(false);
    ui->noCreds->setVisible(false);
    ui->enterBtn->setStyleSheet("QPushButton:hover { border-style: outset; border-width: 1px; color: #413FA7; "
                                "border-radius: 10px; border-color: #413FA7; background-color: #0B0317; }");

    ui->insertBtn->setStyleSheet("QPushButton:hover { border-style: outset; border-width: 1px; color: #413FA7; "
                                "border-radius: 10px; border-color: #413FA7; background-color: #0B0317; }");

    setWindowIcon(QIcon(":/rc/icons/icon.ico"));
    setWindowTitle("Password Manager");

    connect(ui->enterBtn, &QPushButton::clicked, this, &MainWindow::checkPin);
    connect(ui->pin, &QLineEdit::returnPressed, this, &MainWindow::checkPin);

    connect(ui->addBtn, &QPushButton::clicked, this, &MainWindow::openAddForm);
    //0 - is hidden
    //1 - is shown
    ui->echoLogin->setProperty("mode", 0);
    ui->echoPass->setProperty("mode", 0);
    //0 - login
    //1 - password
    ui->echoLogin->setProperty("item", 0);
    ui->echoPass->setProperty("item", 1);
    ui->echoLogin->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");
    ui->echoPass->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");

    connect(ui->echoLogin, &QPushButton::clicked, this, &MainWindow::changeViewMode);
    connect(ui->echoPass, &QPushButton::clicked, this, &MainWindow::changeViewMode);

    connect(ui->insertBtn, &QPushButton::clicked, this, &MainWindow::addCreds);
    connect(ui->updBtn, &QPushButton::clicked, this, &MainWindow::setAllCredentials);

    //ui->retHome->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");
    connect(ui->retHome, &QPushButton::clicked, this, &MainWindow::openHomePage);

}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::generateAESKeyAndIV(const QByteArray &pin, QByteArray &key, QByteArray &iv) {
    QByteArray salt = dbName.toUtf8();
    QByteArray hash = QCryptographicHash::hash(pin + salt, QCryptographicHash::Sha256);

    key = hash.left(32);
    iv = hash.right(16);

    keyPass = hash.mid(32, 32);
    keyPass = hash.mid(64, 16);
}


void MainWindow::copyClipboard()
{
    qDebug() << "here";

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText("text");
}




//setupUI
void MainWindow::setAllCredentials()
{
    QVector<Credentials> credsInfo = readCredentials();

    if (credsInfo.size() == 0) {
        ui->noCreds->setVisible(true);
    }

    ui->scrollArea->setStyleSheet(R"(QScrollBar:vertical { border: none; background: #0B0317; width: 10px; margin: 2px 0 2px 0; }
                                     QScrollBar::handle:vertical { background: #37348F; min-height: 20px; border-radius: 5px; }
                                     QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { background: none; }
                                     QScrollBar::handle:vertical:hover {background: #5A56D0; }
                                     QScrollBar::handle:vertical:pressed { background: #5A56D0; })");

    QWidget *window = new QWidget;
    QVBoxLayout *layoutMain = new QVBoxLayout(window);

    for (int i = 0; i < credsInfo.size(); ++i) {
        QWidget *item = new QWidget;

        item->setProperty("id", QString::number(credsInfo[i].getId()));

        item->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        item->setMinimumSize(30, 50);
        item->setMaximumSize(800, 70);
        item->setObjectName("element");

        item->setStyleSheet("#element { border: 1px solid #37348F; border-radius: 5px; }"
                            "#element:hover {border: 2px solid #5A56D0; border-radius: 5px;}");

        QHBoxLayout *layout = new QHBoxLayout(item);

        QString id = QString::number(credsInfo[i].getId());
        qDebug() << id;

        QLabel *site = new QLabel(credsInfo[i].getSite());
        QLabel *login = new QLabel("********");
        QLabel *password = new QLabel("********");
        //QLabel *password = new QLabel(credsInfo[i].getPassword());

        QPushButton *copySite = new QPushButton();
        copySite->setMaximumSize(30, 30);
        QPixmap pixCopySite(":/rc/icons/website.png");
        QIcon iconBtn(pixCopySite);
        copySite->setIcon(iconBtn);

        QPushButton *copyLogin = new QPushButton();
        QPixmap pixCopy2(":/rc/icons/copy.png");
        QIcon iconBtnCopy2(pixCopy2);
        copyLogin->setMaximumSize(30, 30);
        copyLogin->setIcon(iconBtnCopy2);

        QPushButton *copyPassword = new QPushButton();
        copyPassword->setIcon(iconBtnCopy2);
        copyPassword->setMaximumSize(30, 30);

        connect(copySite, &QPushButton::clicked, this, &MainWindow::copyClipboard);
        connect(copyLogin, &QPushButton::clicked, this, &MainWindow::copyClipboard);
        connect(copyPassword, &QPushButton::clicked, this, &MainWindow::copyClipboard);

        QPushButton *deleteCredentials = new QPushButton();
        QPixmap pixDelete(":/rc/icons/delete.png");
        QIcon delBtn(pixDelete);
        deleteCredentials->setIcon(delBtn);
        deleteCredentials->setMaximumSize(30, 30);

        connect(deleteCredentials, &QPushButton::clicked, this, &MainWindow::delCreds);
        //deleteCredentials->setProperty("id", credsInfo[i].getId());

        QPushButton *changeCredentials = new QPushButton();
        QPixmap pixChange(":/rc/icons/change.png");
        QIcon changeBtn(pixChange);
        changeCredentials->setIcon(changeBtn);
        changeCredentials->setMaximumSize(30, 30);

        //changeCredentials->setProperty("id", credsInfo[i].getId());

        layout->addWidget(copySite);
        layout->addWidget(site);

        layout->addWidget(copyLogin);
        layout->addWidget(login);

        layout->addWidget(copyPassword);
        layout->addWidget(password);

        layout->addWidget(changeCredentials);
        layout->addWidget(deleteCredentials);


        layoutMain->addWidget(item);
    }

    layoutMain->addItem(new QSpacerItem(10, 10, QSizePolicy::Expanding, QSizePolicy::Maximum));
    qDebug() << window;
    ui->scrollArea->setWidget(window);

    //MainWindow::show();
}


void MainWindow::openAddForm()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    ui->siteLine->setFocus();
}


void MainWindow::openHomePage()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


void MainWindow::changeViewMode()
{
    QPushButton *clicked = qobject_cast<QPushButton*>(sender());

    int mode = clicked->property("mode").toInt();

    if (mode == 0) { //данные скрыты
        qDebug() << "here";
        if (clicked->property("item").toInt() == 0) {
            ui->loginLine->setEchoMode(QLineEdit::Normal);
        } else {
            ui->passwordLine->setEchoMode(QLineEdit::Normal);
        }

        QPixmap pixHide(":/rc/icons/hide.png");
        QIcon hideIcon(pixHide);
        clicked->setIcon(hideIcon);

        clicked->setProperty("mode", 1);
    } else { //данные видны
        if (clicked->property("item").toInt() == 0) {
            ui->loginLine->setEchoMode(QLineEdit::Password);
        } else {
            ui->passwordLine->setEchoMode(QLineEdit::Password);
        }

        QPixmap pixShow (":/rc/icons/show.png");
        QIcon showIcon(pixShow);
        clicked->setIcon(showIcon);

        clicked->setProperty("mode", 0);
    }

}



//Work with db
QVector<Credentials> MainWindow::readCredentials()
{
    QVector<Credentials> credentialsInfo;

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);
    query.prepare("SELECT * FROM creds");
    qDebug() << query.isValid();
    query.exec();


    int id;
    QString site;
    QString login;
    QString password;

    Credentials creds;

    QSqlRecord rec = query.record();
    int idIndex = rec.indexOf("id");
    int siteIndex = rec.indexOf("hostname");
    int loginIndex = rec.indexOf("login");
    int passIndex = rec.indexOf("pass");



    while (query.next()) {
        id = query.value(idIndex).toInt();
        site = query.value(siteIndex).toString();
        login = query.value(loginIndex).toString();
        password = query.value(passIndex).toString();
        qDebug() << login;


        creds.setId(id);
        creds.setSite(site);
        creds.setLogin(login);
        creds.setPassword(password);

        credentialsInfo.push_back(creds);
    }

    return credentialsInfo;
}

void MainWindow::addCreds()
{
    qDebug() << "add new";

    QByteArray hostname = ui->siteLine->text().toUtf8();
    QByteArray login = ui->loginLine->text().toUtf8();
    QByteArray password = ui->passwordLine->text().toUtf8();

    qDebug() << hostname;
    qDebug() << login;
    qDebug() << password;

    ui->siteLine->setText("");
    ui->loginLine->setText("");
    ui->passwordLine->setText("");

    QSqlDatabase db = QSqlDatabase::database();

    QSqlQuery query(db);
    query.prepare("INSERT INTO creds (hostname, login, password) "
                  "VALUES (:hostname, :login, :password)");
    query.bindValue(":hostname", QString(hostname));
    query.bindValue(":login",  QString(login));
    query.bindValue(":password",  QString(password));


    query.exec();

    db.commit();

    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


void MainWindow::delCreds()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QWidget *parentWidget = qobject_cast<QWidget*>(button->parent());
        if (parentWidget) {
            QString value = parentWidget->property("id").toString();

        }
    }


}


void MainWindow::checkPin()
{
    QString pin = ui->pin->text();

    QByteArray pinArr = pin.toUtf8();
    QByteArray keyDatabase;
    QByteArray ivDatabase;

    generateAESKeyAndIV(pinArr, keyDatabase, ivDatabase);

    if (pin == "12345") {
        //encrypt pin
        this->encryptedPin = pin;

        ui->stackedWidget->setCurrentWidget(ui->page_2);
        this->setMinimumSize(865, 543);
        this->setMaximumSize(865, 543);

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(this->dbName);

        this->setAllCredentials();
    } else {
        ui->pin->setText("");
        ui->wrongPinInfo->setVisible(true);
        QPixmap lockedPic (":/rc/icons/blocking.png");

        ui->locked->setPixmap(lockedPic);
    }

}


void MainWindow::wipe(QByteArray& mem)
{
    mem.fill(0);
}
