#include "mainwindow.h"
#include "mediaplayer.h"
#include "commandlinebot.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QtGlobal>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qDebug().setVerbosity(7);

    CommandLineBot b(&a);
    MediaPlayer m;
    MainWindow w;


    QObject::connect(&w, &MainWindow::NewFileOpenRequested, &m, &MediaPlayer::OnNewFile,Qt::QueuedConnection);
    QObject::connect(&b, &CommandLineBot::openFile, &m, &MediaPlayer::OnNewFile,Qt::QueuedConnection);
    QObject::connect(&m, &MediaPlayer::NewFileOpened, &b, &CommandLineBot::OnFileOpenned,Qt::QueuedConnection);

    QObject::connect(&w, &MainWindow::NewVideoPosition, &m,  QOverload<double>::of(&MediaPlayer::OnNewPosition),Qt::QueuedConnection);
    QObject::connect(&b, &CommandLineBot::setPostion, &m, QOverload<int>::of(&MediaPlayer::OnNewPosition),Qt::QueuedConnection);

    QObject::connect(&w, &MainWindow::CaptureFrames, &m, &MediaPlayer::OnCaptureFrames,Qt::QueuedConnection);
    QObject::connect(&b, &CommandLineBot::extractFrames, &m, &MediaPlayer::OnCaptureFrames,Qt::QueuedConnection);

    QObject::connect(&m, &MediaPlayer::FramesCaputred, &w, &MainWindow::OnExtractedImage);
    QObject::connect(&m, &MediaPlayer::FramesCaputred, &b, &CommandLineBot::OnFrameExtracted);

    QObject::connect(&b, &CommandLineBot::processFrames, &m, &MediaPlayer::OnProcessFrames,Qt::QueuedConnection);

    QObject::connect(&m,&MediaPlayer::ImageReconstructed, &w, &MainWindow::OnImageReconstructed);




    m.setVideoOutput(w.getVideo());


    b.start();

    w.show();

    return a.exec();
}
