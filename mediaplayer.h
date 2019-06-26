#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QMediaPlayer>
#include <QFileInfo>

#include "framessink.h"

class MediaPlayer : public QMediaPlayer
{
    Q_OBJECT
public:
    MediaPlayer();

signals:
    void NewFileOpened();
    void newDuratationMsCount (qint64);
    void FramesCaputred (QImage,QImage);
    void ImageReconstructed(QImage);

public slots:
    void OnNewFile (QFileInfo);
    void OnNewPosition (double);
    void OnNewPosition (int);
    void OnCaptureFrames ();
    void OnProcessFrames();

private:
    FramesSink _sink;
    QVideoWidget* _stashedVideo;

private slots:
    void onMediaPlayerStateChanged (QMediaPlayer::State);
    void onCapturedImgaes (QImage,QImage);
};

#endif // MEDIAPLAYER_H
