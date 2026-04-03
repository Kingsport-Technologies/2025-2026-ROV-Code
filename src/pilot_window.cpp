#include "pilot_window.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QVideoWidget>

PilotWindow::PilotWindow(QWidget* parent)
{
    setWindowTitle("Dreamer Pilot");
    setMinimumSize(900, 600);
    resize(1100, 680);

    buildUi();
}

void PilotWindow::buildUi()
{
    QWidget* mainWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(mainWidget);
    QLabel* placeholder = new QLabel("Hello World");
    QMediaPlayer *player = new QMediaPlayer(this);
    QVideoWidget *videoWidget = new QVideoWidget(this);

    player->setSource(QUrl(
    "gst-pipeline://"
    "rtspsrc location=rtsp://127.0.0.1:8554/rov-camera latency=0 ! "
    "rtph264depay ! h264parse ! avdec_h264 ! "
    "videoconvert ! "
    "qtvideosink"  // <-- Qt-aware sink, feeds directly into QVideoWidget
    ));
    player->setVideoOutput(videoWidget);
    player->play();
    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    videoWidget->setMinimumSize(640, 480);
    layout->addWidget(placeholder);
    layout->addWidget(videoWidget);
}
