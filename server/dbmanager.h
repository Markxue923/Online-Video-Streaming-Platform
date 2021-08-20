#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QSqlDatabase>

class DBManager
{
private:
    DBManager();
    static DBManager *instance;
    QSqlDatabase db;

public:
    static DBManager *getInstance();
    void createConn();
    void destroyConn();
};

#endif // DBMANAGER_H
