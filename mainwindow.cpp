#include "mainwindow.h"
#include "crypto.h"
#include <QtConcurrent>
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    openLoginForm();

    ui->enterBtn->setStyleSheet("QPushButton:hover { border-style: outset; border-width: 1px; color: #413FA7; "
                                "border-radius: 10px; border-color: #413FA7; background-color: #0B0317; }");

    ui->insertBtn->setStyleSheet("QPushButton:hover { border-style: outset; border-width: 1px; color: #413FA7; "
                                "border-radius: 10px; border-color: #413FA7; background-color: #0B0317; }");

    ui->updBtn->setStyleSheet("QPushButton:hover { border-style: outset; border-width: 1px; color: #413FA7; "
                                 "border-radius: 10px; border-color: #413FA7; background-color: #0B0317; }");
    ui->info->setStyleSheet("QLabel { color: #413FA7; font: 14px; }");
    ui->label_2->setStyleSheet("QLabel { font: 15px; }");

    setWindowIcon(QIcon(":/rc/icons/icon.ico"));
    setWindowTitle("Password Manager");


    connect(ui->addBtn, &QPushButton::clicked, this, &MainWindow::openAddForm);
    //0 - is hidden
    //1 - is shown
    ui->echoLogin->setProperty("mode", 0);
    ui->echoPass->setProperty("mode", 0);
    ui->echoLogin_2->setProperty("mode", 0);
    ui->echoPass_2->setProperty("mode", 0);
    //0 - login
    //1 - password
    ui->echoLogin->setProperty("item", 0);
    ui->echoPass->setProperty("item", 1);
    ui->echoLogin_2->setProperty("item", 0);
    ui->echoPass_2->setProperty("item", 1);
    //1 - add
    //2 - change
    ui->echoLogin->setProperty("func", 1);
    ui->echoPass->setProperty("func", 1);
    ui->echoLogin_2->setProperty("func", 2);
    ui->echoPass_2->setProperty("func", 2);

    //0 - first enter to app
    //1 - add
    //2 - delete
    //3 - upd
    ui->enterBtn->setProperty("stage", 0);
    connect(ui->pin, &QLineEdit::returnPressed, this, &MainWindow::processPin);
    connect(ui->enterBtn, &QPushButton::clicked, this, &MainWindow::processPin);

    ui->echoLogin->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");
    ui->echoPass->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");

    connect(ui->echoLogin, &QPushButton::clicked, this, &MainWindow::changeViewMode);
    connect(ui->echoPass, &QPushButton::clicked, this, &MainWindow::changeViewMode);
    connect(ui->echoLogin_2, &QPushButton::clicked, this, &MainWindow::changeViewMode);
    connect(ui->echoPass_2, &QPushButton::clicked, this, &MainWindow::changeViewMode);

    //connect(ui->insertBtn, &QPushButton::clicked, this, &MainWindow::addCreds);
    connect(ui->insertBtn, &QPushButton::clicked, this, &MainWindow::loginForAdd);
    connect(ui->updBtn, &QPushButton::clicked, this, &MainWindow::loginForUpd);

    //ui->retHome->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");
    connect(ui->retHome, &QPushButton::clicked, this, &MainWindow::openHomePage);
    connect(ui->retHome_2, &QPushButton::clicked, this, &MainWindow::openHomePage);

    connect(ui->searchBtn, &QPushButton::clicked, this, &MainWindow::filterByHostname);

    overlayWidget = new QWidget(this);
    overlayWidget->setGeometry(0, 0, width(), height());
    overlayWidget->setStyleSheet("background-color: black;");
    overlayWidget->hide();
    overlayWidget->raise();

    overlayHideTimer = new QTimer(this);
    overlayHideTimer->setSingleShot(true);
    connect(overlayHideTimer, &QTimer::timeout, this, &MainWindow::hideOverlay);

    checkTimer = new QTimer(this);
    connect(checkTimer, &QTimer::timeout, this, &MainWindow::checkScreenshooter);
    checkTimer->start(500);
}


MainWindow::~MainWindow()
{
    delete ui;
    if (isChanged) {
        qDebug() << "Changes noticed";
    }

    this->db.close();
}


QStringList MainWindow::readSQLStatements(const QByteArray &data)
{
    QStringList sqlStatements;
    QByteArray currentStatement;

    QList<QByteArray> lines = data.split('\n');
    for (const QByteArray &line : lines) {
        QByteArray trimmedLine = line.trimmed();

        if (trimmedLine.isEmpty()) {
            continue;
        }

        currentStatement += trimmedLine + " ";

        if (trimmedLine.endsWith(";")) {
            sqlStatements.append(currentStatement.trimmed());
            currentStatement.clear();
        }
    }

    return sqlStatements;
}


bool MainWindow::restoreDatabaseInMemory(const QByteArray &decryptedData)
{
    db = QSqlDatabase::addDatabase("QSQLITE", "memory_connection");
    db.setDatabaseName(":memory:");

    if (!db.open()) {
        qDebug() << "Failed to open db in memory!";
        return false;
    }

    QStringList sqlCommands = readSQLStatements(decryptedData);

    for (const QString &sql : sqlCommands) {
        QSqlQuery query(db);
        if (!query.exec(sql)) {
            qDebug() << "Error in SQL:" << sql << "→" << query.lastError().text();
            return false;
        }
    }

    return true;
}


bool MainWindow::isPinCorrect()
{
    QByteArray keyDb;
    QByteArray ivDb;
    computeAESKeyAndIV(ui->pin->text().toUtf8(), databaseSalt.toUtf8(), keyDb, ivDb);

    QByteArray decDBFile;
    QFile file(encDbName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Error open file:" << file.errorString();
    }
    QByteArray encDBFile = file.readAll();

    if (!decryptAES256CBC(encDBFile, keyDb, ivDb, decDBFile)) {
        qDebug() << "Error decrypting!";
    }

    //qDebug() << "Start: " << QString::fromUtf8(decDBFile.left(100));

    if (QString::fromUtf8(decDBFile.left(100)).contains("creds", Qt::CaseInsensitive)) {
        return true;
    } else {
        return false;
    }
}


void MainWindow::processPin()
{
    if (isPinCorrect()) {
        if (ui->enterBtn->property("stage") == 0) {
            qDebug() << "First login";
            ui->enterBtn->setProperty("stage", 1);

            QByteArray keyDb;
            QByteArray ivDb;
            QByteArray pin = ui->pin->text().toUtf8();
            computeAESKeyAndIV(pin, databaseSalt.toUtf8(), keyDb, ivDb);

            QByteArray decDBFile;
            QFile file(encDbName);
            if (!file.open(QIODevice::ReadOnly)) {
                qDebug() << "Error open file:" << file.errorString();
            }
            QByteArray encDBFile = file.readAll();
            decryptAES256CBC(encDBFile, keyDb, ivDb, decDBFile);

            if (restoreDatabaseInMemory(decDBFile)) {
                qDebug() << "DB successfully restored";
                openHomePage();
            } else {
                qDebug() << "DB wasn't restored";
            }
        } else if (ui->enterBtn->property("stage") == 1) {
            qDebug() << "add req";

            emit successPin();
        }
    } else {
        clearDataLoginForm();
        clearDataHomePage();
        clearDataAddForm();
        clearDataUpdForm();

        ui->wrongPinInfo->setVisible(true);
        QPixmap lockedPic (":/rc/icons/blocking.png");

        ui->locked->setPixmap(lockedPic);
    }
}

void MainWindow::copySite()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString textToCopy = button->property("data").toString();

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(textToCopy);
}

void MainWindow::loginForCopy()
{
    qDebug() << "Here loginForCopy";
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        dataToCopy = button->property("data").toString();
    }

    qDebug() << dataToCopy;

    QWidget *parentWidget = qobject_cast<QWidget*>(button->parent());
    if (parentWidget) {
        idToCopy = parentWidget->property("id").toInt();
    }

    qDebug() << idToCopy;

    openLoginForm();
    connect(this, &MainWindow::successPin, this, &MainWindow::copyCreds);
}



void MainWindow::copyCreds()
{
    qDebug() << "Here copyCreds";
    qDebug() << dataToCopy;
    qDebug() << idToCopy;

    disconnect(this, &MainWindow::successPin, this, nullptr);
    QString textToCopy = "";

    if (idToCopy != -1 && dataToCopy != "") {
        qDebug() << "inside";
        QString salt = "";
        getSalt(QString::number(idToCopy), salt);

        QByteArray pinCode = ui->pin->text().toUtf8();

        QByteArray key;
        QByteArray iv;
        computeAESKeyAndIV(pinCode, salt.toUtf8(), key, iv);

        QByteArray ciphertext = QByteArray::fromHex(dataToCopy.toUtf8());
        QByteArray plaintext;

        if (!decryptAES256CBC(ciphertext, key, iv, plaintext)) {
            qDebug() << "Decryption failed";
        }

        textToCopy = QString::fromUtf8(plaintext);

        wipe(key);
        wipe(iv);
        wipe(ciphertext);

    }

    idToCopy = -1;
    wipe(dataToCopy);

    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(textToCopy);

    openHomePage();
}


void MainWindow::copyClipboard()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    QString textToCopy = "";

    QString pin = "";

    QByteArray plaintext;
    QByteArray ciphertext;
    if (button->property("type") == "s") {
        textToCopy = button->property("data").toString();
    } else if (button->property("type") == "l") {
        QString data = button->property("data").toString();
        ciphertext = QByteArray::fromHex(data.toUtf8());

        QByteArray key;
        QByteArray iv;
        computeAESKeyAndIV(pin.toUtf8(), loginSalt.toUtf8(), key, iv);

        qDebug() << "Key login: " << key.toHex();
        qDebug() << "Iv login: " << iv.toHex();

        if (!decryptAES256CBC(ciphertext, key, iv, plaintext)) {
            qDebug() << "Decryption failed";
        }

        textToCopy = QString::fromUtf8(plaintext);
    } else if (button->property("type") == "p") {
        QString data = button->property("data").toString();
        ciphertext = QByteArray::fromHex(data.toUtf8());

        QByteArray key;
        QByteArray iv;
        computeAESKeyAndIV(pin.toUtf8(), passwordSalt.toUtf8(), key, iv);

        qDebug() << "Key password: " << key.toHex();
        qDebug() << "Iv password: " << iv.toHex();


        if (!decryptAES256CBC(ciphertext, key, iv, plaintext)) {
            qDebug() << "Decryption failed";
        }

        textToCopy = QString::fromUtf8(plaintext);
    }
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(textToCopy);
}



//setupUI
void MainWindow::setCredentials(QVector<Credentials> &credsInfo)
{
    if (credsInfo.size() == 0) {
        ui->noCreds->setVisible(true);
    } else {
        ui->noCreds->setVisible(false);
    }

    ui->scrollArea->setStyleSheet(R"(QScrollBar:vertical { border: none; background: #0B0317; width: 5px; margin: 2px 0 2px 0; }
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

        QLabel *site = new QLabel(credsInfo[i].getSite());
        QLabel *login = new QLabel("********");
        QLabel *password = new QLabel("********");
        site->setFixedSize(200, 30);
        login->setFixedSize(50, 30);
        password->setFixedSize(50, 30);
        //QLabel *password = new QLabel(credsInfo[i].getPassword());

        QPushButton *copySite = new QPushButton();
        copySite->setMaximumSize(30, 30);
        QPixmap pixCopySite(":/rc/icons/website.png");
        QIcon iconBtn(pixCopySite);
        copySite->setIcon(iconBtn);
        copySite->setProperty("data", credsInfo[i].getSite());
        copySite->setProperty("type", "s");

        QPushButton *copyLogin = new QPushButton();
        QPixmap pixCopy2(":/rc/icons/copy.png");
        QIcon iconBtnCopy2(pixCopy2);
        copyLogin->setMaximumSize(30, 30);
        copyLogin->setIcon(iconBtnCopy2);
        copyLogin->setProperty("data", credsInfo[i].getLogin());
        copyLogin->setProperty("type", "l");

        QPushButton *copyPassword = new QPushButton();
        copyPassword->setIcon(iconBtnCopy2);
        copyPassword->setMaximumSize(30, 30);
        copyPassword->setProperty("data", credsInfo[i].getPassword());
        copyPassword->setProperty("type", "p");

        connect(copySite, &QPushButton::clicked, this, &MainWindow::copySite);
        connect(copyLogin, &QPushButton::clicked, this, &MainWindow::loginForCopy);
        connect(copyPassword, &QPushButton::clicked, this, &MainWindow::loginForCopy);

        QPushButton *deleteCredentials = new QPushButton();
        QPixmap pixDelete(":/rc/icons/delete.png");
        QIcon delBtn(pixDelete);
        deleteCredentials->setIcon(delBtn);
        deleteCredentials->setMaximumSize(30, 30);
        connect(deleteCredentials, &QPushButton::clicked, this, &MainWindow::loginForDel);

        QPushButton *changeCredentials = new QPushButton();
        QPixmap pixChange(":/rc/icons/change.png");
        QIcon changeBtn(pixChange);
        changeCredentials->setIcon(changeBtn);
        changeCredentials->setMaximumSize(30, 30);
        connect(changeCredentials, &QPushButton::clicked, this, &MainWindow::openUpdForm);



        layout->addItem(new QSpacerItem(10, 30, QSizePolicy::Maximum, QSizePolicy::Maximum));

        layout->addWidget(copySite);
        layout->addWidget(site);

        layout->addItem(new QSpacerItem(50, 30, QSizePolicy::Expanding, QSizePolicy::Maximum));

        layout->addWidget(login);
        layout->addWidget(copyLogin);

        layout->addItem(new QSpacerItem(50, 30, QSizePolicy::Expanding, QSizePolicy::Maximum));

        layout->addWidget(password);
        layout->addWidget(copyPassword);

        layout->addItem(new QSpacerItem(50, 30, QSizePolicy::Expanding, QSizePolicy::Maximum));

        layout->addWidget(changeCredentials);
        layout->addWidget(deleteCredentials);

        layout->addItem(new QSpacerItem(10, 30, QSizePolicy::Maximum, QSizePolicy::Maximum));

        layoutMain->addWidget(item);
    }

    layoutMain->addItem(new QSpacerItem(400, 400, QSizePolicy::Expanding, QSizePolicy::Maximum));

    ui->scrollArea->setWidget(window);

}


void MainWindow::openLoginForm()
{
    this->setMinimumSize(488, 364);
    this->setMaximumSize(488, 364);

    clearDataLoginForm();

    ui->pin->setFocus();

    ui->stackedWidget->setCurrentWidget(ui->page);
}


void MainWindow::openAddForm()
{
    this->setMinimumSize(865, 543);
    this->setMaximumSize(865, 543);
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    ui->siteLine->setFocus();
}


void MainWindow::openUpdForm()
{
    this->setMinimumSize(865, 543);
    this->setMaximumSize(865, 543);
    ui->stackedWidget->setCurrentWidget(ui->page_4);
    ui->siteLine->setFocus();
    QString hostname = "";
    QString id = "";

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QWidget *parentWidget = qobject_cast<QWidget*>(button->parent());
        if (parentWidget) {
            id = parentWidget->property("id").toString();
            idToUpd = parentWidget->property("id").toInt();
            QSqlQuery query(this->db);
            query.prepare("SELECT * FROM creds WHERE id = :id");
            query.bindValue(":id", id);
            query.exec();

            QSqlRecord rec = query.record();

            int siteIndex = rec.indexOf("hostname");

            while (query.next()) {
                hostname = query.value(siteIndex).toString();
            }

        }
    }

    ui->info->setText(hostname);

    ui->updBtn->setProperty("id", id);
}


void MainWindow::openHomePage()
{
    clearDataHomePage();

    QVector<Credentials> credsUpd = readCredentials();
    setCredentials(credsUpd);

    this->setMinimumSize(865, 543);
    this->setMaximumSize(865, 543);
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


void MainWindow::filterByHostname()
{
    QString hostnamePart = "%" + ui->searchline->text() + "%";
    QVector<Credentials> creds = readCredentials(hostnamePart);
    setCredentials(creds);
}


void MainWindow::changeViewMode()
{
    QPushButton *clicked = qobject_cast<QPushButton*>(sender());

    int mode = clicked->property("mode").toInt();

    if (mode == 0) { //данные скрыты
        if (clicked->property("func").toInt() == 1) {
            if (clicked->property("item").toInt() == 0) {
                ui->loginLine->setEchoMode(QLineEdit::Normal);
            } else {
                ui->passwordLine->setEchoMode(QLineEdit::Normal);
            }
        } else if (clicked->property("func").toInt() == 2) {
            if (clicked->property("item").toInt() == 0) {
                ui->loginChange->setEchoMode(QLineEdit::Normal);
            } else {
                ui->passwordChange->setEchoMode(QLineEdit::Normal);
            }
        }


        QPixmap pixHide(":/rc/icons/hide.png");
        QIcon hideIcon(pixHide);
        clicked->setIcon(hideIcon);

        clicked->setProperty("mode", 1);
    } else { //данные видны
        if (clicked->property("func").toInt() == 1) {
            if (clicked->property("item").toInt() == 0) {
                ui->loginLine->setEchoMode(QLineEdit::Password);
            } else {
                ui->passwordLine->setEchoMode(QLineEdit::Password);
            }
        } else if (clicked->property("func").toInt() == 2) {
            if (clicked->property("item").toInt() == 0) {
                ui->loginChange->setEchoMode(QLineEdit::Password);
            } else {
                ui->passwordChange->setEchoMode(QLineEdit::Password);
            }
        }

        QPixmap pixShow (":/rc/icons/show.png");
        QIcon showIcon(pixShow);
        clicked->setIcon(showIcon);

        clicked->setProperty("mode", 0);
    }

}


//Work with db
QVector<Credentials> MainWindow::readCredentials(const QString &hostname)
{
    QVector<Credentials> credentialsInfo;

    QSqlQuery query(this->db);
    query.prepare("SELECT * FROM creds WHERE hostname LIKE :hostname OR :hostname IS NULL");
    query.bindValue(":hostname", hostname);

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
    int passIndex = rec.indexOf("password");

    while (query.next()) {
        id = query.value(idIndex).toInt();
        site = query.value(siteIndex).toString();
        login = query.value(loginIndex).toString();
        password = query.value(passIndex).toString();

        creds.setId(id);
        creds.setSite(site);
        creds.setLogin(login);
        creds.setPassword(password);

        credentialsInfo.push_back(creds);
    }

    return credentialsInfo;
}


void MainWindow::generateSalt(QString &salt)
{
    int saltLen = QRandomGenerator::global()->bounded(5, 25);
    salt = "";

    salt.reserve(saltLen);
    for (int i = 0; i < saltLen; ++i) {
        int index = QRandomGenerator::global()->bounded(chars.length());
        salt.append(chars.at(index));
    }
}


void MainWindow::loginForAdd()
{
    openLoginForm();
    connect(this, &MainWindow::successPin, this, &MainWindow::addCreds);
}


void MainWindow::addCreds()
{
    disconnect(this, &MainWindow::successPin, this, nullptr);

    QByteArray pinCode = ui->pin->text().toUtf8();

    QByteArray key;
    QByteArray iv;
    QString saltStr;
    generateSalt(saltStr);
    QByteArray salt = saltStr.toUtf8();
    computeAESKeyAndIV(pinCode, salt, key, iv);

    QByteArray keyDb;
    QByteArray ivDb;
    computeAESKeyAndIV(pinCode, databaseSalt.toUtf8(), keyDb, ivDb);

    QString hostname = ui->siteLine->text();

    QString login = ui->loginLine->text();
    QByteArray loginArr = login.toUtf8();
    QByteArray encLoginArr;
    if (!encryptAES256CBC(loginArr, key, iv, encLoginArr)) {
        qDebug() << "Encryption failed!";
    }
    QString encLogin = encLoginArr.toHex();

    QString password = ui->passwordLine->text();
    QByteArray passwordArr = password.toUtf8();
    QByteArray encPasswordArr;
    if (!encryptAES256CBC(passwordArr, key, iv, encPasswordArr)) {
        qDebug() << "Encryption failed!";
    }
    QString encPassword = encPasswordArr.toHex();

    QSqlQuery query(this->db);
    query.prepare("INSERT INTO creds (hostname, login, password, salt) "
                  "VALUES (:hostname, :login, :password, :salt)");
    query.bindValue(":hostname", hostname);
    query.bindValue(":login",  encLogin);
    query.bindValue(":password",  encPassword);
    query.bindValue(":salt",  saltStr);

    query.exec();

    this->isChanged = true;
    applyChanges(keyDb, ivDb);
    this->isChanged = false;

    wipe(key);
    wipe(iv);
    wipe(keyDb);
    wipe(ivDb);
    wipe(salt);
    wipe(saltStr);
    wipe(pinCode);

    clearDataAddForm();
    clearDataLoginForm();

    openHomePage();
}


void MainWindow::loginForDel()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QWidget *parentWidget = qobject_cast<QWidget*>(button->parent());
        if (parentWidget) {
            idToDel = parentWidget->property("id").toInt();
        }
    }

    openLoginForm();
    connect(this, &MainWindow::successPin, this, &MainWindow::delCreds);
}


void MainWindow::delCreds()
{
    disconnect(this, &MainWindow::successPin, this, nullptr);
    if (idToUpd != -1) {
        QString id = QString::number(idToUpd);
        QByteArray pinCode = ui->pin->text().toUtf8();

        QByteArray keyDb;
        QByteArray ivDb;
        computeAESKeyAndIV(pinCode, databaseSalt.toUtf8(), keyDb, ivDb);

        QSqlQuery query(this->db);
        query.prepare("DELETE FROM creds WHERE id = :id");
        query.bindValue(":id", id);

        if (!query.exec()) {
            qWarning() << "Deletion error:" << query.lastError().text();
            idToDel = -1;
        }

        this->isChanged = true;
        applyChanges(keyDb, ivDb);
        this->isChanged = false;

        wipe(keyDb);
        wipe(ivDb);
        wipe(pinCode);
    }
    clearDataLoginForm();

    openHomePage();
}


QString MainWindow::getEncLogin(QString id)
{
    QString encLogin = "";

    QSqlQuery query(this->db);
    query.prepare("SELECT * FROM creds WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    QSqlRecord rec = query.record();

    int loginIndex = rec.indexOf("login");

    while (query.next()) {
        encLogin = query.value(loginIndex).toString();
    }

    return encLogin;
}


QString MainWindow::getEncPassword(QString id)
{
    QString encPassword = "";

    QSqlQuery query(this->db);
    query.prepare("SELECT * FROM creds WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    QSqlRecord rec = query.record();

    int passIndex = rec.indexOf("password");

    while (query.next()) {
        encPassword = query.value(passIndex).toString();
    }

    return encPassword;
}


void MainWindow::getSalt(QString id, QString &salt)
{
    QSqlQuery query(this->db);
    query.prepare("SELECT * FROM creds WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();

    QSqlRecord rec = query.record();

    int saltIndex = rec.indexOf("salt");

    while (query.next()) {
        salt = query.value(saltIndex).toString();
    }
}


void MainWindow::loginForUpd()
{
    connect(this, &MainWindow::successPin, this, &MainWindow::updCreds);
    openLoginForm();
}


void MainWindow::updCreds()
{
    disconnect(this, &MainWindow::successPin, this, nullptr);
    qDebug() << "Upd Creds";

    QString login = ui->loginChange->text();
    QString password = ui->passwordChange->text();

    if (login == "" && password == "") {
        qDebug() << "Nothing to change";
        openHomePage();
        return;
    }

    if (idToUpd != -1) {
        QString id = QString::number(idToUpd);
        QString encLogin = "";
        QString encPassword = "";
        QString salt = "";
        getSalt(id, salt);

        QByteArray pinCode = ui->pin->text().toUtf8();

        QByteArray key;
        QByteArray iv;
        computeAESKeyAndIV(pinCode, salt.toUtf8(), key, iv);

        QByteArray keyDb;
        QByteArray ivDb;
        computeAESKeyAndIV(pinCode, databaseSalt.toUtf8(), keyDb, ivDb);


        if (login == "") {
            encLogin = getEncLogin(id);
        } else {
            QByteArray loginArr = login.toUtf8();
            QByteArray encLoginArr;
            if (!encryptAES256CBC(loginArr, key, iv, encLoginArr)) {
                qDebug() << "Encryption failed!";
            }
            encLogin = encLoginArr.toHex();
        }

        if (password == "") {
            encPassword = getEncPassword(id);
        } else {
            QByteArray passwordArr = password.toUtf8();
            QByteArray encPasswordArr;
            if (!encryptAES256CBC(passwordArr, key, iv, encPasswordArr)) {
                qDebug() << "Encryption failed!";
            }
            encPassword = encPasswordArr.toHex();
        }


        QSqlQuery query(this->db);
        query.prepare("UPDATE creds SET login = :login, password = :password "
                      "WHERE id = :id");

        query.bindValue(":id",  id);
        query.bindValue(":login",  encLogin);
        query.bindValue(":password",  encPassword);

        clearDataLoginForm();
        clearDataUpdForm();

        if (!query.exec()) {
            qWarning() << "Updating error:" << query.lastError().text();
        } else {

            this->isChanged = true;
            applyChanges(keyDb, ivDb);
            this->isChanged = false;
        }

        wipe(key);
        wipe(iv);
        wipe(keyDb);
        wipe(ivDb);
        wipe(salt);
        wipe(pinCode);


    }

    openHomePage();
}


bool MainWindow::applyChanges(QByteArray &key, QByteArray &iv)
{
    QString dump;

    dump += "PRAGMA foreign_keys=OFF;\n";
    dump += "BEGIN TRANSACTION;\n";

    QSqlQuery query("SELECT sql FROM sqlite_master WHERE type='table' AND sql NOT NULL", db);
    while (query.next()) {
        QString tableDefinition = query.value(0).toString();

        if (!tableDefinition.contains("sqlite_sequence")) {
            dump += tableDefinition + ";\n";
        }
    }

    QStringList tables;
    query.exec("SELECT name FROM sqlite_master WHERE type='table'");
    while (query.next()) {
        QString tableName = query.value(0).toString();

        if (tableName != "sqlite_sequence") {
            tables.append(tableName);
        }
    }

    for (const QString &table : tables) {
        query.exec(QString("SELECT * FROM %1").arg(table));
        while (query.next()) {
            dump += "INSERT INTO " + table + " VALUES(";
            for (int i = 0; i < query.record().count(); ++i) {
                dump += "'" + query.value(i).toString().replace("'", "''") + "'";
                if (i < query.record().count() - 1) dump += ", ";
            }
            dump += ");\n";
        }
    }

    query.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='sqlite_sequence'");
    if (query.next()) {
        dump += "DELETE FROM sqlite_sequence;\n";
        query.exec("SELECT name, seq FROM sqlite_sequence");
        while (query.next()) {
            dump += QString("INSERT INTO sqlite_sequence VALUES('%1', %2);\n")
            .arg(query.value(0).toString())
                .arg(query.value(1).toInt());
        }
    }

    dump += "COMMIT;\n";

    QByteArray dumpArr = dump.toUtf8();
    QByteArray encDumpArr;

    //qDebug() << "Key: " << key.toHex();
    //qDebug() << "Iv: " << iv.toHex();

    if (!encryptAES256CBC(dumpArr, key, iv, encDumpArr)) {
        qDebug() << "Encryption failed!";
        return false;
    }

    QFile file(encDbName);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << encDbName;
        return false;
    }

    qint64 bytesWritten = file.write(encDumpArr);
    file.close();

    if (bytesWritten != encDumpArr.size()) {
        qDebug() << "Failed to write all encrypted data to file!";
        return false;
    }

    return true;
}


void MainWindow::wipe(QByteArray& mem)
{
    for (int i = 0; i < mem.size(); ++i) {
        mem[i] = static_cast<char>(QRandomGenerator::global()->generate() & 0xFF);
    }
}


void MainWindow::wipe(QString& str) {
    int len = str.length();
    for (int i = 0; i < len; ++i) {
        ushort randomChar = QRandomGenerator::global()->bounded(0x21, 0x7E); // Printable ASCII range
        str[i] = QChar(randomChar);
    }
    str.clear();
}


void MainWindow::clearDataLoginForm()
{
    ui->pin->setText("");
    ui->wrongPinInfo->setVisible(false);
    ui->locked->setPixmap(QPixmap());
}


void MainWindow::clearDataAddForm()
{
    ui->loginLine->setText("");
    ui->passwordLine->setText("");
    ui->siteLine->setText("");
}


void MainWindow::clearDataUpdForm()
{
    ui->info->setText("");
    ui->loginChange->setText("");
    ui->passwordChange->setText("");
}


void MainWindow::clearDataHomePage()
{
    QWidget *oldWidget = ui->scrollArea->widget();
    if (oldWidget) {
        oldWidget->deleteLater(); // безопасное удаление после событий
    }
    QWidget *window = new QWidget;
    ui->scrollArea->setWidget(window);
    ui->searchline->setText("");
    ui->noCreds->setVisible(false);
}


void MainWindow::checkScreenshooter()
{
    QtConcurrent::run([this]() {
        // Проверка процессов
        QProcess process;
        process.start("pgrep", QStringList() << "-f" << "xfce4-screenshooter");
        process.waitForFinished();
        QString output1 = process.readAllStandardOutput();
        bool xfceShot = !output1.trimmed().isEmpty();

        QProcess flameshot;
        flameshot.start("pgrep", QStringList() << "-f" << "flameshot");
        flameshot.waitForFinished();
        QString output2 = flameshot.readAllStandardOutput();
        bool flameshotRunning = !output2.trimmed().isEmpty();

        if (xfceShot || flameshotRunning) {
            QMetaObject::invokeMethod(this, [this]() {
                overlayWidget->show();
                overlayWidget->raise();
                overlayHideTimer->start(5000);

                QClipboard *clipboard = QGuiApplication::clipboard();
                clipboard->clear();
            }, Qt::QueuedConnection);

            // Удаление последнего скриншота
            QString screenshotsDir = QDir::homePath() + "/Pictures";
            QDir dir(screenshotsDir);
            if (dir.exists()) {
                QFileInfoList files = dir.entryInfoList(
                    QStringList() << "*.png" << "*.jpg" << "*.jpeg",
                    QDir::Files, QDir::Time
                    );
                if (!files.isEmpty()) {
                    QFileInfo latest = files.first();
                    QDateTime now = QDateTime::currentDateTime();
                    if (latest.lastModified().secsTo(now) < 5) {
                        QFile::remove(latest.absoluteFilePath());
                        qDebug() << "🗑️ Удалён файл:" << latest.fileName();
                    }
                }
            }
        }
    });
}


void MainWindow::hideOverlay()
{
    overlayWidget->hide();

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->clear();
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
    overlayWidget->setGeometry(0, 0, width(), height());
}



