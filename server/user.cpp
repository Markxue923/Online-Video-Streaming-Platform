#include "user.h"

QString User::getName() const
{
    return name;
}

void User::setName(const QString &value)
{
    name = value;
}

QString User::getPwd() const
{
    return pwd;
}

void User::setPwd(const QString &value)
{
    pwd = value;
}

bool User::getIsOnline() const
{
    return isOnline;
}

void User::setIsOnline(bool value)
{
    isOnline = value;
}

User::User()
    : isOnline(false)
{
}

User::User(QString name, QString pwd, bool isOnline)
    : name(name), pwd(pwd), isOnline(isOnline)
{
}
