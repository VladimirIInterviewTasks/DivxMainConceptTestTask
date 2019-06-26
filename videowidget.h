#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QVideoWidget>
#include <QWidget>

class VideoWidget : public QVideoWidget
{
public:
    VideoWidget(QWidget* wid): QVideoWidget(wid){}
};

#endif // VIDEOWIDGET_H
