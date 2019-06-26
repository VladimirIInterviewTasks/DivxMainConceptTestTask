#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

   QObject::connect(ui->actionOpen,&QAction::triggered, this, &MainWindow::onFileEventClicked);
   QObject::connect(ui->horizontalSlider,&QSlider::valueChanged, this, &MainWindow::onNewSliderPosition);

}

MainWindow::~MainWindow()
{
    delete ui;
}

QVideoWidget *MainWindow::getVideo()
{
    return ui->widget;
}

void MainWindow::OnNewFramesCount(qint64 count)
{
    (void)count;
}

void MainWindow::OnExtractedImage(QImage base,QImage future)
{
    ui->tabWidget->setCurrentWidget(ui->tab_2);
    ui->label_baseImage->setPixmap(QPixmap::fromImage(base));
    ui->label_futureImage->setPixmap(QPixmap::fromImage(future));

}

void MainWindow::OnImageReconstructed(QImage reconstructed)
{
    ui->label_reconstructed->setPixmap(QPixmap::fromImage(reconstructed));
}

void MainWindow::onFileEventClicked()
{
    onFileOpenRequested();
}

void MainWindow::onNewSliderPosition(int pos)
{
   emit(NewVideoPosition((pos*1.0)/ui->horizontalSlider->maximum()));
}

void MainWindow::on_pushButton_2_clicked()
{
    emit  CaptureFrames ();

}

void MainWindow::on_pushButtonOpenFile_clicked()
{
    onFileOpenRequested();
}

void MainWindow::onFileOpenRequested()
{
    QFileDialog dialog (this);
    dialog.setModal(true);

    if (  dialog.exec() == QDialog::Accepted)
    {
        emit NewFileOpenRequested(QFileInfo(dialog.selectedFiles().at(0)));
    }

}
