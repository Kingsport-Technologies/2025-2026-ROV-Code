#include "settingsDialog.hpp"
#include <QFormLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGroupBox>
settingsDialog::settingsDialog(QWidget* parent)
{
    m_settings = new QSettings("KTech", "2526Dreamer");
    buildUi();
}
void settingsDialog::buildUi()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    QFormLayout* form = new QFormLayout();
    m_address = new QLineEdit(m_settings->value("video_server/ip").toString());
    m_stream1 = new QLineEdit(m_settings->value("video_server/stream1").toString());
    m_stream2 = new QLineEdit(m_settings->value("video_server/stream2").toString());
    form->addRow("Video Server Address: ", m_address);
    form->addRow("Left Stream Path: ", m_stream1);
    form->addRow("Right Stream Path: ", m_stream2);
    layout->addLayout(form);
    QPushButton* button = new QPushButton("Save");
    layout->addWidget(button);
    connect(button, &QPushButton::clicked, this, &settingsDialog::submit);
}
void settingsDialog::submit()
{
    m_settings->setValue("video_server/ip", m_address->text());
    m_settings->setValue("video_server/stream1", m_stream1->text());
    m_settings->setValue("video_server/stream2", m_stream2->text());
    emit submited();
    close();
}