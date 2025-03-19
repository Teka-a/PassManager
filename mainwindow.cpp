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
    //connect(ui->updBtn, &QPushButton::clicked, this, &MainWindow::setCredentials();

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
    //deleteFileSecurely(this->tempFile);
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
    QByteArray keyDatabase;
    QByteArray ivDatabase;

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
        wipe(keyDatabase);
        wipe(ivDatabase);

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
void MainWindow::setCredentials(QVector<Credentials> &credsInfo)
{
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

    QVector<Credentials> credsUpd = readCredentials();
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
                setCredentials(credsUpd);
            }
        }
    }

}


void MainWindow::wipe(QByteArray& mem)
{
    mem.fill(0);
}
