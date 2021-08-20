#include "clientwidget.h"
#include "ui_clientwidget.h"

clientwidget::clientwidget(QTcpSocket *client_socket, QWidget *parent) : QWidget(parent),
                                                                         ui(new Ui::clientwidget),
                                                                         client_socket(client_socket)
{
    ui->setupUi(this);
    QHostAddress groupAddress;
    client_udp = new QUdpSocket(this);
    client_udp->bind(8888, QUdpSocket::ReuseAddressHint);
    client_udp->joinMulticastGroup(groupAddress);

    sender = new QUdpSocket(this);
    receiver = new QUdpSocket(this);

    groupAddress.setAddress("239.88.88.88");
    QObject::connect(client_udp, SIGNAL(readyRead()), this, SLOT(onUdpRead()));
    QObject::connect(client_udp, SIGNAL(readyRead()), this, SLOT(onReadyreadaudio()));

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    audioInput = new QAudioInput(format);
    audioOutput = new QAudioOutput(format);
    audioOutputIODevice = audioOutput->start();
}

clientwidget::~clientwidget()
{
    delete ui;
}

void clientwidget::chatAppend(const QString &text)
{
    ui->chat_display->append(text);
}

QString clientwidget::getAnchorName() const
{
    return anchor_name;
}

void clientwidget::setAnchorName(const QString &value)
{
    anchor_name = value;
}

QString clientwidget::getClientName() const
{
    return client_name;
}

void clientwidget::setClientName(const QString &value)
{
    client_name = value;
}

void clientwidget::clientAppend(const QString &text)
{
    ui->client_display->clear();
    QStringList list = text.split("\n");
    for (auto it : list)
    {
        ui->client_display->append(it);
    }
}

void clientwidget::onClientExitClicked()
{
    Protocol protocol;
    protocol.setType(Protocol::clientQuit);
    protocol.setName(anchor_name);
    protocol.setData(client_name);
    client_socket->write(protocol.toArray());
    emit sig_cw_quit();
    this->close();
}

void clientwidget::onRefreshButtonClicked()
{
    Protocol protocol;
    protocol.setName(anchor_name);
    protocol.setType(Protocol::refreshClient);
    client_socket->write(protocol.toArray());
}

void clientwidget::onChatEnterReturnPressed()
{
    QString text = ui->chat_enter->text();
    ui->chat_enter->clear();
    Protocol protocol;
    protocol.setType(Protocol::chat);
    protocol.setName(anchor_name);
    protocol.setPwd(client_name);
    protocol.setData(text);
    client_socket->write(protocol.toArray());
}

void clientwidget::onUdpRead()
{
    static QByteArray *imageBytes = new QByteArray(2280000, 0);
    static QDataStream *dataStream = new QDataStream(imageBytes, QIODevice::ReadWrite);
    while (client_udp->hasPendingDatagrams())
    {
        ImagePackage pack;
        memset(&pack, 0, sizeof(ImagePackage));
        client_udp->readDatagram((char *)&pack, sizeof(ImagePackage));
        if (true == pack.isLastpack)
        {
            dataStream->writeRawData(pack.data, pack.packTaken);
            QImage image((uchar *)imageBytes->data(), pack.width, pack.height, pack.bytesPerline, QImage::Format_RGB32);
            QPixmap map = QPixmap::fromImage(image);
            map = map.scaled(ui->camera_show->size());
            ui->camera_show->setPixmap(map);
            dataStream->device()->seek(0);
        }
        else
        {
            dataStream->writeRawData(pack.data, pack.packTaken);
        }
    }
}

void clientwidget::onReadyreadaudio()
{
    AudioPackage pack;
    memset(&pack, 0, sizeof(AudioPackage));
    receiver->readDatagram((char *)&pack, sizeof(AudioPackage));
    audioOutputIODevice->write(pack.data, pack.datalen);
}

void clientwidget::captureDataFromDevice()
{
    AudioPackage pack;
    memset(&pack, 0, sizeof(AudioPackage));
    pack.datalen = audioInputIODevice->read(pack.data, 1024);
    sender->writeDatagram((char *)&pack, sizeof(AudioPackage), QHostAddress::Broadcast, 8888);
    audioOutputIODevice->write(pack.data, pack.datalen);
}

void clientwidget::closeEvent(QCloseEvent *event)
{
    onClientExitClicked();
    event->accept();
}

void clientwidget::onMicrophoneOpenButtonClicked()
{
    if (ui->microphone_open_button->isChecked())
    {
        audioInputIODevice = audioInput->start();
        QObject::connect(audioInputIODevice, SIGNAL(readyRead()), this, SLOT(captureDataFromDevice()));
    }
    else
    {
        audioInput->stop();
    }
}

void clientwidget::onMicrophoneCloseButtonClicked()
{
    audioInput->stop();
}
