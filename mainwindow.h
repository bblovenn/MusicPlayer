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
#include <QListWidgetItem>
#include <QMap>

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

    void on_ModelButton_clicked();

    void on_NextButton_clicked();

    void on_PrevButton_clicked();

    void on_MusicList_itemClicked(QListWidgetItem *item);


    void on_LyricButton_clicked();

private:
    //初始化按钮
    void initButton();

    //设置按钮样式
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
    void playMusic(int index);

    //播放下一首歌
    void playNextMusic();

    //播放上一首歌
    void playPrevMusic();

    //加载歌词
    void loadLyric(const QString &musicName);

    //歌词滚动更新
    void updateLyric(qint64 position);


private:
    Ui::MainWindow *ui;

    QSound *m_sound;
    QMediaPlayer *m_player;

    QPropertyAnimation *m_fadeOut;
    QPropertyAnimation *m_fadeIn;
    //控件淡入淡出动画必须用到的透明度特效对象
    QGraphicsOpacityEffect *m_opacityEffect;

    // 记录歌曲列表当前是否显示，用来决定点击列表按钮时淡出还是淡入
    bool m_musicListVisible = false;

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

    //开始时歌词默认隐藏
    bool m_lyricVisible = false;

    //.lrc 是“时间 -> 歌词”的结构
    //歌词数据结构
    QMap<qint64, QString> m_lyrics;
};
#endif // MAINWINDOW_H
