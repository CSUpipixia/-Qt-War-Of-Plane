#ifndef DBHELPER_H
#define DBHELPER_H
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#define dBHelperInstance DBHelper::getInstance()

class DBHelper
{
public:
    QSqlDatabase database;

    static DBHelper* getInstance()
    {
        if(dBHelper == nullptr)
        {
            dBHelper = new DBHelper();
        }

        return dBHelper;
    }

private:
    DBHelper();//禁止构造函数
    DBHelper(const DBHelper&);//禁止拷贝构造函数
    DBHelper & operator=(const DBHelper &);//禁止赋值拷贝构造函数

    static DBHelper *dBHelper;

    static QString dbType;
    static QString dbFile;
    static QString dbName;
    static QString dbUser;
    static QString dbPasswd;
};

#endif // DBHELPER_H
