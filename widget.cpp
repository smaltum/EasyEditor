#include "widget.h"

// 延时函数
void delay(int msec)
{
    QTime dieTime = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

Widget::Widget(QWidget *parent) :
    QWidget(parent)
{
    initViews();
    initListener();
}

Widget::~Widget()
{

}

// 初始化布局
void Widget::initViews(){

    // 初始化窗口大小
    this->setFixedSize(600, 800);

    // 播放 左
    QVBoxLayout* pVLayoutPlay = new QVBoxLayout;

    int btn_width=100,btn_height=33;

    // 按钮
    m_pBtnOpen = new QPushButton;
    m_pBtnPlay = new QPushButton;
    m_pBtnOpen->setFixedSize(btn_width, btn_height);
    m_pBtnPlay->setFixedSize(btn_width, btn_height);
    m_pBtnOpen->setText(QStringLiteral("打开"));
    m_pBtnPlay->setText(QStringLiteral("播放"));

    // 时间框
    m_pLabCurTime = new QLabel;
    m_pLabTotal = new QLabel;

    m_pLabCurTime->setFixedHeight(btn_height);
    m_pLabTotal->setFixedHeight(btn_height);

    m_pLabCurTime->setText("00:00:00");
    m_pLabTotal->setText("00:00:00");

    m_pLabCurTime->setStyleSheet("QLabel{color:black;font-size:18px;}");
    m_pLabTotal->setStyleSheet("QLabel{color:black;font-size:18px;}");

    // 水平布局
    QHBoxLayout* pHLayoutFunc = new QHBoxLayout;
    pHLayoutFunc->setSpacing(8);
    pHLayoutFunc->addWidget(m_pBtnPlay);
    pHLayoutFunc->addSpacing(12);
    pHLayoutFunc->addWidget(m_pBtnOpen);
    pHLayoutFunc->addStretch();
    pHLayoutFunc->addWidget(m_pLabCurTime);
    pHLayoutFunc->addSpacing(2);
    pHLayoutFunc->addWidget(m_pLabTotal);
    pHLayoutFunc->setSpacing(8);

    // 进度条
    m_pSliderVideo = new QSlider;
    m_pSliderVideo->setOrientation(Qt::Horizontal);
    m_pSliderVideo->setMinimum(0);
    m_pSliderVideo->setMaximum(100);
    m_pSliderVideo->setValue(0);
    m_pSliderVideo->setFixedHeight(6);

    // 播放区
    m_pLabPlayer=new QLabel;

    // 主布局
    pVLayoutPlay->addWidget(m_pLabPlayer);
    pVLayoutPlay->addWidget(m_pSliderVideo);
    pVLayoutPlay->addLayout(pHLayoutFunc);
    pVLayoutPlay->addSpacing(1);

    // 底层布局
    QHBoxLayout* pHLayoutMain=new QHBoxLayout(this);
    pHLayoutMain->addLayout(pVLayoutPlay);
}

// 初始化监听
void Widget::initListener(){

    // 打开文件
    connect(m_pBtnOpen,&QPushButton::clicked,[=](){
        openFile();
    });

    // 播放文件
    connect(m_pBtnPlay,&QPushButton::clicked,[=](){
//        playOrPause();
    });
}

// 选择文件
void Widget::openFile()
{
    // 文件选择
    QString path = QFileDialog::getOpenFileName(this,"打开文件","D:/Download/Task/input/","(*.mp4)");

    // 判断是否打开
    if (path.compare("")==0){
        return;
    }

    // 自动播放
    videoPlay(path.toLatin1().data());
}

// 播放文件
void Widget::videoPlay(char * filepath){

    EasyDemux demux;

    if(!demux.fileDemux(filepath)){
        cout<<"faild to demux file"<<endl;
        return;
    }


    stMediaInfo* mediaInfo;
    mediaInfo=demux.getMediaInfo();

    if (mediaInfo==nullptr){
        cout<<"faild to get media info"<<endl;
        return;
    }

//    cout<<"begin to decode"<<endl;

    EasyDecode decode;
}

// 重置空间尺寸
void Widget::resizeWidget(int width,int height){

    this->setFixedSize(width,height);

    m_pLabPlayer->setFixedSize(width,height);
}
