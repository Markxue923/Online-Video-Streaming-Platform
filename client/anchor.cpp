#include "anchor.h"
#include "ui_anchor.h"
#include "protocol.h"
#include <QCloseEvent>
#include <QStringList>

anchor::anchor(QTcpSocket* anchor_socket,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::anchor),
    anchor_socket(anchor_socket)
{
    ui->setupUi(this);
    anchor_udp=new QUdpSocket(this);

    sender=new QUdpSocket(this);
    receiver=new QUdpSocket(this);


    QObject::connect(timer,SIGNAL(timeout()),this,SLOT(onTimeout()));
   // QCameraInfo info = QCameraInfo::defaultCamera();
    camera=new QCamera(this);
    VideoSurface* vf= new VideoSurface(this);
    camera->setViewfinder(vf);
    QObject::connect(vf,SIGNAL(videoChanged(QVideoFrame)),this,SLOT(vedioChangedSlot(QVideoFrame)));
    QObject::connect(anchor_udp,SIGNAL(readyRead()),this,SLOT(onReadyreadaudio()));

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);
    audioInput=new QAudioInput(format);
    audioOutput=new QAudioOutput(format);
    audioOutputIODevice=audioOutput->start();

}


QString anchor::getName() const
{
    return name;
}

void anchor::setName(const QString &value)
{
    name =value;
}

void anchor::appendText(const QString &text)
{
    ui->display_anchor->clear();
    QStringList list=text.split("\n");
    for(auto it : list)
    {
        ui->display_anchor->append(it);
    }
}

void anchor::chatText(const QString &text)
{
    ui->display_chat->append(text);
}

anchor::~anchor()
{
    delete ui;
}



void anchor::on_refresh_button_clicked()
{
    Protocol protocol;
    protocol.setName(name);
    protocol.setType(Protocol::refresh_client);
    anchor_socket->write(protocol.toArray());
}

void anchor::closeEvent(QCloseEvent *event)
{
    on_anchor_exit_clicked();
    event->accept();
}

void anchor::on_anchor_exit_clicked()
{
    Protocol protocol;
    protocol.setType(Protocol::anchor_quit);
    protocol.setName(name);
    anchor_socket->write(protocol.toArray());
    emit sig_aw_quit();
    this->close();
}

void anchor::on_enter_chat_returnPressed()
{
    QString text= ui->enter_chat->text();
    ui->enter_chat->clear();

    Protocol protocol;
    protocol.setType(Protocol::chat);
    protocol.setName(name);
    protocol.setPwd(name);
    protocol.setData(text);
    anchor_socket->write(protocol.toArray());
}

void anchor::on_camera_open_button_clicked()
{
    camera->start();
}

void anchor::on_camera_close_button_clicked()
{
    camera->stop();
}

void anchor::vedioChangedSlot(QVideoFrame curFrame)
{
    static int i=0;
    i++;
    int unitBytes=4096;
    QVideoFrame frame(curFrame);
    frame.map(QAbstractVideoBuffer::ReadOnly);
    QImage image(frame.bits(),frame.width(),frame.height(),QVideoFrame::imageFormatFromPixelFormat(frame.pixelFormat()));
    QMatrix matrix;
    matrix.rotate(180);
    image=image.transformed(matrix);
    QPixmap pixmap=QPixmap::fromImage(image);
    pixmap=pixmap.scaled(ui->camera_show->size());
    ui->camera_show->setPixmap(pixmap);
    if(i%4!=0)
    {
        return;
    }
    int byteCount=image.byteCount();
    int width=image.width();
    int height=image.height();
    int bytePerLine=image.bytesPerLine();
    int writeByte=0;
    while(true)
    {
        ImagePackage pack;
        memset(&pack,0,sizeof(ImagePackage));
        pack.width=width;
        pack.height=height;
        pack.bytesPerline=bytePerLine;
        if(writeByte<byteCount)
        {
            memcpy(pack.data,(char*)image.bits()+writeByte,unitBytes);
            pack.packTaken=unitBytes;
            writeByte+=unitBytes;
            if(writeByte>=byteCount)
            {
                pack.isLastpack=true;
                pack.packTaken=unitBytes-writeByte+byteCount;
            }else
            {
                pack.isLastpack=false;
                pack.packTaken=unitBytes;
            }
            anchor_udp->writeDatagram((char*)&pack,sizeof(ImagePackage),QHostAddress::Broadcast,8888);
        }else
        {
            break;
        }
    }
}

void anchor::onTimeout()
{
    QString str=ui->udp_anchor->text();
    char data[24]="";
    strcpy(data,str.toStdString().data());
    anchor_udp->writeDatagram(data,20,QHostAddress::Broadcast,8888);
}

void anchor::on_microphone_open_button_clicked()
{
    if(ui->microphone_open_button->isChecked())
    {
        audioInputIODevice=audioInput->start();
        QObject::connect(audioInputIODevice,SIGNAL(readyRead()),this,SLOT(capturedatafromdevice()));
    }else
    {
        audioInput->stop();
    }
}

void anchor::on_microphone_close_button_clicked()
{
    audioInput->stop();
}

void anchor::onReadyreadaudio()
{
    AudioPackage pack;
    memset(&pack,0,sizeof(AudioPackage));
    receiver->readDatagram((char*)&pack,sizeof(AudioPackage));
    audioOutputIODevice->write(pack.data,pack.datalen);
}

void anchor::capturedatafromdevice()
{
    AudioPackage pack;
    memset(&pack,0,sizeof(AudioPackage));
    pack.datalen=audioInputIODevice->read(pack.data,1024);
    sender->writeDatagram((char*)&pack,sizeof(AudioPackage),QHostAddress::Broadcast,8888);
    audioOutputIODevice->write(pack.data,pack.datalen);
}
