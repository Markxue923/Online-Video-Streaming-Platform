#ifndef USER_H
#define USER_H

#include <QString>

class User
{
private:
    QString name;
    QString pwd;
    bool isOnline;

public:
    User();
    User(QString name, QString pwd, bool isOnline = false);
    QString getName() const;
    void setName(const QString &value);
    QString getPwd() const;
    void setPwd(const QString &value);
    bool getIsOnline() const;
    void setIsOnline(bool value);
};

#endif // USER_H
