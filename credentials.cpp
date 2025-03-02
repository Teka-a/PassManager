#include "credentials.h"

Credentials::Credentials() {}


void Credentials::setId(int id)
{
    this->id = id;
}


void Credentials::setSite(QString site)
{
    this->site = site;
}


void Credentials::setLogin(QString login)
{
    this->login = login;
}


void Credentials::setPassword(QString password)
{
    this->password = password;
}


int Credentials::getId()
{
    return this->id;
}


QString Credentials::getSite()
{
    //decrypt
    return this->site;
}


QString Credentials::getLogin()
{
    //decrypt
    return this->login;
}


QString Credentials::getPassword()
{
    //decrypt
    return this->password;
}
