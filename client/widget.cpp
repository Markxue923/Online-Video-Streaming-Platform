#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include "protocol.h"
#include <QMessageBox>
#include <QDebug>
#include "subwidget.h"
#include <QUdpSocket>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    socket(new QTcpSocket(this)),
    sw(new SubWidget(socket)),
    aw(new anchor(socket)),
    cw(new clientwidget(socket))
{
    ui->setupUi(this);

    //sender=new QUdpSocket(this);
    //receiver=new QUdpSocket(this);

    //receiver->bind(8888,QUdpSocket::ReuseAddressHint);
    //connect(&timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
    //connect(receiver,SIGNAL(readyRead()),this,SLOT(onReadyRead());

    socket->connectToHost(QHostAddress("192.168.153.1"),12001);
    QObject::connect(socket,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    QObject::connect(sw,SIGNAL(sig_sw_quit()),this,SLOT(show()));
    QObject::connect(aw,SIGNAL(sig_aw_quit()),sw,SLOT(show()));
    QObject::connect(cw,SIGNAL(sig_cw_quit()),sw,SLOT(show()));


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
void Widget::on_btnRegist_clicked()
{
    QString name = ui->leName->text();
    QString pwd = ui->lePwd->text();

    ui->leName->clear();
    ui->lePwd->clear();

    Protocol protocol;
    protocol.setType(Protocol::regist);
    protocol.setName(name);
    protocol.setPwd(pwd);
    qDebug()<<"regist";
    socket->write(protocol.toArray());
}

void Widget::on_btnLogin_clicked()
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
    while(socket->read(protocol.toChar(),protocol.size())){
            switch(protocol.getType()){
            case Protocol::regist_success:
                QMessageBox::information(this,"注册信息",protocol.getData());
                break;

            case Protocol::regist_failed:
                QMessageBox::warning(this,"注册信息",protocol.getData());
                break;

            case Protocol::login_success:
                QMessageBox::information(this,"注册信息",protocol.getData());
                this->hide();
                sw->setName(protocol.getName());
                sw->show();
                break;

            case Protocol::login_failed:
                QMessageBox::warning(this,"登录信息",protocol.getData());
                break;

            case Protocol::chat:
                 aw->chatText(protocol.getData());
                 cw->chat_append(protocol.getData());
                break;

            case Protocol::printf_room:
                sw->appendtext(protocol.getData());
                break;

            case Protocol::create_success:
                sw->hide();
                qDebug()<<"create_success";
                aw->show();
                aw->setName(protocol.getName());
                break;

            case Protocol::enter_anchor_success:
                cw->set_client_name(protocol.getName());
                cw->set_anchor_name(protocol.getData());
                sw->hide();
                cw->show();
                 break;
            case Protocol::printf_client:
                aw->appendText(protocol.getData());
                cw->client_append(protocol.getData());
                break;


            default:
                break;
        }
    }
}
