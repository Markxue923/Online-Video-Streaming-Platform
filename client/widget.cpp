#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include "protocol.h"
#include <QMessageBox>
#include <QDebug>
#include "subwidget.h"
#include <QUdpSocket>

Widget::Widget(QWidget *parent) : QWidget(parent),
                                  ui(new Ui::Widget),
                                  socket(new QTcpSocket(this)),
                                  sw(new SubWidget(socket)),
                                  aw(new anchor(socket)),
                                  cw(new clientwidget(socket))
{
    ui->setupUi(this);

    // sender=new QUdpSocket(this);
    // receiver=new QUdpSocket(this);
    // receiver->bind(8888,QUdpSocket::ReuseAddressHint);
    // connect(&timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    // connect(receiver,SIGNAL(readyRead()),this,SLOT(onReadyRead());

    socket->connectToHost(QHostAddress("192.168.153.1"), 12001);
    QObject::connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
    QObject::connect(sw, SIGNAL(sigSwQuit()), this, SLOT(show()));
    QObject::connect(aw, SIGNAL(sigAwQuit()), sw, SLOT(show()));
    QObject::connect(cw, SIGNAL(sig_cw_quit()), sw, SLOT(show()));
}

Widget::~Widget()
{
    delete ui;
}

/*void Widget::onTimeout()
{
    sender->writeDatagram(data,20,QHostAddress::Broadcast,8888);
}
*/

void Widget::onRegisterButtonClicked()
{
    QString name = ui->leName->text();
    QString pwd = ui->lePwd->text();

    ui->leName->clear();
    ui->lePwd->clear();

    Protocol protocol;
    protocol.setType(Protocol::regist);
    protocol.setName(name);
    protocol.setPwd(pwd);
    qDebug() << "regist";
    socket->write(protocol.toArray());
}

void Widget::onLoginButtonClicked()
{
    QString name = ui->leName->text();
    QString pwd = ui->lePwd->text();

    ui->leName->clear();
    ui->lePwd->clear();

    Protocol protocol;
    protocol.setType(Protocol::login);
    protocol.setName(name);
    protocol.setPwd(pwd);

    socket->write(protocol.toArray());
}

void Widget::onReadyRead()
{
    Protocol protocol;
    while (socket->read(protocol.toChar(), protocol.size()))
    {
        switch (protocol.getType())
        {
        case Protocol::regist_success:
            QMessageBox::information(this, "regist_success", protocol.getData());
            break;

        case Protocol::regist_failed:
            QMessageBox::warning(this, "regist_failed", protocol.getData());
            break;

        case Protocol::login_success:
            QMessageBox::information(this, "login_success", protocol.getData());
            this->hide();
            sw->setName(protocol.getName());
            sw->show();
            break;

        case Protocol::login_failed:
            QMessageBox::warning(this, "login_failed", protocol.getData());
            break;

        case Protocol::chat:
            aw->chatText(protocol.getData());
            cw->chatAppend(protocol.getData());
            break;

        case Protocol::printf_room:
            sw->appendText(protocol.getData());
            break;

        case Protocol::create_success:
            sw->hide();
            qDebug() << "create_success";
            aw->show();
            aw->setName(protocol.getName());
            break;

        case Protocol::enterAnchor_success:
            cw->setClientName(protocol.getName());
            cw->setAnchorName(protocol.getData());
            sw->hide();
            cw->show();
            break;
        case Protocol::printf_client:
            aw->appendText(protocol.getData());
            cw->clientAppend(protocol.getData());
            break;

        default:
            break;
        }
    }
}
