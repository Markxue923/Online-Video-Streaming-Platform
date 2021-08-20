#ifndef VIDEOSURFACE_H
#define VIDEOSURFACE_H

#include <QObject>
#include <QAbstractVideoSurface>

class VideoSurface : public QAbstractVideoSurface
{
    Q_OBJECT

signals:
    void videoChanged(QVideoFrame);

public:
    VideoSurface(QObject *parent = 0);
    virtual QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType handletype) const;
    bool present(const QVideoFrame &frame);
};

#endif // VIDEOSURFACE_H
