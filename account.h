#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <QString>

class Account
{
public:
    Account();
    Account(QString username, QString password, int unlocklevel);

    QString getUsername();
    void setUserName(QString username);
    QString getPassword();
    void setPassword(QString password);
    int getUnlockLevel();
    void setUnlockLevel(int unlocklevel);

private:
    QString username;
    QString password;
    int unlocklevel;
};

#endif // ACCOUT_H
