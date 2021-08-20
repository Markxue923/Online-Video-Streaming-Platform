#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include "protocol.h"
#include <QCloseEvent>
#include <QString>
#include <QUdpSocket>
#include <QTimer>
#include "pack.h"
#include "videosurface.h"
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>

namespace Ui
{
    class clientwidget;
}

class clientwidget : public QWidget
{
    Q_OBJECT

public:
    explicit clientwidget(QTcpSocket *client_socket, QWidget *parent = 0);
    ~clientwidget();

    void chatAppend(const QString &text);

    QString getAnchorName() const;

    void setAnchorName(const QString &value);

    QString getClientName() const;

    void setClientName(const QString &value);

    void clientAppend(const QString &text);

private slots:
    void onClientExitClicked();

    void onRefreshButtonClicked();

    void onChatEnterReturnPressed();

    void onUdpRead();

    void onReadyreadaudio();

    void captureDataFromDevice();

    void onMicrophoneOpenButtonClicked();

    void onMicrophoneCloseButtonClicked();

private:
    Ui::clientwidget *ui;
    QTcpSocket *client_socket;
    QString anchor_name;
    QString client_name;
    QUdpSocket *client_udp;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    QAudioInput *audioInput;
    QAudioOutput *audioOutput;
    QIODevice *audioOutputIODevice;
    QIODevice *audioInputIODevice;

protected:
    void closeEvent(QCloseEvent *event);

signals:
    void sig_cw_quit();
};

#endif // CLIENTWIDGET_H
