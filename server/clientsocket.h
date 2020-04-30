#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "protocol.h"

class ClientSocket : public QObject
{
    Q_OBJECT

private:
    QTcpSocket* socket;

    void registration(Protocol& p);
    void login(Protocol& p);
    void chat(Protocol& p);
    void quit(Protocol& p);
    void sql(Protocol& p);
    void create_room(Protocol& p);
    void enter_anchor(Protocol& p);
    void refresh_room(Protocol& p);
    void anchor_quit(Protocol& p);
    void refresh_client(Protocol& p);
    void client_quit(Protocol& p);

public:
    explicit ClientSocket(QTcpSocket* socket,QObject *parent = 0);

signals:
    void sigSend(QTcpSocket*,QByteArray);

public slots:
    void onReadyRead();
    void onDisConnected();
};

#endif // CLIENTSOCKET_H
