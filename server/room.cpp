#include "room.h"



Room::Room()
{

}

QString Room::get_room_name() const
{
    return room_name;
}

void Room::set_room_name(const QString &value)
{
    room_name=value;
}

QVector<QTcpSocket *> &Room::return_Users()
{
    return users;
}

QVector<QString> &Room::return_users_name()
{
    return users_name;
}
