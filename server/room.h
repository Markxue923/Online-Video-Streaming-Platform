#ifndef ROOM_H
#define ROOM_H

#include <QString>
#include <QTcpSocket>
#include <QVector>
#include <QString>
class Room
{
private:
    QVector<QString> users_name;
    QVector<QTcpSocket*> users;
    QString room_name;
public:
    Room();
    //QString getUsername() const;
    //void setUsername(const QString &value);

    QString get_room_name()const;
    void set_room_name(const QString &value);


    QVector<QTcpSocket *>& return_Users();
    QVector<QString>& return_users_name();
};

#endif // ROOM_H
