#ifndef CREDENTIALS_H
#define CREDENTIALS_H

#include <QString>

class Credentials
{
public:
    Credentials();

    void setId(int id);
    void setSite(QString site);
    void setLogin(QString login);
    void setPassword(QString password);

    int getId();
    QString getSite();
    QString getLogin();
    QString getPassword();

private:
    int id;
    QString site;
    QString login;
    QString password;


};

#endif // CREDENTIALS_H
