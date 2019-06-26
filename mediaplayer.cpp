#include "mediaplayer.h"
#include "algorithm.h"

#include <QImage>


MediaPlayer::MediaPlayer()
{
    connect (&_sink, &FramesSink::ImageReconstructed,this, &MediaPlayer::ImageReconstructed);
}

void MediaPlayer::OnNewFile(QFileInfo f)
{
    qDebug() << __LINE__ << "atempt to play file: " << f.absoluteFilePath();
    setMedia(QUrl::fromLocalFile(f.absoluteFilePath()));
    setPlaybackRate(0.1);
    play();
    emit newDuratationMsCount(duration());
    emit NewFileOpened();
}

void MediaPlayer::OnNewPosition(double value)
{
    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__ << "position is " << duration() * value;
    setPosition(static_cast<int>(duration() * value));
}

void MediaPlayer::OnNewPosition (int value)
{
    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__ << "position is " << value;
    setPosition(static_cast<int>(value));
}

void MediaPlayer::OnCaptureFrames()
{
    qint64 pos = position();
    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__ << pos ;
    pause();
    setPosition(pos);

    setVideoOutput(&_sink);
    setPosition(pos);
    connect (&_sink,&FramesSink::ImagesCaptured, this, &MediaPlayer::onCapturedImgaes,Qt::QueuedConnection);
    play();
    pause();
    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__ << pos ;
    setPosition(pos);
    play();
    pos = position();
    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__ << pos ;

}

void MediaPlayer::OnProcessFrames()
{
    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__ ;

    unsigned int height =   static_cast<unsigned int> (_sink.imageSize.height());
    unsigned int width = static_cast<unsigned int> (_sink.imageSize.width());

    std::vector<point> p_vec = getMove(this->_sink.baseFrame.data(), this->_sink.futureFrame.data(),height, width);

    _sink.reconstructImage(p_vec,height, width);
}

void MediaPlayer::onMediaPlayerStateChanged(QMediaPlayer::State)
{

}

void MediaPlayer::onCapturedImgaes(QImage img1,QImage img2)
{
    qDebug() << __FILE__ << ":" <<  __LINE__ << __FUNCTION__ ;
    stop();
    emit FramesCaputred(img1,img2);
}
