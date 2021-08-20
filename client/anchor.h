#ifndef ANCHOR_H
#define ANCHOR_H

#include <QWidget>
#include <QTcpSocket>
#include <Qstring>
#include <QCamera>
#include <QUdpSocket>
#include <QTimer>
#include <QAudioFormat>
#include <QAudioInput>
#include <QAudioOutput>
#include <QIODevice>
#include "videosurface.h"
#include "pack.h"

namespace Ui
{
    class anchor;
}

class anchor : public QWidget
{
    Q_OBJECT

public:
    explicit anchor(QTcpSocket *anchor_socket, QWidget *parent = 0);

    QString getName() const;
    void setName(const QString &value);
    void appendText(const QString &text);
    void chatText(const QString &text);

    ~anchor();

private slots:
    void onRefreshButtonClicked();

    void onAnchorExitClicked();

    void onEnterChatReturnPressed();

    void onCameraOpenButtonClicked();

    void onCameraCloseButtonClicked();

    void vedioChangedSlot(QVideoFrame curFrame);

    void onTimeout();

    void onMicrophoneOpenButtonClicked();

    void onMicrophoneCloseButtonClicked();

    void onReadyreadaudio();

    void captureDataFromDevice();

protected:
    virtual void closeEvent(QCloseEvent *event);

signals:
    void sigAwQuit();

private:
    QString name;
    Ui::anchor *ui;
    QTcpSocket *anchor_socket;
    QCamera *camera;
    QUdpSocket *anchor_udp;
    QUdpSocket *sender;
    QUdpSocket *receiver;
    QTimer *timer;
    QAudioInput *audioInput;
    QAudioOutput *audioOutput;
    QIODevice *audioOutputIODevice;
    QIODevice *audioInputIODevice;
};

#endif // ANCHOR_H
