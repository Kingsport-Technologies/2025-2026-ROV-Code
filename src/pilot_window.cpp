#include "pilot_window.hpp"
#include <QVBoxLayout>
#include <QLabel>
#include <QMediaPlayer>
#include <QVideoWidget>
#include "settingsDialog.hpp"
#include <QString>
#include <QGroupBox>
#include <QIcon>
#include <QFormLayout>
#include <QStyle>

PilotWindow::PilotWindow(ControllerInterface *contoller, outgoingserver *server, QWidget* parent)
{
    m_outgoingserver = server;
    m_contoller = contoller;
    setWindowTitle("Dreamer Pilot");
    setMinimumSize(900, 600);
    resize(1100, 680);
    m_settings = new QSettings("KTech", "2526Dreamer");
    buildUi();    
}

void PilotWindow::buildUi()
{
    settingsDialog* settings = new settingsDialog();
    
    settings->exec();
    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    QVBoxLayout* layout = new QVBoxLayout(mainWidget);
    m_videos = buildVideos();
    layout->addLayout(m_videos);

    connect(settings, &settingsDialog::submited, this, &PilotWindow::reloadVideos);
    QHBoxLayout* statusBoxes = new QHBoxLayout();

    QGroupBox* robotBox = new QGroupBox("Robot");
    QHBoxLayout* robotBoxLayout = new QHBoxLayout();
    m_robotIcon = new QLabel;
    m_robotIcon->setPixmap(QPixmap(":/icons/submarine_dark.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    robotBoxLayout->addWidget(m_robotIcon);

    QFormLayout* robotStatusLayout = new QFormLayout();
    m_robot_connected = new QLabel("Disconnected");
    m_robot_connected->setStyleSheet(R"(
        QLabel {
            color: red;
        }
    )");
    m_robot_ping = new QLabel("0.00 ms");
    connect(m_outgoingserver, &outgoingserver::changedPing, this, &PilotWindow::changePing);
    QPushButton* reconnect = new QPushButton("Reconnect");
    robotStatusLayout->addRow("Connected: ", m_robot_connected);
    robotStatusLayout->addRow("Ping: ", m_robot_ping);
    robotStatusLayout->addRow(reconnect);
    connect(reconnect, &QPushButton::clicked, this, &PilotWindow::sigReconnect);
    robotBoxLayout->addLayout(robotStatusLayout);
    robotBox->setLayout(robotBoxLayout);

    QGroupBox* controllerBox = new QGroupBox("Controller");
    QHBoxLayout* controllerBoxLayout = new QHBoxLayout();
    m_controllerIcon = new QLabel;
    m_controllerIcon->setPixmap(QPixmap(":/icons/controller_dark.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    controllerBoxLayout->addWidget(m_controllerIcon);

    QFormLayout* controllerStatusLayout = new QFormLayout();
    m_controller_name = new QLabel(QString::fromStdString(m_contoller->getControllerName()));
    controllerStatusLayout->addRow("Controller: ", m_controller_name);
    controllerBoxLayout->addLayout(controllerStatusLayout);
    controllerBox->setLayout(controllerBoxLayout);

    QGroupBox* clawBox = new QGroupBox("Claw");
    QHBoxLayout* clawBoxLayout = new QHBoxLayout();
    m_clawIcon = new QLabel;
    m_clawIcon->setPixmap(QPixmap(":/icons/claw_dark.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    clawBoxLayout->addWidget(m_clawIcon);

    QFormLayout* clawStatusLayout = new QFormLayout();
    m_claw_status = new QLabel("Open");
    m_claw_angle = new QLabel("0 deg");
    clawStatusLayout->addRow("Claw Status: ", m_claw_status);
    clawStatusLayout->addRow("Claw Angle: ", m_claw_angle);
    clawBoxLayout->addLayout(clawStatusLayout);
    clawBox->setLayout(clawBoxLayout);

    statusBoxes->addWidget(robotBox);
    statusBoxes->addWidget(controllerBox);
    statusBoxes->addWidget(clawBox);

    layout->addLayout(statusBoxes);
    // Bottom bar: theme toggle and settings button
    QHBoxLayout* bottomBar = new QHBoxLayout();

    auto makeIconBtn = [](const QString& iconPath) -> QPushButton* {
        QPushButton* btn = new QPushButton();
        btn->setIcon(QIcon(iconPath));
        btn->setIconSize(QSize(24, 24));
        btn->setFixedSize(44, 44);
        btn->setFlat(true);
        btn->setStyleSheet(R"(
            QPushButton {
                background: palette(window);
                border: 1px solid palette(mid);
                border-radius: 22px;
            }
            QPushButton:hover {
                background: palette(light);
            }
            QPushButton:pressed {
                background: palette(midlight);
            }
        )");
        return btn;
    };

    themeBtn = makeIconBtn(":/icons/moon_dark.svg");
    themeBtn->setToolTip("Switch to dark mode");
    connect(themeBtn, &QPushButton::clicked, this, &PilotWindow::toggleTheme);

    settingsBtn = makeIconBtn(":/icons/settings_dark.svg");
    connect(settingsBtn, &QPushButton::clicked, settings, &QDialog::exec);

    bottomBar->addStretch();
    bottomBar->addWidget(themeBtn);
    bottomBar->addWidget(settingsBtn);
    layout->addLayout(bottomBar);
}

QHBoxLayout* PilotWindow::buildVideos()
{
    QHBoxLayout* videos = new QHBoxLayout();

    auto *player = new QMediaPlayer(this);
    auto *videoWidget = new QVideoWidget(this);

    videoWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(player, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error e, const QString &msg) {
        qWarning() << "Media error:" << msg;
    });
    player->setVideoOutput(videoWidget);
    QString url1 = QString("rtsp://%1:8554/%2").arg(m_settings->value("video_server/ip").toString(), m_settings->value("video_server/stream1").toString());
    qDebug() << url1;
    player->setSource(QUrl(url1));
    player->play();

    auto *player1 = new QMediaPlayer(this);
    auto *videoWidget1 = new QVideoWidget(this);

    videoWidget1->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    connect(player1, &QMediaPlayer::errorOccurred, this, [](QMediaPlayer::Error e, const QString &msg) {
        qWarning() << "Media error:" << msg;
    });
    player1->setVideoOutput(videoWidget1);
    QString url2 = QString("rtsp://%1:8554/%2").arg(m_settings->value("video_server/ip").toString()).arg(m_settings->value("video_server/stream2").toString());
    qDebug() << url2;
    player1->setSource(QUrl("rtsp://127.0.0.1:8554/rov-camera1"));
    player1->play();

    videos->addWidget(videoWidget);
    videos->addWidget(videoWidget1);
    return videos;
}
void PilotWindow::reloadVideos()
{
    // m_videos = buildVideos();
}

void PilotWindow::toggleTheme()
{
    m_darkMode = !m_darkMode;
    applyTheme(m_darkMode);
}

void PilotWindow::applyTheme(bool dark)
{
    QString btnStyle;
    if (dark) {
        QPalette p;
        p.setColor(QPalette::Window,          QColor(30, 30, 30));
        p.setColor(QPalette::WindowText,      QColor(220, 220, 220));
        p.setColor(QPalette::Base,            QColor(20, 20, 20));
        p.setColor(QPalette::AlternateBase,   QColor(45, 45, 45));
        p.setColor(QPalette::Text,            QColor(220, 220, 220));
        p.setColor(QPalette::Button,          QColor(45, 45, 45));
        p.setColor(QPalette::ButtonText,      QColor(220, 220, 220));
        p.setColor(QPalette::Highlight,       QColor(0, 120, 215));
        p.setColor(QPalette::HighlightedText, QColor(255, 255, 255));
        p.setColor(QPalette::Light,           QColor(60, 60, 60));
        p.setColor(QPalette::Midlight,        QColor(50, 50, 50));
        p.setColor(QPalette::Mid,             QColor(80, 80, 80));
        p.setColor(QPalette::Dark,            QColor(15, 15, 15));
        qApp->setPalette(p);

        btnStyle = R"(
            QPushButton {
                background: #1e1e1e;
                border: 1px solid #505050;
                border-radius: 22px;
            }
            QPushButton:hover {
                background: #3c3c3c;
            }
            QPushButton:pressed {
                background: #323232;
            }
        )";

        themeBtn->setIcon(QIcon(":/icons/sun.svg"));
        themeBtn->setToolTip("Switch to light mode");
        settingsBtn->setIcon(QIcon(":/icons/settings.svg"));
        m_robotIcon->setPixmap(QPixmap(":/icons/submarine.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_controllerIcon->setPixmap(QPixmap(":/icons/controller.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_clawIcon->setPixmap(QPixmap(":/icons/claw.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qApp->setPalette(qApp->style()->standardPalette());

        btnStyle = R"(
            QPushButton {
                background: #f0f0f0;
                border: 1px solid #b0b0b0;
                border-radius: 22px;
            }
            QPushButton:hover {
                background: #e0e0e0;
            }
            QPushButton:pressed {
                background: #d0d0d0;
            }
        )";

        themeBtn->setIcon(QIcon(":/icons/moon_dark.svg"));
        themeBtn->setToolTip("Switch to dark mode");
        settingsBtn->setIcon(QIcon(":/icons/settings_dark.svg"));
        m_robotIcon->setPixmap(QPixmap(":/icons/submarine_dark.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_controllerIcon->setPixmap(QPixmap(":/icons/controller_dark.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        m_clawIcon->setPixmap(QPixmap(":/icons/claw_dark.svg").scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    themeBtn->setStyleSheet(btnStyle);
    settingsBtn->setStyleSheet(btnStyle);
}

void PilotWindow::changePing(double ping)
{
    QString tmp_ping = QString("%1 ms").arg(ping);
    m_robot_ping->setText(tmp_ping);
}