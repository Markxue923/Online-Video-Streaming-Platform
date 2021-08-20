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
    QVector<QTcpSocket *> users;
    QString room_name;

public:
    Room();
    //QString getUsername() const;
    //void setUsername(const QString &value);

    QString getRoomName() const;
    void setRoomName(const QString &value);

    QVector<QTcpSocket *> &returnUsers();
    QVector<QString> &returnUsersName();
};

#endif // ROOM_H
