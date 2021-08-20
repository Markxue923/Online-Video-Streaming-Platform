#include "sqldaoimp.h"
#include "dbmanager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

SqlDaoImp::SqlDaoImp()
{
}

void SqlDaoImp::insertUser(const User &user)
{
    DBManager *manager = DBManager::getInstance();
    manager->createConn();

    QSqlQuery query;
    query.prepare("insert into user(name,pwd)values(:name,:pwd);");
    query.bindValue(":name", user.getName());
    query.bindValue(":pwd", user.getPwd());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    else
    {
    }

    manager->destroyConn();
}

void SqlDaoImp::deleteUser(const User &user)
{
    DBManager *manager = DBManager::getInstance();
    manager->createConn();

    QSqlQuery query;
    query.prepare("delete from user where name = :name;");
    query.bindValue(":name", user.getName());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    else
    {
    }

    manager->destroyConn();
}

void SqlDaoImp::updateUser(const User &user)
{
    DBManager *manager = DBManager::getInstance();
    manager->createConn();

    QSqlQuery query;
    query.prepare("update user set pwd=:pwd,isOnline=:isOnline where name=:name;");
    query.bindValue(":pwd", user.getPwd());
    query.bindValue(":isOnline", user.getIsOnline());
    query.bindValue(":name", user.getName());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    else
    {
    }

    manager->destroyConn();
}

User *SqlDaoImp::selectUser(const User &user)
{
    User *ret = NULL;

    DBManager *manager = DBManager::getInstance();
    manager->createConn();

    QSqlQuery query;
    query.prepare("select * from user where name=:name;");
    query.bindValue(":name", user.getName());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
    }
    else
    {
        if (query.next())
        {
            QString name = query.value(0).toString();
            QString pwd = query.value(1).toString();
            bool isOnline = query.value(2).toBool();
            ret = new User(name, pwd, isOnline);
        }
        else
        {
            ret = NULL;
        }
    }

    manager->destroyConn();

    return ret;
}

QVector<User> SqlDaoImp::getAllUser()
{
    QVector<User> ret;
    DBManager *manager = DBManager::getInstance();
    manager->createConn();

    QSqlQuery query;
    if (!query.exec("select * from user;"))
    {
        qDebug() << query.lastError().text();
    }
    else
    {
        while (query.next())
        {
            QString name = query.value(0).toString();
            QString pwd = query.value(1).toString();
            bool isOnline = query.value(2).toBool();
            User user(name, pwd, isOnline);
            ret.append(user);
        }
    }

    manager->destroyConn();

    return ret;
}

void SqlDaoImp::exec(const QString &sql)
{
    DBManager *manager = DBManager::getInstance();
    manager->createConn();

    QSqlQuery query;
    if (!query.exec(sql))
    {
        qDebug() << query.lastError().text();
    }
    else
    {
    }

    manager->destroyConn();
}
