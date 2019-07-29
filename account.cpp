#include "account.h"

Account::Account()
{

}

Account::Account(QString username, QString password, int unlocklevel)
{
    this->username = username;
    this->password = password;
    this->unlocklevel = unlocklevel;
}

QString Account::getUsername()
{
    return this->username;
}

void Account::setUserName(QString username)
{
    this->username = username;
}

QString Account::getPassword()
{
    return this->password;
}

void Account::setPassword(QString password)
{
    this->password = password;
}

int Account::getUnlockLevel()
{
    return unlocklevel;
}

void Account::setUnlockLevel(int unlocklevel)
{
    this->unlocklevel = unlocklevel;
}
