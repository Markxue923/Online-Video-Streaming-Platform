#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QTcpSocket>
#include <QThread>
#include "socketmanager.h"
#include "clientsocket.h"

Widget::Widget(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Widget),
                                  server(new QTcpServer(this))
{
    ui->setupUi(this);

    server->listen(QHostAddress("192.168.153.1"), 12001);

    QObject::connect(server, &QTcpServer::newConnection,
                     this, &Widget::onNewConnection);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::onNewConnection()
{
    QTcpSocket *socket = server->nextPendingConnection();
    SocketManager *manager = SocketManager::getInstance();
    manager->appendSocket(socket);

    ClientSocket *cs = new ClientSocket(socket);
    QThread *th = new QThread(this);

    cs->moveToThread(th);

    th->start();

    QObject::connect(cs, &ClientSocket::sigSend,
                     this, &Widget::onSigSend);

    QObject::connect(socket, &QTcpSocket::disconnected,
                     th, &QThread::quit);
    QObject::connect(th, &QThread::finished,
                     cs, &ClientSocket::deleteLater);
    QObject::connect(th, &QThread::finished,
                     th, &QThread::deleteLater);
}

void Widget::onSigSend(QTcpSocket *socket, QByteArray arr)
{
    socket->write(arr);
}
