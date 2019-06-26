#ifndef FRAMESSINK_H
#define FRAMESSINK_H

#include "algorithm.h"

#include <QAbstractVideoSurface>
#include <vector>

class FramesSink : public QAbstractVideoSurface
{
    Q_OBJECT
public:
    FramesSink();

    std::vector<unsigned char> baseFrame;
    std::vector<unsigned char> futureFrame;

    bool present(const QVideoFrame &frame) override;
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(
            QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const override;
    QSize imageSize;

    void reconstructImage(const std::vector<point> p, unsigned int height, unsigned int width);
signals:
    void ImagesCaptured (QImage, QImage);
    void ImageReconstructed(QImage);

private:
    static QImage frameToImage (const std::vector<unsigned char> data , QSize size );
    unsigned int _imageCount = 0;
};

#endif // FRAMESSINK_H
