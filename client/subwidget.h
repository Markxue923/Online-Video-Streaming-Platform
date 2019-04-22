#ifndef SUBWIDGET_H
#define SUBWIDGET_H
#include "anchor.h"
#include "clientwidget.h"
#include "protocol.h"
#include <QWidget>
#include <QTcpSocket>
#include <QCloseEvent>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidgetItem>


namespace Ui {
class SubWidget;
}

class SubWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubWidget(QTcpSocket* socket,QWidget *parent = 0);
    ~SubWidget();

    QString getName() const;
    void setName(const QString &value);
    void appendtext(const QString& text);

private:

    Ui::SubWidget *ui;
    QString name;
    QTcpSocket* socket;


signals:
    void sig_sw_quit();
    void sig_create_room();

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void on_create_room_Button_clicked();
    void on_refresh_Button_clicked();
    void on_exit_Button_clicked();
    void on_room_show_itemDoubleClicked(QListWidgetItem *item);
};

#endif // SUBWIDGET_H
