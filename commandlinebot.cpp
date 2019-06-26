#include "commandlinebot.h"
#include <QDebug>


CommandLineBot::CommandLineBot(QApplication *app) : QObject (app)
{
    _parser.setApplicationDescription("Test helper");
    _parser.addHelpOption();
    _parser.addVersionOption();

    _parser.addOption(_videoSourceOption);
    _parser.addOption(_videoPositionOption);
    _parser.addOption(_videoOffsetPositionOption);

    _parser.process(*app);
    if (_parser.isSet(_videoSourceOption))
    {
        _targetDir = _parser.value(_videoSourceOption);
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << _targetDir ;
    }

    if (_parser.isSet(_videoPositionOption))
    {
        _position = _parser.value(_videoPositionOption).toInt();
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << _position ;
    }

    if (_parser.isSet(_videoOffsetPositionOption))
    {
        _offset = _parser.value(_videoOffsetPositionOption).toInt();
        qDebug() << __FILE__ << __FUNCTION__ << __LINE__ << _offset ;
    }

}

CommandLineBot::~CommandLineBot()
{

}

void CommandLineBot::start()
{
    if (!_targetDir.isEmpty())
        emit (openFile(QFileInfo(_targetDir)));
}

void CommandLineBot::OnFileOpenned()
{
    if (_parser.isSet(_videoPositionOption))
        emit (setPostion (_position));
    if (_parser.isSet(_videoPositionOption)) {
        this->_tmr.setSingleShot(true);
        this->_tmr.callOnTimeout(this, &CommandLineBot::onTimerEpriredForExtractFrames);
        this->_tmr.start(1000);
    }
}

void CommandLineBot::OnPostionSetted()
{

}

void CommandLineBot::OnFrameExtracted()
{
   if (_parser.isSet(_videoPositionOption))
       emit (processFrames());
}

void CommandLineBot::OnFramesProcessed()
{

}

void CommandLineBot::onTimerEpriredForExtractFrames()
{
    emit(extractFrames());
}
