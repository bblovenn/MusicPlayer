#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("超级帅的音乐播放器");

    m_player = new QMediaPlayer(this);

    setFixedSize(1700,950);

    setBackGround(":/BackGround");

    initButton();

    //加载本地音乐
    QString songPath = "D:\\QTProject\\MusicPlayer\\MusicPlayer\\song\\陈奕迅 - 单车.mp3";
    qDebug() << "文件是否存在:" << QFile::exists(songPath);
    m_player->setMedia(QUrl::fromLocalFile(songPath));

    //检查播放器状态
    connect(m_player, &QMediaPlayer::stateChanged, [](QMediaPlayer::State state){
        qDebug() << "播放器状态:" << state;
    });
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error),
            [](QMediaPlayer::Error error){
        qDebug() << "播放器错误:" << error;
    });

    m_player->play();
    qDebug() << "当前音量:" << m_player->volume();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initButton()
{
    setButtonStyle(ui->PrevButton,":/Prev");
    setButtonStyle(ui->PSButton,":/Play");
    setButtonStyle(ui->NextButton,":/Next");
    setButtonStyle(ui->ModelButton,":/Order");
    setButtonStyle(ui->ListButton,":/List");
}

void MainWindow::setButtonStyle(QPushButton * button, const QString &fileName)
{
    //设置图标
    button->setFixedSize(50,50);
    button->setIcon(QIcon(fileName));
    button->setIconSize(QSize(button->width(),button->height()));
    //设置按钮背景
    //将 UI 界面的按钮背景设置为完全透明
    button->setStyleSheet("background-color:transparent");
}

void MainWindow::setBackGround(const QString &fileName)
{
    QPixmap pixmap(fileName);

    //获取当前窗口的大小
    QSize windowSize = this->size();

    //将自己的背景图片缩放到窗口的大小
    QPixmap scalePixmap = pixmap.scaled(windowSize,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);

    //获取当前控件（this 指向的窗口 / 按钮 / 标签等）自带的调色板对象，并复制一份到本地变量 palette 中。
    //QPalette	Qt 调色板类，专门管理控件的颜色、背景、文字颜色
    QPalette palette = this->palette();
    //把一张缩放后的图片（scalePixmap），设置为当前控件的背景图案。
    palette.setBrush(QPalette::Background,QBrush(scalePixmap));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

