#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>

void computeAESKeyAndIV(const QByteArray &inputCode, const QByteArray &salt, QByteArray &key, QByteArray &iv);

bool encryptAES256CBC(const QByteArray &plainData, const QByteArray &key, const QByteArray &iv, QByteArray &encData);
bool decryptAES256CBC(const QByteArray &encData, const QByteArray &key, const QByteArray &iv, QByteArray &decData);


#endif // CRYPTO_H
