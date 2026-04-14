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
#include <QEasingCurve>
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


private:
    Ui::MainWindow *ui;

    QSound *m_sound;
    QMediaPlayer *m_player;

    QPropertyAnimation *m_fadeOut;
    QPropertyAnimation *m_fadeIn;
    //控件淡入淡出动画必须用到的透明度特效对象
    QGraphicsOpacityEffect *m_opacityEffect;


};
#endif // MAINWINDOW_H
