#include "videosurface.h"

VideoSurface::VideoSurface(QObject *parent)
    : QAbstractVideoSurface(parent)
{
}

QList<QVideoFrame::PixelFormat> VideoSurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handletype) const
{
    return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
}

bool VideoSurface::present(const QVideoFrame &frame)
{
    emit videoChanged(frame);
    return true;
}
