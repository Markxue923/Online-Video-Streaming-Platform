#ifndef SOCKETMANAGER_H
#define SOCKETMANAGER_H

#include <QTcpSocket>
#include <QVector>

class SocketManager
{
private:
    SocketManager();
    static SocketManager *instance;
    QVector<QTcpSocket *> sockets;

public:
    static SocketManager *getInstance();

    void appendSocket(QTcpSocket *socket)
    {
        sockets.append(socket);
    }

    void deleteSocket(QTcpSocket *socket)
    {
        QVector<QTcpSocket *>::iterator it;
        for (it = sockets.begin(); it != sockets.end(); ++it)
        {
            if (*it == socket)
            {
                sockets.erase(it);
                return;
            }
        }
    }

    QVector<QTcpSocket *> &getSockets()
    {
        return sockets;
    }
};

#endif // SOCKETMANAGER_H
