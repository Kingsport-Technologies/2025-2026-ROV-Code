#pragma once
#include <QMainWindow>
#include <QObject>
#include <QMenu>
#include <QWidget>
#include <QSettings>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QApplication>
#include <QPalette>
#include "controller_interface.hpp"


class PilotWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit PilotWindow(ControllerInterface *contoller, QWidget* parent = nullptr);
        ~PilotWindow() override = default;
    public slots:
        void reloadVideos();
        void toggleTheme();
    signals:
        void sigReconnect();
    protected:
        void buildUi();
        QHBoxLayout* buildVideos();
        void applyTheme(bool dark);
    private:
        QSettings* m_settings = nullptr;
        QPushButton* settingsBtn = nullptr;
        QPushButton* themeBtn = nullptr;
        QHBoxLayout* m_videos = nullptr;
        bool m_darkMode = false;
    // Status icon labels (updated on theme change)
        QLabel* m_robotIcon = nullptr;
        QLabel* m_controllerIcon = nullptr;
        QLabel* m_clawIcon = nullptr;
    // Dynamic Labels
        QLabel* m_robot_ping = nullptr;
        QLabel* m_robot_connected = nullptr;
        QLabel* m_controller_name = nullptr;
        QLabel* m_claw_status = nullptr;
        QLabel* m_claw_angle = nullptr;
        ControllerInterface* m_contoller = nullptr;
};
