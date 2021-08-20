#include "room.h"

Room::Room()
{
}

QString Room::getRoomName() const
{
    return room_name;
}

void Room::setRoomName(const QString &value)
{
    room_name = value;
}

QVector<QTcpSocket *> &Room::returnUsers()
{
    return users;
}

QVector<QString> &Room::returnUsersName()
{
    return users_name;
}
