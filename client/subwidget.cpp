#include "subwidget.h"
#include "ui_subwidget.h"
#include "protocol.h"
#include <QListWidget>
#include <QListView>
#include <QTcpSocket>
#include <QDebug>

SubWidget::SubWidget(QTcpSocket *socket, QWidget *parent) : QWidget(parent),
                                                            ui(new Ui::SubWidget),
                                                            socket(socket)
{
    ui->setupUi(this);
}

SubWidget::~SubWidget()
{
    delete ui;
}

QString SubWidget::getName() const
{
    return name;
}

void SubWidget::setName(const QString &value)
{
    name = value;
}

void SubWidget::appendText(const QString &text)
{
    ui->room_show->clear();
    QStringList list = text.split("\n");

    for (auto it : list)
    {
        ui->room_show->addItem(new QListWidgetItem(it));
    }
    ui->room_show->sortItems();
}

void SubWidget::closeEvent(QCloseEvent *event)
{
    onExitButtonClicked();
    event->accept();
}

void SubWidget::onCreateRoomButtonClicked()
{
    qDebug() << "creating";
    Protocol protocol;
    protocol.setName(name);
    protocol.setType(Protocol::createRoom);
    socket->write(protocol.toArray());
}

void SubWidget::onRefreshButtonClicked()
{
    Protocol protocol;
    protocol.setType(Protocol::refreshRoom);
    socket->write(protocol.toArray());
}

void SubWidget::onRoomShowItemDoubleClicked(QListWidgetItem *item)
{
    qDebug() << item->text();
    Protocol protocol;
    protocol.setName(name);
    protocol.setType(Protocol::enterAnchor);
    protocol.setData(item->text());
    socket->write(protocol.toArray());
}

void SubWidget::onExitButtonClicked()
{
    Protocol protocol;
    protocol.setType(Protocol::quit);
    protocol.setName(name);

    socket->write(protocol.toArray());

    emit sigSwQuit();
    this->close();
}
