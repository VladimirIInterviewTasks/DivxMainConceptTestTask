#include "framessink.h"

#include <QDebug>
#include <array>

FramesSink::FramesSink()
{

}

bool FramesSink::present(const QVideoFrame &frame)
{

    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__;

    if (frame.pixelFormat() == QVideoFrame::Format_Invalid)
        return false;
    if (_imageCount == 0)
    {
        const_cast<QVideoFrame&>(frame).map(QAbstractVideoBuffer::ReadOnly);
        imageSize = frame.size();
        this->baseFrame = std::vector<unsigned char> (frame.bits(), frame.bits()+frame.mappedBytes());
        const_cast<QVideoFrame&>(frame).unmap();
    }


    if (_imageCount == 2 )
    {
        const_cast<QVideoFrame&>(frame).map(QAbstractVideoBuffer::ReadOnly);
        this->futureFrame = std::vector<unsigned char> (frame.bits(), frame.bits()+frame.mappedBytes());;
        //_imageCount = 0;
        emit ImagesCaptured(frameToImage(baseFrame,frame.size()),frameToImage(futureFrame,frame.size()));
    }
    else
    {
        _imageCount++;
    }

    return true;
}

QList<QVideoFrame::PixelFormat> FramesSink::supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const
{
    (void)type;
    return QList<QVideoFrame::PixelFormat>{QVideoFrame::Format_YV12};
}

void FramesSink::reconstructImage(const std::vector<point> p, unsigned int height, unsigned int width)
{
    std::vector <unsigned char > data;
    data.resize(p.size()*8*12,0);

    memset(data.data() , 0 , p.size()*8*12);

    for (unsigned int i = 0; i <  p.size() ; ++i )
    {

        unsigned int currentX = (i * 8) % width;
        unsigned int currentY = ((i * 8) / width)*8;

        for (unsigned int j = 0 ; j < 8 ; j++)
            for (unsigned int t  = 0; t < 8 ; t++)
            {
                {
                    unsigned int dstX = currentX+t;
                    unsigned int dstY = currentY+j;
                    unsigned int dstOffset = dstX + dstY * width;

                    unsigned int srcX = p[i].x + t;
                    unsigned int srcY = p[i].y + j;
                    unsigned int srcOffset = srcX + srcY* width;

                    data[dstOffset] = this->baseFrame[srcOffset];
                }

                {
                    unsigned int dstX = currentX+t;
                    unsigned int dstY = currentY+j;
                    unsigned int dstOffset = (dstX/2) + (dstY/2 * width/2);

                    unsigned int srcX = p[i].x + t;
                    unsigned int srcY = p[i].y + j;
                    unsigned int srcOffset = (srcX/2) + (srcY/2 * width/2);

                    data[dstOffset+p.size()*8*8] = this->baseFrame[srcOffset+p.size()*8*8];
                }

                {
                    unsigned int dstX = currentX+t;
                    unsigned int dstY = currentY+j;
                    unsigned int dstOffset = (dstX/2) + (dstY/2 * width/2);

                    unsigned int srcX = p[i].x + t;
                    unsigned int srcY = p[i].y + j;
                    unsigned int srcOffset = (srcX/2) + (srcY/2 * width/2);

                    data[dstOffset+p.size()*8*10] = this->baseFrame[srcOffset+p.size()*8*10];
                }


            }
    }
    emit ImageReconstructed(frameToImage(data,QSize(static_cast<int>(width), static_cast<int>(height))));

}


std::array <unsigned char,3> convertYUVtoRGB(unsigned char y, unsigned char u, unsigned char v) {
    int r,g,b;
    int y1 = static_cast<int> (y);
    int u1 = static_cast<int>(u)-128;
    int v1 = static_cast<int>(v)-128;

    r = static_cast<int>(y1 + 1.402f*v1);
    g = static_cast<int>(y1 - 0.344f*u1 -0.714f*v1);
    b = static_cast<int>(y1 + 1.772f*u1);

    r = r>255? 255 : r<0 ? 0 : r;
    g = g>255? 255 : g<0 ? 0 : g;
    b = b>255? 255 : b<0 ? 0 : b;
    return {static_cast<unsigned char>(r)
                ,static_cast<unsigned char>(g),
                static_cast<unsigned char>(b)};
}



QImage FramesSink::frameToImage(const std::vector<unsigned char> data, QSize sz)
{
    QImage img(sz,QImage::Format::Format_RGB32);

    unsigned int width = static_cast<unsigned int> (sz.width());
    unsigned int height = static_cast<unsigned int> (sz.height());
    unsigned int size = width*height;
    unsigned int offset = size;
    unsigned char u, v, y1, y2, y3, y4;

    for(unsigned int i=0, k=0; i < size; i+=2, k+=1) {
        y1 = data[i  ]&0xff;
        y2 = data[i+1]&0xff;
        y3 = data[width+i  ]&0xff;
        y4 = data[width+i+1]&0xff;

        v = data[offset+k  ]&0xff;
        u = data[offset+k+size/4]&0xff;
        //u = u-128;
        //v = v-128;

        //u = 0;
        //v = 0;


        {
            std::array <unsigned char,3> rgb = convertYUVtoRGB(y1, u, v);
            img.setPixel(static_cast<int> (i%width),
                         static_cast<int> (i/width),
                         qRgb(rgb.at(0),rgb.at(1),rgb.at(2)));
        }

        {
            std::array <unsigned char,3> rgb = convertYUVtoRGB(y2, u, v);
            img.setPixel(static_cast<int>((i+1)%width),
                         static_cast<int>((i+1)/width),
                         qRgb(rgb.at(0),rgb.at(1),rgb.at(2)));
        }

        {
            std::array <unsigned char,3> rgb = convertYUVtoRGB(y3, u, v);
            img.setPixel(static_cast<int>(i%width),
                         static_cast<int>((i+1+width)/width),
                         qRgb(rgb.at(0),rgb.at(1),rgb.at(2)));
        }

        {
            std::array <unsigned char,3> rgb = convertYUVtoRGB(y4, u, v);
            img.setPixel(static_cast<int>((i+width+1)%width),
                         static_cast<int>((i+1+width)/width),
                         qRgb(rgb.at(0),rgb.at(1),rgb.at(2)));
        }

        if (i!=0 && (i+2)%width==0)
            i+=width;
    }

    return img;
}
