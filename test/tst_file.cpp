#include <gmock/gmock-matchers.h>
#include <gtest/gtest.h>
#include "/home/smth/projects/GitHub_Teka-a_/PassManager/crypto.cpp"

using namespace testing;

class CryptoTests
    : public ::testing::Test
{
protected:
    int* x;
    QByteArray *key;
    QByteArray *iv;

    void SetUp() override
    {
        std::cout << "Starting up!\n";
        QString derivedKey = "b340947333ac4783f8d88683acc45f0165834e14160df964ffa5d3aee6b9c8dc";
        QString derivedIV = "65834e14160df964ffa5d3aee6b9c8dc";

        key = new QByteArray(QByteArray::fromHex(derivedKey.toUtf8()));
        iv = new QByteArray(QByteArray::fromHex(derivedIV.toUtf8()));

    }

    void TearDown() override
    {
        delete key;
        delete iv;

    }

};


TEST_F(CryptoTests, encryptStr1)
{
    QString textToEnc = "very important text";
    QByteArray plaintext = textToEnc.toUtf8();

    QByteArray producedCiphertext;
    if (!encryptAES256CBC(plaintext, *key, *iv, producedCiphertext)) {
        qDebug() << "Error during encryption occured!";
    }

    QString expectedStr = "7dfbec05754fb8b96c294212ac7571ac9ca55bc77a67ca1ee36de634903d6454";
    QByteArray expectedCiphertext = QByteArray::fromHex(expectedStr.toUtf8());

    EXPECT_EQ(expectedCiphertext, producedCiphertext);
}


TEST_F(CryptoTests, encryptStr2)
{
    QString textToEnc = "not really important text";
    QByteArray plaintext = textToEnc.toUtf8();

    QByteArray producedCiphertext;
    if (!encryptAES256CBC(plaintext, *key, *iv, producedCiphertext)) {
        qDebug() << "Error during encryption occured!";
    }

    QString expectedStr = "3fb3e4682f3a581c7565afe9d79f8b2bcdaa12e580c4c0532a8a62490f0e6caf";
    QByteArray expectedCiphertext = QByteArray::fromHex(expectedStr.toUtf8());

    EXPECT_EQ(expectedCiphertext, producedCiphertext);
}


TEST_F(CryptoTests, decryptStr1)
{
    QString textToDec = "7dfbec05754fb8b96c294212ac7571ac9ca55bc77a67ca1ee36de634903d6454";
    QByteArray ciphertext = QByteArray::fromHex(textToDec.toUtf8());

    QByteArray producedPlaintext;
    if (!decryptAES256CBC(ciphertext, *key, *iv, producedPlaintext)) {
        qDebug() << "Error during decryption occured!";
    }

    QString expectedStr = "very important text";
    QByteArray expectedPlaintext = expectedStr.toUtf8();

    EXPECT_EQ(expectedPlaintext, producedPlaintext);
}


TEST_F(CryptoTests, decryptStr2)
{
    QString textToDec = "3fb3e4682f3a581c7565afe9d79f8b2bcdaa12e580c4c0532a8a62490f0e6caf";
    QByteArray ciphertext = QByteArray::fromHex(textToDec.toUtf8());

    QByteArray producedPlaintext;
    if (!decryptAES256CBC(ciphertext, *key, *iv, producedPlaintext)) {
        qDebug() << "Error during decryption occured!";
    }

    QString expectedStr = "not really important text";
    QByteArray expectedPlaintext = expectedStr.toUtf8();

    EXPECT_EQ(expectedPlaintext, producedPlaintext);
}


TEST_F(CryptoTests, deriveKeyInfo)
{
    QString salt = "../../info.db";
    QString pin = "12345";


    QByteArray producedKey;
    QByteArray producedIV;
    computeAESKeyAndIV(pin.toUtf8(), salt.toUtf8(), producedKey, producedIV);

    QPair<QByteArray, QByteArray> produced {producedKey, producedIV};
    QPair<QByteArray, QByteArray> expected {*key, *iv};

    EXPECT_EQ(expected, produced);
}

