#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("超级帅的音乐播放器");

    initButton();

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

