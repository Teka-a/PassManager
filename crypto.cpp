#include "crypto.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QString>

#include <openssl/evp.h>
#include <openssl/sha.h>

void computeAESKeyAndIV(const QByteArray &inputCode, const QByteArray &salt, QByteArray &key, QByteArray &iv)
{
    QByteArray msg = inputCode + salt;
    QByteArray hash = QCryptographicHash::hash(msg, QCryptographicHash::Sha512);

    key = hash.left(32);
    iv = hash.right(16);

    msg.clear();
    //qDebug() << key.toHex();
    //qDebug() << iv.toHex();
}


bool encryptAES256CBC(const QByteArray &plainData, const QByteArray &key, const QByteArray &iv, QByteArray &encData)
{
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
    int bytesProcessed = 0;

    while (bytesProcessed < plainData.size()) {
        int bytesToProcess = qMin(buffer.size(), plainData.size() - bytesProcessed);
        buffer.replace(0, bytesToProcess, plainData.mid(bytesProcessed, bytesToProcess));

        EVP_EncryptUpdate(ctx, reinterpret_cast<unsigned char*>(encryptedBuffer.data()), &outLen,
                          reinterpret_cast<const unsigned char*>(buffer.constData()), bytesToProcess);
        encData.append(encryptedBuffer.constData(), outLen);

        bytesProcessed += bytesToProcess;
    }

    EVP_EncryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(encryptedBuffer.data()), &outLen);
    encData.append(encryptedBuffer.constData(), outLen);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}


bool decryptAES256CBC(const QByteArray &encData, const QByteArray &key, const QByteArray &iv, QByteArray &decData)
{
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        qWarning() << "Ошибка создания контекста OpenSSL!";
        return false;
    }

    EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), nullptr,
                       reinterpret_cast<const unsigned char*>(key.data()),
                       reinterpret_cast<const unsigned char*>(iv.data()));

    QByteArray buffer(4096, 0);
    QByteArray decryptedBuffer(4096 + EVP_CIPHER_block_size(EVP_aes_256_cbc()), 0);

    int outLen = 0;
    int bytesProcessed = 0;


    while (bytesProcessed < encData.size()) {
        int bytesToProcess = qMin(buffer.size(), encData.size() - bytesProcessed);
        buffer.replace(0, bytesToProcess, encData.mid(bytesProcessed, bytesToProcess));

        EVP_DecryptUpdate(ctx, reinterpret_cast<unsigned char*>(decryptedBuffer.data()), &outLen,
                          reinterpret_cast<const unsigned char*>(buffer.constData()), bytesToProcess);
        decData.append(decryptedBuffer.constData(), outLen);

        bytesProcessed += bytesToProcess;
    }

    EVP_DecryptFinal_ex(ctx, reinterpret_cast<unsigned char*>(decryptedBuffer.data()), &outLen);
    decData.append(decryptedBuffer.constData(), outLen);

    EVP_CIPHER_CTX_free(ctx);
    return true;
}
