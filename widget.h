#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QPainter>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QMouseEvent>
#include <QEvent>
#include <QStyle>
#include <QDebug>
#include <QTime>
#include <QFileDialog>
#include <QCoreApplication>

#include "easydemux.h"
#include "easydecode.h"

#include <iostream>

using namespace std;

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void videoPlay(char * path);

public slots:
    void openFile();

private:

    // 初始化布局
    void initViews();
    // 初始化监听
    void initListener();
    // 重置空间尺寸
    void resizeWidget(int width,int height);

    // 打开视频按钮
    QPushButton* m_pBtnOpen;
    // 播放按钮
    QPushButton* m_pBtnPlay;
    // 视频滑动条
    QSlider* m_pSliderVideo;
    // 当前播放时间
    QLabel* m_pLabCurTime;
    // 当前播放时间
    QLabel* m_pLabTotal;
    // 播放空间
    QLabel* m_pLabPlayer;
};

#endif // WIDGET_H
