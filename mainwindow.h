#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileInfo>
#include <QVideoWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVideoWidget* getVideo();

public slots:
    void OnNewFramesCount (qint64);
    void OnExtractedImage (QImage,QImage);
    void OnImageReconstructed (QImage);

signals:
    void NewFileOpenRequested(QFileInfo);
    void NewVideoPosition (double);
    void CaptureFrames (void);

private:
    Ui::MainWindow *ui;

private slots:
    void onFileEventClicked(void);
    void onNewSliderPosition (int);
    void on_pushButton_2_clicked();
    void on_pushButtonOpenFile_clicked();
    void onFileOpenRequested ();
};

#endif // MAINWINDOW_H
