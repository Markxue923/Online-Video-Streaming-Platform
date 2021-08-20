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

namespace Ui
{
    class SubWidget;
}

class SubWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SubWidget(QTcpSocket *socket, QWidget *parent = 0);
    ~SubWidget();

    QString getName() const;
    void setName(const QString &value);
    void appendText(const QString &text);

private:
    Ui::SubWidget *ui;
    QString name;
    QTcpSocket *socket;

signals:
    void sigSwQuit();
    void sigCreateRoom();

protected:
    virtual void closeEvent(QCloseEvent *event);

private slots:
    void onCreateRoomButtonClicked();
    void onRefreshButtonClicked();
    void onExitButtonClicked();
    void onRoomShowItemDoubleClicked(QListWidgetItem *item);
};

#endif // SUBWIDGET_H
