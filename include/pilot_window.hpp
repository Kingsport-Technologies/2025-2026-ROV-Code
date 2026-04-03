#pragma once
#include <QMainWindow>
#include <QObject>
#include <QMenu>
#include <QWidget>

class PilotWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit PilotWindow(QWidget* parent = nullptr);
        ~PilotWindow() override = default;

    protected:
        void buildUi();


};