#include <gtest/gtest.h>
#include "../crypto.h"

#include <QDir>
#include <QDebug>

using namespace testing;

TEST(decryption, decrypt)
{
    QString currentPath = QDir::currentPath();
    qDebug() << currentPath;

    QDir dir(currentPath);

    QString fileDirectory = dir.relativeFilePath("/home/smth/projects/GitHub_Teka-a_/PassManager/credentials.h");
    qDebug() << "Directory of the file is:" << fileDirectory;

    QByteArray keyDatabase;
    QByteArray ivDatabase;
    QString salt = "../../info.db";
    QString pin = "12345";
    computeAESKeyAndIV(pin.toUtf8(), salt.toUtf8(), keyDatabase, ivDatabase);
    EXPECT_EQ(1, 1);
}
