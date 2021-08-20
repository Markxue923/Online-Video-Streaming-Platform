#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "protocol.h"

class ClientSocket : public QObject
{
    Q_OBJECT

private:
    QTcpSocket *socket;

    void registration(Protocol &p);
    void login(Protocol &p);
    void chat(Protocol &p);
    void quit(Protocol &p);
    void sql(Protocol &p);
    void createRoom(Protocol &p);
    void enterAnchor(Protocol &p);
    void refreshRoom(Protocol &p);
    void anchorQuit(Protocol &p);
    void refreshClient(Protocol &p);
    void clientQuit(Protocol &p);

public:
    explicit ClientSocket(QTcpSocket *socket, QObject *parent = 0);

signals:
    void sigSend(QTcpSocket *, QByteArray);

public slots:
    void onReadyRead();
    void onDisConnected();
};

#endif // CLIENTSOCKET_H
