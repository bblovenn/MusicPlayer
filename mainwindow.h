#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QIcon>
#include <QString>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    //初始化按钮
    void initButton();

    //设置按钮样式
    //const QString &	高效传递只读字符串（图片路径）
    //fileName	参数：图片文件路径（如 ":/prev.png"）
    //PushButton * button	传入任意按钮指针，给这个按钮设置样式（
    void setButtonStyle(QPushButton * button,const QString & fileName);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
