#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QUdpSocket>
#include "subwidget.h"
#include "anchor.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:
    void on_btnRegist_clicked();
    void on_btnLogin_clicked();
    void onReadyRead();

private:
    Ui::Widget *ui;
    QTcpSocket* socket;
    SubWidget* sw;
    anchor* aw;
    clientwidget* cw;
};

#endif // WIDGET_H
