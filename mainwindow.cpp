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

    ui->updBtn->setStyleSheet("QPushButton:hover { border-style: outset; border-width: 1px; color: #413FA7; "
                                 "border-radius: 10px; border-color: #413FA7; background-color: #0B0317; }");
    ui->info->setStyleSheet("QLabel { color: #413FA7; font: 14px; }");
    ui->label_2->setStyleSheet("QLabel { font: 15px; }");

    setWindowIcon(QIcon(":/rc/icons/icon.ico"));
    setWindowTitle("Password Manager");

    connect(ui->enterBtn, &QPushButton::clicked, this, &MainWindow::checkPin);
    connect(ui->pin, &QLineEdit::returnPressed, this, &MainWindow::checkPin);

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

    ui->echoLogin->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");
    ui->echoPass->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");

    connect(ui->echoLogin, &QPushButton::clicked, this, &MainWindow::changeViewMode);
    connect(ui->echoPass, &QPushButton::clicked, this, &MainWindow::changeViewMode);
    connect(ui->echoLogin_2, &QPushButton::clicked, this, &MainWindow::changeViewMode);
    connect(ui->echoPass_2, &QPushButton::clicked, this, &MainWindow::changeViewMode);

    connect(ui->insertBtn, &QPushButton::clicked, this, &MainWindow::addCreds);
    connect(ui->updBtn, &QPushButton::clicked, this, &MainWindow::updCreds);

    //ui->retHome->setStyleSheet("QPushButton:hover { border: none; background: transparent; }");
    connect(ui->retHome, &QPushButton::clicked, this, &MainWindow::openHomePage);
    connect(ui->retHome_2, &QPushButton::clicked, this, &MainWindow::openHomePage);

    connect(ui->searchBtn, &QPushButton::clicked, this, &MainWindow::filterByHostname);
}


MainWindow::~MainWindow()
{
    delete ui;
    if (isChanged) {
        if(!applyChanges()) {
            qDebug() << "Dump failed!";
        }
    }

    this->db.close();
    wipe(keyPass);
    wipe(ivPass);
    wipe(keyLogin);
    wipe(ivLogin);
}


QStringList MainWindow::readSQLStatements(const QByteArray &data)
{
    QStringList sqlStatements;
    QByteArray currentStatement;

    QList<QByteArray> lines = data.split('\n');
    for (const QByteArray &line : lines) {
        QByteArray trimmedLine = line.trimmed();

        if (trimmedLine.isEmpty()) {
            continue;  // Пропускаем пустые строки
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
        qDebug() << "Ошибка при открытии базы данных в памяти!";
        return false;
    }

    QStringList sqlCommands = readSQLStatements(decryptedData);

    for (const QString &sql : sqlCommands) {
        QSqlQuery query(db);
        if (!query.exec(sql)) {
            qDebug() << "Ошибка в SQL:" << sql << "→" << query.lastError().text();
            return false;
        }
    }

    return true;
}


void MainWindow::checkPin()
{
    //QByteArray keyDatabase;
    //QByteArray ivDatabase;

    QString dbName = "../../info.db";
    computeAESKeyAndIV(ui->pin->text().toUtf8(), dbName.toUtf8(), keyDatabase, ivDatabase);


    QByteArray decDBFile;
    QFile file(encDbName);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Ошибка открытия файла:" << file.errorString();
        return;
    }
    QByteArray encDBFile = file.readAll();

    if (!decryptAES256CBC(encDBFile, keyDatabase, ivDatabase, decDBFile)) {
        qDebug() << "Ошибка расшифрования!" ;
    }


    if (QString::fromUtf8(decDBFile.left(100)).contains("creds", Qt::CaseInsensitive)) {
        qDebug() << "Here dec: " << QString::fromUtf8(decDBFile.left(100));
        //encrypt pin
        this->encryptedPin = ui->pin->text();

        ui->pin->setText("");

        ui->stackedWidget->setCurrentWidget(ui->page_2);
        this->setMinimumSize(865, 543);
        this->setMaximumSize(865, 543);

        QString loginSalt = "093ls902hsdfd";
        computeAESKeyAndIV(ui->pin->text().toUtf8(), loginSalt.toUtf8(), keyPass, ivPass);
        qDebug() << "keyPass: " << keyPass.toHex();
        qDebug() << "ivPass: " << ivPass.toHex();

        QString passwordSalt = "dfdbt89tr.";
        computeAESKeyAndIV(ui->pin->text().toUtf8(), passwordSalt.toUtf8(), keyLogin, ivLogin);
        qDebug() << "keyLogin: " << keyLogin.toHex();
        qDebug() << "ivLogin: " << ivLogin.toHex();

        if (restoreDatabaseInMemory(decDBFile)) {
            qDebug() << "База данных успешно восстановлена.";
        } else {
            qDebug() << "Ошибка восстановления базы данных.";
        }

        QVector<Credentials> creds = readCredentials();
        setCredentials(creds);


        //qDebug() << decryptAndLoadDatabase(keyDatabase, ivDatabase);



    } else {
        wipe(keyDatabase);
        wipe(ivDatabase);

        ui->pin->setText("");
        ui->wrongPinInfo->setVisible(true);
        QPixmap lockedPic (":/rc/icons/blocking.png");

        ui->locked->setPixmap(lockedPic);
    }

}


void MainWindow::copyClipboard()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    QString textToCopy = "";

    QByteArray plaintext;
    QByteArray ciphertext;
    if (button->property("type") == "s") {
        textToCopy = button->property("data").toString();
    } else if (button->property("type") == "l") {
        QString data = button->property("data").toString();
        qDebug() << "Login" << data;
        ciphertext = QByteArray::fromHex(data.toUtf8());
        qDebug() << ciphertext.toHex();
        if (!decryptAES256CBC(ciphertext, keyLogin, ivLogin, plaintext)) {
            //TODO: failed to decrypt, show window
        }
        qDebug() << "Decrypted: " << plaintext;
        textToCopy = QString::fromUtf8(plaintext);

    } else if (button->property("type") == "p") {
        //decrypt with keyLogin and ivLogin
        QString data = button->property("data").toString();
        qDebug() << "Password" << data;
        ciphertext = QByteArray::fromHex(data.toUtf8());
        qDebug() << ciphertext.toHex();
        if (!decryptAES256CBC(ciphertext, keyPass, ivPass, plaintext)) {
            //TODO: failed to decrypt, show window
            qDebug() << "Decryption failed";
        }
        qDebug() << "Decrypted: " << plaintext;

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

        connect(copySite, &QPushButton::clicked, this, &MainWindow::copyClipboard);
        connect(copyLogin, &QPushButton::clicked, this, &MainWindow::copyClipboard);
        connect(copyPassword, &QPushButton::clicked, this, &MainWindow::copyClipboard);

        QPushButton *deleteCredentials = new QPushButton();
        QPixmap pixDelete(":/rc/icons/delete.png");
        QIcon delBtn(pixDelete);
        deleteCredentials->setIcon(delBtn);
        deleteCredentials->setMaximumSize(30, 30);
        connect(deleteCredentials, &QPushButton::clicked, this, &MainWindow::delCreds);

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


void MainWindow::openAddForm()
{
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    ui->siteLine->setFocus();
}


void MainWindow::openUpdForm()
{
    ui->stackedWidget->setCurrentWidget(ui->page_4);
    ui->siteLine->setFocus();
    QString hostname = "";
    QString id = "";

    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QWidget *parentWidget = qobject_cast<QWidget*>(button->parent());
        if (parentWidget) {
            id = parentWidget->property("id").toString();
            qDebug() << "Val id: " << id;

            QSqlQuery query(this->db);
            query.prepare("SELECT * FROM creds WHERE id = :id");
            query.bindValue(":id", id);
            query.exec();

            QSqlRecord rec = query.record();

            int siteIndex = rec.indexOf("hostname");

            while (query.next()) {
                hostname = query.value(siteIndex).toString();
                qDebug() << "Hostname: " << hostname;
            }

        }
    }

    ui->info->setText(hostname);

    ui->updBtn->setProperty("id", id);
}


void MainWindow::openHomePage()
{
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}


void MainWindow::filterByHostname()
{
    QString hostnamePart = "%" + ui->searchline->text() + "%";
    qDebug() << "Hostname part: " << hostnamePart;
    QVector<Credentials> creds = readCredentials(hostnamePart);
    setCredentials(creds);
}


void MainWindow::changeViewMode()
{
    QPushButton *clicked = qobject_cast<QPushButton*>(sender());

    int mode = clicked->property("mode").toInt();

    if (mode == 0) { //данные скрыты
        qDebug() << "here";
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
    //QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    //db.setDatabaseName(this->dbName);
    //if (!db.open()) {
    //    qDebug() << "not opened";
    //}
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
    qDebug() << credentialsInfo.size();

    return credentialsInfo;
}


void MainWindow::addCreds()
{
    qDebug() << "add new";


    QString hostname = ui->siteLine->text();
    //encrypt with keyLogin and ivLogin
    QString login = ui->loginLine->text();
    QByteArray loginArr = login.toUtf8();
    QByteArray encLoginArr;
    if (!encryptAES256CBC(loginArr, this->keyLogin, this->ivLogin, encLoginArr)) {
        qDebug() << "Encryption failed!";
    }
    QString encLogin = encLoginArr.toHex();
    qDebug() << "Encrypted login: " << encLogin;

    //encrypt with keyPass and ivPass
    QString password = ui->passwordLine->text();
    QByteArray passwordArr = password.toUtf8();
    QByteArray encPasswordArr;
    if (!encryptAES256CBC(passwordArr, this->keyPass, this->ivPass, encPasswordArr)) {
        qDebug() << "Encryption failed!";
    }
    QString encPassword = encPasswordArr.toHex();
    qDebug() << "Encrypted password: " << encPassword;

    ui->siteLine->setText("");
    ui->loginLine->setText("");
    ui->passwordLine->setText("");


    QSqlQuery query(this->db);
    query.prepare("INSERT INTO creds (hostname, login, password) "
                  "VALUES (:hostname, :login, :password)");
    query.bindValue(":hostname", hostname);
    query.bindValue(":login",  encLogin);
    query.bindValue(":password",  encPassword);


    query.exec();

    QVector<Credentials> credsUpd = readCredentials();
    this->isChanged = true;
    setCredentials(credsUpd);
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
                QVector<Credentials> credsUpd = readCredentials();
                this->isChanged = true;
                setCredentials(credsUpd);
            }
        }
    }
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


void MainWindow::updCreds()
{
    qDebug() << "Upd creds";
    QPushButton *button = qobject_cast<QPushButton*>(sender());

    QString login = ui->loginChange->text();
    QString password = ui->passwordChange->text();

    if (login == "" && password == "") {
        return;
    }

    if (button) {
        QString id = button->property("id").toString();
        qDebug() << "Id: " << id;

        QString encLogin = "";
        QString encPassword = "";

        qDebug() << "Log and pass";

        if (login == "") {
            encLogin = getEncLogin(id);
        } else {
            QByteArray loginArr = login.toUtf8();
            QByteArray encLoginArr;
            if (!encryptAES256CBC(loginArr, this->keyLogin, this->ivLogin, encLoginArr)) {
                qDebug() << "Encryption failed!";
            }
            encLogin = encLoginArr.toHex();
            qDebug() << "Encrypted login: " << encLogin;
        }

        if (password == "") {
            encPassword = getEncPassword(id);
        } else {
            QByteArray passwordArr = password.toUtf8();
            QByteArray encPasswordArr;
            if (!encryptAES256CBC(passwordArr, this->keyPass, this->ivPass, encPasswordArr)) {
                qDebug() << "Encryption failed!";
            }
            encPassword = encPasswordArr.toHex();
            qDebug() << "Encrypted password: " << encPassword;
        }

        qDebug() << "before query";

        QSqlQuery query(this->db);
        query.prepare("UPDATE creds SET login = :login, password = :password "
                      "WHERE id = :id");

        query.bindValue(":id",  id);
        query.bindValue(":login",  encLogin);
        query.bindValue(":password",  encPassword);

        if (!query.exec()) {
            qWarning() << "Ошибка удаления:" << query.lastError().text();
        } else {
            qDebug() << "Updated";
            QVector<Credentials> credsUpd = readCredentials();
            this->isChanged = true;
            setCredentials(credsUpd);
            ui->stackedWidget->setCurrentWidget(ui->page_2);

            ui->loginChange->setText("");
            ui->passwordChange->setText("");

        }
    }

}


bool MainWindow::applyChanges()
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


    if (!encryptAES256CBC(dumpArr, this->keyDatabase, this->ivDatabase, encDumpArr)) {
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
    mem.fill(0);
}
