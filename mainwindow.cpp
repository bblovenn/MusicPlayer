#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QCoreApplication>

namespace {
QString songDirPath()
{
    return QDir(QCoreApplication::applicationDirPath()).filePath("song");
}

QString songFilePath(const QString &fileName)
{
    return QDir(songDirPath()).filePath(fileName);
}
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("超级帅的音乐播放器");

    // 测试QMediaPlayer播放MP3
    m_player = new QMediaPlayer(this);
    m_player->setVolume(100);
    qDebug() << "QMediaPlayer created, state:" << m_player->state();

    setFixedSize(1700,950);

    setBackGround(":/BackGround");

    initButton();
    //QListWidget{}	选中目标控件：仅对当前 QListWidget 生效
    //border:none	去掉控件边框，无任何边框线
    //background:transparent	背景完全透明，不遮挡底层界面
    //font-size:24px	设置列表文字大小为 24 像素
    //font-weight:bold	设置列表文字为粗体


    ui->MusicList->setIconSize(QSize(52, 52));
    ui->MusicList->setSpacing(8);
    ui->MusicList->setStyleSheet(
        "QListWidget{"
        "border:1px solid rgba(255,255,255,80);"
        "border-radius:8px;"
        "background-color:rgba(12,18,24,115);"
        "color:white;"
        "font-size:24px;"
        "font-weight:bold;"
        "padding:16px 10px;"
        "outline:0;"
        "}"
        "QListWidget::item{"
        "height:54px;"
        "padding:0 18px;"
        "margin:4px 2px;"
        "border-radius:6px;"
        "}"
        "QListWidget::item:hover{"
        "background-color:rgba(255,255,255,42);"
        "}"
        "QListWidget::item:selected{"
        "background-color:rgba(90,170,255,145);"
        "color:white;"
        "}"
        "QScrollBar:vertical{"
        "background:transparent;"
        "width:8px;"
        "margin:12px 2px 12px 0;"
        "}"
        "QScrollBar::handle:vertical{"
        "background:rgba(255,255,255,95);"
        "border-radius:4px;"
        "min-height:36px;"
        "}"
        "QScrollBar::add-line:vertical,"
        "QScrollBar::sub-line:vertical{"
        "height:0;"
        "}"
        "QScrollBar::add-page:vertical,"
        "QScrollBar::sub-page:vertical{"
        "background:transparent;"
        "}"
    );
    ui->MusicProgressBarWidget->reset();

    QString musicDir = songDirPath();
    loadMusicDir(musicDir);

    ui->LyricLabel->hide();

    //初始化进度条
    connect(m_player, &QMediaPlayer::durationChanged, this, [=](qint64 duration){
        qDebug() << "durationChanged:" << duration;
        ui->MusicProgressBarWidget->setDuration(duration);
    });

    //歌曲每播放一小段，positionChanged 就会触发，进度条跟着往前走。
    connect(m_player, &QMediaPlayer::positionChanged, this, [=](qint64 position){
        ui->MusicProgressBarWidget->setPosition(position);
        updateLyric(position);
    });

    //自定义进度条 -> 播放器
    connect(ui->MusicProgressBarWidget, &MusicProgressBar::sliderMoved, this, [=](qint64 position){
        m_player->setPosition(position);
        ui->MusicProgressBarWidget->setPosition(position);
        updateLyric(position);
    });


    //告诉播放器当前音乐是否播放结束
    connect(m_player,&QMediaPlayer::mediaStatusChanged,this,[=](QMediaPlayer::MediaStatus status){
        if(status == QMediaPlayer::LoadedMedia || status == QMediaPlayer::BufferedMedia){
            ui->MusicProgressBarWidget->setDuration(m_player->duration());
        }
        if(status == QMediaPlayer::EndOfMedia){
            // 播放结束后根据模式处理
            playNextMusic();
        }
    });

    //初始化淡出动画
    m_opacityEffect = new QGraphicsOpacityEffect(this);
    ui->MusicList->setGraphicsEffect(m_opacityEffect);
    m_opacityEffect->setOpacity(0.0);  // 初始隐藏歌曲列表
    ui->MusicList->setVisible(false);
    ui->MusicList->setEnabled(false);

    //创建一个控制控件透明度的淡出动画对象
    //QPropertyAnimation
    //Qt 专门用于修改对象属性的动画类，核心功能：自动平滑地修改一个 Qt 对象的属性值（比如透明度、位置、大小）
    //这是一个 QGraphicsOpacityEffect 对象，作用是给控件添加透明度效果。
    //必须先给控件设置这个特效，才能控制透明度
    m_fadeOut = new QPropertyAnimation(m_opacityEffect, "opacity");
    m_fadeOut->setDuration(500);
    m_fadeOut->setStartValue(1.0);
    //让控件在动画结束时，彻底消失（淡出）
    m_fadeOut->setEndValue(0.0);
    //Qt 动画的「缓动曲线」设置，作用是：让淡出动画的速度变化更自然、更顺滑，不是机械匀速，而是先快后慢的平滑减速效果
    m_fadeOut->setEasingCurve(QEasingCurve::OutCubic);
    connect(m_fadeOut, &QPropertyAnimation::finished, this, [=](){
        if(!m_musicListVisible){
            ui->MusicList->setVisible(false);
            ui->MusicList->setEnabled(false);
        }
    });

    // 初始化淡入动画，与淡出动画相反
    m_fadeIn = new QPropertyAnimation(m_opacityEffect, "opacity");
    m_fadeIn->setDuration(500);
    m_fadeIn->setStartValue(0.0);
    m_fadeIn->setEndValue(1.0);
    m_fadeIn->setEasingCurve(QEasingCurve::InCubic);

}

MainWindow::~MainWindow()
{
    delete ui;
}

//初始化按钮
void MainWindow::initButton()
{
    setButtonStyle(ui->PrevButton,":/Prev");
    setButtonStyle(ui->PSButton,":/Play");
    setButtonStyle(ui->NextButton,":/Next");
    setButtonStyle(ui->ModelButton,":/Order");
    setButtonStyle(ui->ListButton,":/List");
    setButtonStyle(ui->LyricButton,":/Lyric");
}

//设置按钮大小和背景
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

//设置窗口背景
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

//加载本地音乐
void MainWindow::loadMusicDir(const QString &filePath)
{
    QDir dir(filePath);
    if(dir.exists()==false){
        QMessageBox::warning(this,"文件夹","文件打开失败");
        return;
    }
    //获取 dir 指向的目录下，所有「文件」（不包含子目录）的详细信息列表，并存储到 fileList 变量中。
    //QDir::Files
    //过滤参数，指定只获取「文件」类型的条目
    //会自动排除子目录、符号链接等其他类型
    QFileInfoList fileList = dir.entryInfoList(QDir::Files);

    //遍历文件信息列表 fileList，筛选出后缀为 mp3 的文件，将「不含后缀的文件名」添加到 UI 的 MusicList 列表控件中展示。
    //suffix: 返回文件后缀名的函数
    //范围 for 循环（C++11 引入），遍历 fileList（QFileInfoList 类型）
    //auto element：自动推导类型为 QFileInfo，每次循环代表列表中的一个文件信息对象
    for (const auto &element : fileList) {
        if(element.suffix() == "mp3"){
            QString musicName = element.baseName();

            QString coverPath = QDir(filePath).filePath(musicName + ".png");

            QListWidgetItem *item = new QListWidgetItem;
            item->setText(musicName);
            item->setIcon(QIcon(coverPath));

            ui->MusicList->addItem(item);
        }
    }
}

//淡出
void MainWindow::fadeOut()
{
    // 当前需求：淡出后保持隐藏，不在动画结束时自动淡入
    /*实现逻辑：
      1. 创建 QGraphicsOpacityEffect 对象
      2. 设置起始透明度为 1.0，结束为 0.0
      3. 设置持续时间（如 500ms）
      4. 绑定 effect 到音乐列表 widget
      5. 启动动画
      6. 动画结束后保持透明，等待下一次点击触发 fadeIn()*/

    //每次淡出前先停止旧动画并重置透明度
    m_fadeIn->stop();
    m_fadeOut->stop();
    ui->MusicList->setEnabled(false);
    m_opacityEffect->setOpacity(1.0);
    m_fadeOut->start();
}

//淡入
void MainWindow::fadeIn()
{
    //淡入前先停止淡入动画，并把透明度设为 0.0：
    // 淡入完成后恢复
    m_fadeIn->stop();
    m_fadeOut->stop();
    ui->MusicList->setVisible(true);
    ui->MusicList->setEnabled(true);
    m_opacityEffect->setOpacity(0.0);
    m_fadeIn->start();
}

//切换播放模式
void MainWindow::on_ModelButton_clicked()
{
    if(m_playMode == OrderMode){
        m_playMode = RepeatMode;
        ui->ModelButton->setIcon(QIcon(":/Repeat"));
    }
    else if(m_playMode == RepeatMode){
        m_playMode = RandomMode;
        ui->ModelButton->setIcon(QIcon(":/Random"));
    }
    else if(m_playMode == RandomMode){
        m_playMode = OrderMode;
        ui->ModelButton->setIcon(QIcon(":/Order"));
    }
}

//播放指定下标音乐
void MainWindow::playMusic(int index)
{
    if(index < 0 || index >= ui->MusicList->count()){
        return;
    }

    m_currentIndex = index;
    ui->MusicList->setCurrentRow(index);

    ui->MusicProgressBarWidget->reset();

    QString musicName = ui->MusicList->item(index)->text();
    QString musicPath = songFilePath(musicName + ".mp3");

    loadLyric(musicName);

    m_player->setMedia(QUrl::fromLocalFile(musicPath));
    ui->PSButton->setIcon(QIcon(":/Stop"));
    m_player->play();
}

//播放下一首歌
void MainWindow::playNextMusic()
{
    if(ui->MusicList->count() == 0){
        return;
    }

    if(m_playMode == OrderMode){
        int nextIndex = m_currentIndex + 1;
        if(nextIndex >= ui->MusicList->count()){
            nextIndex = 0;
        }
        playMusic(nextIndex);
    }
    else if(m_playMode == RepeatMode){
        m_player->setPosition(0);
        ui->MusicProgressBarWidget->setPosition(0);
        ui->PSButton->setIcon(QIcon(":/Stop"));
        m_player->play();
    }
    else if(m_playMode == RandomMode){
        // 从音乐列表中获取一个 0 ~ 列表总数-1 之间的随机索引
        //QRandomGenerator::global()	获取 Qt 全局随机数生成器（安全、通用，无需手动创建）
        //bounded(数值)	生成一个 大于等于 0 且 小于 传入数值 的随机整数
        //ui->MusicList->count()	获取音乐列表中歌曲的总数量
        //int randomIndex	存储最终生成的随机歌曲索引
        int randomIndex = QRandomGenerator::global()->bounded(ui->MusicList->count());
        if(ui->MusicList->count() > 1){
            while(randomIndex == m_currentIndex){
                randomIndex = QRandomGenerator::global()->bounded(ui->MusicList->count());
            }
        }
        playMusic(randomIndex);
    }
}

void MainWindow::playPrevMusic()
{
    if(ui->MusicList->count() == 0){
        return;
    }

    if(m_playMode == OrderMode){
            int prevIndex = m_currentIndex - 1;
            if(prevIndex < 0){
                prevIndex = ui->MusicList->count() - 1;
            }
            playMusic(prevIndex);
     }
     else if(m_playMode == RepeatMode){
         m_player->setPosition(0);
         ui->MusicProgressBarWidget->setPosition(0);
         ui->PSButton->setIcon(QIcon(":/Stop"));
         m_player->play();
     }
     else if(m_playMode == RandomMode){
         int randomIndex = QRandomGenerator::global()->bounded(ui->MusicList->count());
         if(ui->MusicList->count() > 1){
             while(randomIndex == m_currentIndex){
                 randomIndex = QRandomGenerator::global()->bounded(ui->MusicList->count());
             }
         }
         playMusic(randomIndex);
    }
}

void MainWindow::loadLyric(const QString &musicName)
{
    m_lyrics.clear();

    QString lyricPath = songFilePath(musicName + ".lrc");
    QFile file(lyricPath);

    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        ui->LyricLabel->setText("暂无歌词");
        return;
    }

    while(!file.atEnd()){
        QString line = QString::fromUtf8(file.readLine()).trimmed();
        // 正则表达式
        //匹配格式：[分钟:秒.毫秒]歌词内容
        //\\[` 和 `\\]：匹配方括号 [ ]（需要转义）
        //(\\d+)：捕获组 1 → 匹配分钟（纯数字）
        //:：匹配冒号
        //(\\d+\\.\\d+)：捕获组 2 → 匹配 秒.毫秒（带小数点）
        //(.*)：捕获组 3 → 匹配后面所有歌词文本
        QRegExp rx("\\[(\\d+):(\\d+\\.\\d+)\\](.*)");

        if (rx.indexIn(line) != -1) {
        int minutes = rx.cap(1).toInt();
        double seconds = rx.cap(2).toDouble();
        QString text = rx.cap(3);

        qint64 time = minutes * 60 * 1000 + seconds * 1000;
        m_lyrics.insert(time, text);
        }
    }

    file.close();
}

void MainWindow::updateLyric(qint64 position)
{
    if(m_lyrics.isEmpty()){
        return;
    }

    QString currentText; // 用来保存当前要显示的歌词

    // 遍历歌词列表（QMap 已经按时间从小到大排好序了）
    for(auto it = m_lyrics.begin(); it != m_lyrics.end(); ++it){
        // 如果歌词时间 ≤ 当前播放位置，就记录这句歌词
        if(it.key() <= position){
            currentText = it.value();
        }
        else{
            break;
        }
    }

    // 如果找到歌词，就显示到 Label 上
    if(!currentText.isEmpty()){
       ui->LyricLabel->setText(currentText);
    }
}

//播放暂停
void MainWindow::on_PSButton_clicked()
{
    qDebug() << "Current state:" << m_player->state();
    if(m_player->state() == QMediaPlayer::PlayingState){
        m_player->pause();
        ui->PSButton->setIcon(QIcon(":/Play"));
    }
    else if(m_player->state() == QMediaPlayer::PausedState){
        m_player->play();
        ui->PSButton->setIcon(QIcon(":/Stop"));
    }
    else{
        playMusic(m_currentIndex);
        ui->PSButton->setIcon(QIcon(":/Stop"));
    }
}

//查看歌曲列表
void MainWindow::on_ListButton_clicked()
{
    // 根据当前显示状态切换动画：显示时淡出，隐藏时淡入
    if(m_musicListVisible){
        //qDebug() << "ListButton clicked, fade out MusicList";
        fadeOut();
        m_musicListVisible = false;
    }
    else{
        //qDebug() << "ListButton clicked, fade in MusicList";
        fadeIn();
        m_musicListVisible = true;
    }
}

void MainWindow::on_NextButton_clicked()
{
    playNextMusic();
}

void MainWindow::on_PrevButton_clicked()
{
    playPrevMusic();
}

void MainWindow::on_MusicList_itemClicked(QListWidgetItem *item)
{
    if(item == nullptr){
            return;
        }
    //从 MusicList 里找到这个 item 是第几行。
    int index = ui->MusicList->row(item);
    playMusic(index);
    ui->PSButton->setIcon(QIcon(":/Stop"));
}

//点击按钮显示/隐藏歌词区域
void MainWindow::on_LyricButton_clicked()
{
    if(m_lyricVisible){
        ui->LyricLabel->hide();
        m_lyricVisible = false;
    }
    else{
        ui->LyricLabel->show();
        updateLyric(m_player->position());
        m_lyricVisible = true;
    }
}
