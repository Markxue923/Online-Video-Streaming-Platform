#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QString>
#include <cstring>
#include <QByteArray>

class Protocol
{
public:
    enum Type
    {
        none = 0,
        quit = 1,
        regist = 2,
        regist_success = 3,
        regist_failed = 4,
        login = 5,
        login_success = 6,
        login_failed = 7,
        chat = 8,
        sql = 9,
        createRoom = 10,
        create_success = 11,
        refreshRoom = 12,
        printf_room = 13,
        refreshClient = 14,
        enterAnchor = 15,
        enterAnchor_success = 16,
        anchorQuit = 17,
        clientQuit = 18,
        printf_client = 19
    };

private:
    struct Ptc
    {
        Type type;
        char name[24];
        char pwd[24];
        char data[100];
    } ptc;

public:
    Protocol();

    Type getType() const { return ptc.type; }
    void setType(Type t) { ptc.type = t; }

    QString getName() const { return QString::fromUtf8(ptc.name); }
    void setName(QString n) { strncpy(ptc.name, n.toUtf8().data(), 24); }

    QString getPwd() const { return QString::fromUtf8(ptc.pwd); }
    void setPwd(QString n) { strncpy(ptc.pwd, n.toUtf8().data(), 24); }

    QString getData() const { return QString::fromUtf8(ptc.data); }
    void setData(QString n) { strncpy(ptc.data, n.toUtf8().data(), 24); }

    QByteArray toArray()
    {
        QByteArray array((char *)&ptc, sizeof(ptc));
        return array;
    }

    int size() const { return sizeof(ptc); }
    char *toChar() { return (char *)&ptc; }
    void clear() { memset(&ptc, 0, sizeof(ptc)); }
};

#endif // PROTOCOL_H
