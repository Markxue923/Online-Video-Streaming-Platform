#ifndef SQLDAOIMP_H
#define SQLDAOIMP_H

#include "sqldao.h"

class SqlDaoImp : public SqlDao
{
public:
    SqlDaoImp();

    virtual void insertUser(const User &user);
    virtual void deleteUser(const User &user);
    virtual void updateUser(const User &user);
    virtual User *selectUser(const User &user);
    virtual QVector<User> getAllUser();
    virtual void exec(const QString &sql);
};

#endif // SQLDAOIMP_H
