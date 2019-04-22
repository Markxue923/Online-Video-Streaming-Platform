#include "subwidget.h"
#include "ui_subwidget.h"
#include "protocol.h"
#include <QListWidget>
#include <QListView>
#include <QTcpSocket>
#include <QDebug>

SubWidget::SubWidget(QTcpSocket* socket,QWidget *parent):
    QWidget(parent),
    ui(new Ui::SubWidget),
    socket(socket)
{
    ui->setupUi(this);
}

SubWidget::~SubWidget()
{
    delete ui;
}

QString SubWidget::getName()const
{
    return name;
}

void SubWidget::setName(const QString &value)
{
    name = value;
}

void SubWidget::appendtext(const QString &text)
{
    ui->room_show->clear();
    QStringList list = text.split("\n");

    for(auto it : list){
        ui->room_show->addItem(new QListWidgetItem(it));
    }
    ui->room_show->sortItems();
}


void SubWidget::closeEvent(QCloseEvent *event)
{
    on_exit_Button_clicked();
    event->accept();
}

void SubWidget::on_create_room_Button_clicked()
{
    qDebug()<<"creating";
    Protocol protocol;
    protocol.setName(name);
    protocol.setType(Protocol::create_room);
    socket->write(protocol.toArray());
}

void SubWidget::on_refresh_Button_clicked()
{
    Protocol protocol;
    protocol.setType(Protocol::refresh_room);
    socket->write(protocol.toArray());
}


void SubWidget::on_room_show_itemDoubleClicked(QListWidgetItem *item)
{
    qDebug()<<item->text();
    Protocol protocol;
    protocol.setName(name);
    protocol.setType(Protocol::enter_anchor);
    protocol.setData(item->text());
    socket->write(protocol.toArray());
}

void SubWidget::on_exit_Button_clicked()
{
    Protocol protocol;
    protocol.setType(Protocol::quit);
    protocol.setName(name);

    socket->write(protocol.toArray());

    emit sig_sw_quit();
    this->close();
}



