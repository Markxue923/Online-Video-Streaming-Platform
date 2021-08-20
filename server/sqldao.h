#ifndef SQLDAO_H
#define SQLDAO_H

#include "user.h"
#include <QVector>

class SqlDao
{
public:
    SqlDao();
    virtual ~SqlDao();
    virtual void insertUser(const User &user) = 0;
    virtual void deleteUser(const User &user) = 0;
    virtual void updateUser(const User &user) = 0;
    virtual User *selectUser(const User &user) = 0;
    virtual QVector<User> getAllUser() = 0;
    virtual void exec(const QString &sql) = 0;
};

#endif // SQLDAO_H
