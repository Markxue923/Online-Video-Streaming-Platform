#include "dbmanager.h"

DBManager::DBManager()
    : db(QSqlDatabase::addDatabase("QSQLITE"))
{
}

DBManager *DBManager::instance = new DBManager;

DBManager *DBManager::getInstance()
{
    return instance;
}

void DBManager::createConn()
{
    db.setDatabaseName("user.db");
    db.open();
}

void DBManager::destroyConn()
{
    db.close();
}
