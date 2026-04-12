#pragma once
#include <QSettings>
#include <QDialog>
#include <QLineEdit>
#include <QWidget>
#include <QObject>

class settingsDialog : public QDialog
{
    Q_OBJECT
    public:
        settingsDialog(QWidget* parent = nullptr);
        ~settingsDialog() override = default;
    
    protected:
        void buildUi();
    public slots:
        void submit();
    signals:
        void submited();
    private:
        QSettings* m_settings = nullptr;
        QLineEdit* m_address = nullptr;
        QLineEdit* m_stream1 = nullptr;
        QLineEdit* m_stream2 = nullptr;
};