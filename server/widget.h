#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>

namespace Ui
{
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpServer *server;

private slots:
    void onNewConnection();
    void onSigSend(QTcpSocket *socket, QByteArray arr);
};

#endif // WIDGET_H
