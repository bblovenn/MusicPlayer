#ifndef MUSICPROGRESSBAR_H
#define MUSICPROGRESSBAR_H

#include <QWidget>
#include <QSlider>

namespace Ui {
class MusicProgressBar;
}

class MusicProgressBar : public QWidget
{
    Q_OBJECT

public:
    explicit MusicProgressBar(QWidget *parent = nullptr);
    ~MusicProgressBar();

    //setDuration   设置歌曲总时长
    //setPosition   设置当前播放位置
    //reset         重置为 00:00 / 00:00

    void setDuration(qint64 duration);

    void setPosition(qint64 position);

    void reset();



private:
    QString formatTime(qint64 milliseconds);

signals:
    void sliderMoved(qint64 position);

private:
    Ui::MusicProgressBar *ui;
};

#endif // MUSICPROGRESSBAR_H
