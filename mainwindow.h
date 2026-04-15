#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QString>
#include <QPushButton>
#include <QPixmap>
#include <QPalette>
#include <QBrush>
#include <QMediaPlayer>
#include <QUrl>
#include <QSound>
#include <QFile>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QMessageBox>
#include <QPropertyAnimation>
#include <QGraphicsOpacityEffect>
#include <QRandomGenerator>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_PSButton_clicked();

    void on_ListButton_clicked();

    //切换播放模式
    void on_ModelButton_clicked();

private:
    //初始化按钮
    void initButton();

    //设置按钮样式
    //const QString &	高效传递只读字符串（图片路径）
    //fileName	参数：图片文件路径（如 ":/prev.png"）
    //PushButton * button	传入任意按钮指针，给这个按钮设置样式（
    void setButtonStyle(QPushButton * button,const QString & fileName);

    //设置背景
    void setBackGround(const QString & fileName);

    //加载指定的文件夹
    void loadMusicDir(const QString &filePath);

    //淡出动画函数
    void fadeOut();

    //淡入动画函数
    void fadeIn();

    //播放指定下标音乐
    //index :下标
    //检查 index 是否越界
    //根据 index 找到对应歌曲文件路径
    //设置 m_player->setMedia(...)
    //调用 m_player->play()
    //更新 m_currentIndex
    void playMusic(int index);

    //播放下一首歌
    void playNextMusic();

    //播放上一首歌
    void playPrevMusic();

private:
    Ui::MainWindow *ui;

    QSound *m_sound;
    QMediaPlayer *m_player;

    QPropertyAnimation *m_fadeOut;
    QPropertyAnimation *m_fadeIn;
    //控件淡入淡出动画必须用到的透明度特效对象
    QGraphicsOpacityEffect *m_opacityEffect;

    // 记录歌曲列表当前是否显示，用来决定点击列表按钮时淡出还是淡入
    bool m_musicListVisible = true;

    //播放模式
    enum PlayMode {
        OrderMode,
        RepeatMode,
        RandomMode
    };

    //当前播放模式
    PlayMode m_playMode = OrderMode;

    //记录当前歌曲下标
    int m_currentIndex = 0;
};
#endif // MAINWINDOW_H
