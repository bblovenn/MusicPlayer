#include "musicprogressbar.h"
#include "ui_musicprogressbar.h"

MusicProgressBar::MusicProgressBar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicProgressBar)
{
    ui->setupUi(this);

    connect(ui->ProgressSlider, &QSlider::sliderMoved, this, [=](int position){
        emit sliderMoved(position);
    });
}

MusicProgressBar::~MusicProgressBar()
{
    delete ui;
}

void MusicProgressBar::setDuration(qint64 duration)
{
    ui->ProgressSlider->setRange(0, static_cast<int>(duration));
    ui->TotalTimeLabel->setText(formatTime(duration));
}

void MusicProgressBar::setPosition(qint64 position)
{
    ui->ProgressSlider->setValue(static_cast<int>(position));
    ui->CurrentTimeLabel->setText(formatTime(position));
}

void MusicProgressBar::reset()
{
    ui->ProgressSlider->setRange(0, 0);
    ui->ProgressSlider->setValue(0);
    ui->CurrentTimeLabel->setText("00:00");
    ui->TotalTimeLabel->setText("00:00");
}

QString MusicProgressBar::formatTime(qint64 milliseconds)
{
    int totalSeconds = milliseconds / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

    return QString("%1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
}
