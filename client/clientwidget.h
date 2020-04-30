#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include<QTcpSocket>
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

namespace Ui {
class clientwidget;
}

class clientwidget : public QWidget
{
    Q_OBJECT

public:
    explicit clientwidget(QTcpSocket* client_socket,QWidget *parent = 0);
    ~clientwidget();

    void chat_append(const QString& text);

    QString get_anchor_name() const;

    void set_anchor_name(const QString &value);

    QString get_client_name() const;

    void set_client_name(const QString &value);

    void client_append(const QString& text);

private slots:
    void on_client_exit_clicked();

    void on_refresh_button_clicked();

    void on_chat_enter_returnPressed();

    void on_udp_read();

    void onReadyreadaudio();

    void capturedatafromdevice();

    void on_microphone_open_button_clicked();

    void on_microphone_close_button_clicked();

private:
    Ui::clientwidget *ui;
    QTcpSocket* client_socket;
    QString anchor_name;
    QString client_name;
    QUdpSocket* client_udp;
    QUdpSocket* sender;
    QUdpSocket* receiver;
    QAudioInput* audioInput;
    QAudioOutput* audioOutput;
    QIODevice* audioOutputIODevice;
    QIODevice* audioInputIODevice;

protected:
    void closeEvent(QCloseEvent* event);

signals:
    void sig_cw_quit();
};

#endif // CLIENTWIDGET_H

