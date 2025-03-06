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
    this->db.close();
    wipe(keyPass);
    wipe(ivPass);
    wipe(keyLogin);
    wipe(ivLogin);
}


void MainWindow::generateAESKeyAndIV(const QByteArray &pin, QByteArray &key, QByteArray &iv) {
    QByteArray salt = dbName.toUtf8();
    QByteArray hash = QCryptographicHash::hash(pin + salt, QCryptographicHash::Sha256);

    key = hash.left(32);
    iv = hash.right(16);

    qDebug() << key.toHex();
    qDebug() << iv.toHex();

    QString newSalt = "dfdbt89tr.";
    salt = newSalt.toUtf8();
    QByteArray hash2 = QCryptographicHash::hash(pin + salt, QCryptographicHash::Sha256);

    keyPass = hash2.left(32);
    ivPass = hash2.right(16);

    qDebug() << keyPass.toHex();
    qDebug() << ivPass.toHex();

    newSalt = "093ls902hsdfd";
    salt = newSalt.toUtf8();
    QByteArray hash3 = QCryptographicHash::hash(pin + salt, QCryptographicHash::Sha256);

    keyLogin = hash3.left(32);
    ivLogin = hash3.right(16);

    qDebug() << keyLogin.toHex();
    qDebug() << ivLogin.toHex();
}


QString MainWindow::encryptData(const QString &data, const QByteArray &key, const QByteArray &iv) {
    QByteArray inputData = data.toUtf8();

    QByteArray encryptedData(4096 + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        qWarning() << "Ошибка создания контекста OpenSSL!";
        return QString();
    }

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.data()),
                       reinterpret_cast<const unsigned char*>(iv.data()));

    int outLen = 0;
    EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(encryptedData.data()), &outLen,
                      reinterpret_cast<const unsigned char*>(inputData.constData()), inputData.size());

    int finalLen = 0;
    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(encryptedData.data()) + outLen, &finalLen);

    EVP_CIPHER_CTX_free(ctx);

    encryptedData.resize(outLen + finalLen);

    return QString(encryptedData.toHex());
}


QString MainWindow::decryptData(const QString &data, const QByteArray &key, const QByteArray &iv) {
    QByteArray encryptedBytes = QByteArray::fromHex(data.toUtf8());
    QByteArray decryptedData(encryptedBytes.size(), 0);

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        qWarning() << "Ошибка создания контекста OpenSSL!";
        return QString();
    }

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.data()),
                       reinterpret_cast<const unsigned char*>(iv.data()));

    int outLen = 0;
    EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(decryptedData.data()), &outLen,
                      reinterpret_cast<const unsigned char*>(encryptedBytes.constData()), encryptedBytes.size());

    int finalLen = 0;
    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(decryptedData.data()) + outLen, &finalLen);

    EVP_CIPHER_CTX_free(ctx);

    decryptedData.resize(outLen + finalLen);
    return QString::fromUtf8(decryptedData);
}


bool MainWindow::encryptFile(const QByteArray &key, const QByteArray &iv)
{
    QFile inputFile(dbName);
    QFile outputFile(encDbName);

    if (!inputFile.open(QIODevice::ReadOnly) || !outputFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Ошибка открытия файлов!";
        return false;
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        qWarning() << "Ошибка создания контекста OpenSSL!";
        return false;
    }

    EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.data()),
                       reinterpret_cast<const unsigned char*>(iv.data()));

    QByteArray buffer(4096, 0);
    QByteArray encryptedBuffer(4096 + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);

    int outLen = 0;
    while (!inputFile.atEnd()) {
        int bytesRead = inputFile.read(buffer.data(), buffer.size());
        EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(encryptedBuffer.data()), &outLen,
                          reinterpret_cast<const unsigned char*>(buffer.constData()), bytesRead);
        outputFile.write(encryptedBuffer.constData(), outLen);
    }

    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(encryptedBuffer.data()), &outLen);
    outputFile.write(encryptedBuffer.constData(), outLen);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}


QByteArray MainWindow::decryptFile(const QByteArray &key, const QByteArray &iv) {
    QFile inputFile(encDbName);
    if (!inputFile.open(QIODevice::ReadOnly)) {
        qWarning() << "Ошибка открытия файла!";
    }

    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        qWarning() << "Ошибка создания контекста OpenSSL!";
    }

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.data()),
                       reinterpret_cast<const unsigned char*>(iv.data()));

    QByteArray decryptedData;
    QByteArray buffer(4096, 0);
    QByteArray decryptedBuffer(4096 + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);

    int outLen = 0;
    while (!inputFile.atEnd()) {
        int bytesRead = inputFile.read(buffer.data(), buffer.size());
        EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(decryptedBuffer.data()), &outLen,
                          reinterpret_cast<const unsigned char*>(buffer.constData()), bytesRead);
        decryptedData.append(decryptedBuffer.constData(), outLen);
    }

    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(decryptedBuffer.data()), &outLen);
    decryptedData.append(decryptedBuffer.constData(), outLen);

    EVP_CIPHER_CTX_free(ctx);

    return decryptedData;
}


void MainWindow::copyClipboard()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    QString textToCopy = "";
    if (button->property("type") == "s") {
        textToCopy = button->property("data").toString();
    } else if (button->property("type") == "l") {
        //decrypt with keyLogin and ivLogin
        textToCopy = button->property("data").toString();
    } else if (button->property("type") == "p") {
        //decrypt with keyLogin and ivLogin
        textToCopy = button->property("data").toString();
    }
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText("text");
}




//setupUI
void MainWindow::setAllCredentials()
{
    QVector<Credentials> credsInfo = readCredentials();

    if (credsInfo.size() == 0) {
        ui->noCreds->setVisible(true);
    } else {
        ui->noCreds->setVisible(false);
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


        QLabel *site = new QLabel(credsInfo[i].getSite());
        QLabel *login = new QLabel("********");
        QLabel *password = new QLabel("********");
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
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(this->dbName);
    //if (!db.open()) {
    //    qDebug() << "not opened";
    //}
    QSqlQuery query(this->db);
    query.prepare("SELECT * FROM creds");

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



        creds.setId(id);
        creds.setSite(site);
        creds.setLogin(login);
        creds.setPassword(password);

        credentialsInfo.push_back(creds);
    }
    qDebug() << credentialsInfo.size();


    return credentialsInfo;
}

void MainWindow::addCreds()
{
    qDebug() << "add new";

    QString hostname = ui->siteLine->text();
    //encrypt with keyLogin and ivLogin
    QString login = ui->loginLine->text();
    //encrypt with keyPass and ivPass
    QString password = ui->passwordLine->text();

    ui->siteLine->setText("");
    ui->loginLine->setText("");
    ui->passwordLine->setText("");


    QSqlQuery query(this->db);
    query.prepare("INSERT INTO creds (hostname, login, password) "
                  "VALUES (:hostname, :login, :password)");
    query.bindValue(":hostname", hostname);
    query.bindValue(":login",  login);
    query.bindValue(":password",  password);


    query.exec();

    setAllCredentials();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


void MainWindow::delCreds()
{
    qDebug() << "Del creds";
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QWidget *parentWidget = qobject_cast<QWidget*>(button->parent());
        if (parentWidget) {
            QString id = parentWidget->property("id").toString();
            qDebug() << "Val id: " << id;

            QSqlQuery query(this->db);
            query.prepare("DELETE FROM creds WHERE id = :id");
            query.bindValue(":id", id);

            // Выполняем запрос
            if (!query.exec()) {
                qWarning() << "Ошибка удаления:" << query.lastError().text();
            } else {
                setAllCredentials();
            }
            db.close();
        }
    }

}


void MainWindow::checkPin()
{
    QString pin = ui->pin->text();

    if (pin == "12345") {
        //encrypt pin
        this->encryptedPin = pin;

        ui->stackedWidget->setCurrentWidget(ui->page_2);
        this->setMinimumSize(865, 543);
        this->setMaximumSize(865, 543);

        QByteArray pinArr = pin.toUtf8();
        QByteArray keyDatabase;
        QByteArray ivDatabase;

        generateAESKeyAndIV(pinArr, keyDatabase, ivDatabase);

        QString pass = "pass1";
        QString encPass = encryptData(pass, keyPass, ivPass);
        qDebug() << keyPass.toHex();
        qDebug() << ivPass.toHex();
        qDebug() << encPass;

        qDebug() << decryptAndLoadDatabase(keyDatabase, ivDatabase);

        wipe(keyDatabase);
        wipe(ivDatabase);

        this->setAllCredentials();
    } else {
        ui->pin->setText("");
        ui->wrongPinInfo->setVisible(true);
        QPixmap lockedPic (":/rc/icons/blocking.png");

        ui->locked->setPixmap(lockedPic);
    }

}


bool MainWindow::decryptAndLoadDatabase(const QByteArray &key, const QByteArray &iv)
{
    QByteArray decryptedDatabase = decryptFile(key, iv);

    QString tempFileName = QDir::temp().filePath("temp_sqlite.db");
    QFile tempFile(tempFileName);

    if (!tempFile.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open temporary file";
        return false;
    }

    tempFile.write(decryptedDatabase);
    tempFile.close();

    this->db = QSqlDatabase::addDatabase("QSQLITE", "memory_connection");
    db.setDatabaseName(":memory:");

    if (!db.open()) {
        qDebug() << "Failed to open SQLite database in memory.";
        return false;
    }

    QSqlQuery query(db);

    query.prepare("ATTACH DATABASE :filename AS attachedDB;");
    query.bindValue(":filename", tempFileName);

    if (!query.exec()) {
        qDebug() << "Failed to attach database to memory:" << query.lastError().text();
        return false;
    }

    return true;
}


void MainWindow::wipe(QByteArray& mem)
{
    mem.fill(0);
}
