#ifndef ANCHOR_H
#define ANCHOR_H

#include <QWidget>
#include<QTcpSocket>
#include <Qstring>
#include"videosurface.h"
#include<QCamera>
#include<QUdpSocket>
#include<QTimer>
#include"pack.h"
#include<QAudioFormat>
#include<QAudioInput>
#include<QAudioOutput>
#include<QIODevice>

namespace Ui {
class anchor;
}

class anchor : public QWidget
{
    Q_OBJECT

public:
    explicit anchor(QTcpSocket* anchor_socket,QWidget *parent = 0);

    QString getName()const;
    void setName(const QString &value);
    void appendText(const QString& text);
    void chatText(const QString& text);

    ~anchor();

private slots:
    void on_refresh_button_clicked();

    void on_anchor_exit_clicked();

    void on_enter_chat_returnPressed();

    void on_camera_open_button_clicked();

    void on_camera_close_button_clicked();

    void vedioChangedSlot(QVideoFrame curFrame);

    void onTimeout();

    void on_microphone_open_button_clicked();

    void on_microphone_close_button_clicked();

    void onReadyreadaudio();

    void capturedatafromdevice();

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void sig_aw_quit();

private:
    QString name;
    Ui::anchor *ui;
    QTcpSocket* anchor_socket;

    QCamera* camera;
    QUdpSocket* anchor_udp;
    QUdpSocket* sender;
    QUdpSocket* receiver;


    QTimer* timer;

    QAudioInput* audioInput;
    QAudioOutput* audioOutput;

    QIODevice* audioOutputIODevice;
    QIODevice* audioInputIODevice;
};

#endif // ANCHOR_H

