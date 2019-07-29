#include "dbhelper.h"

DBHelper *DBHelper::dBHelper = nullptr;

QString DBHelper::dbType = "QSQLITE";
QString DBHelper::dbFile = "WarOfPlaneDataBase.db";
QString DBHelper::dbName = "qt_sql_default_connection";
QString DBHelper::dbUser = "root";
QString DBHelper::dbPasswd = "1234";

DBHelper::DBHelper()
{
    //建立并打开数据库
    if (QSqlDatabase::contains(dbName))
    {
        database = QSqlDatabase::database(dbName);
    }
    else
    {
        database = QSqlDatabase::addDatabase(dbType); //省略了第二个参数
        database.setDatabaseName(dbFile);
        database.setUserName(dbUser);
        database.setPassword(dbPasswd);
        database.open();
    }


    //建立用户表account
    QSqlQuery sql_query;
    if(sql_query.exec("select * from account"))
    {
        qDebug() << "table account exist!";
    }
    else
    {
        if(sql_query.exec("create table account(id integer primary key autoincrement, username text, password text, unlocklevel integer)"))
        {
            qDebug() << "create table account!";
        }
        else
        {
            qDebug() << "create table account failed!";
        }
    }

    //建立得分表gamescore
    if(sql_query.exec("select * from gamescore"))
    {
        qDebug() << "table gamescore exist!";
    }
    else
    {
        if(sql_query.exec("create table gamescore(id integer primary key autoincrement, username text, level integer, score integer, time text)"))
        {
            qDebug() << "create table gamescore!";
        }
        else
        {
            qDebug() << "create table gamescore failed!";
        }
    }

    database.close();
}


