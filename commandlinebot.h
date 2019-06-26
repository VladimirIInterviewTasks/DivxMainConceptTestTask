#ifndef COMMANDLINEBOT_H
#define COMMANDLINEBOT_H

#include <QApplication>
#include <QCommandLineParser>
#include <QFileInfo>
#include <QTimer>

class CommandLineBot : public QObject
{
    Q_OBJECT
public:
    explicit CommandLineBot(QApplication *app);
    ~CommandLineBot();

    void start();

public slots:
    void OnFileOpenned();
    void OnPostionSetted();
    void OnFrameExtracted();
    void OnFramesProcessed();

signals:
    void openFile(QFileInfo);
    void setPostion (int ms);
    void extractFrames (void);
    void processFrames (void);

private slots:
    void onTimerEpriredForExtractFrames (void);

private:
    QCommandLineParser _parser;
    QTimer _tmr;

    QCommandLineOption _videoSourceOption{"s", QCoreApplication::translate("source", "video source"),"source"};
    QCommandLineOption _videoPositionOption{"p", QCoreApplication::translate("position", "position"), "0"};
    QCommandLineOption _videoOffsetPositionOption{"o", QCoreApplication::translate("offset", "offset of next frame"),"0"};

    QString _targetDir;
    int _position;
    int _offset;
};

#endif // COMMANDLINEBOT_H
