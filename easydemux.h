#ifndef EASYDEMUX_H
#define EASYDEMUX_H


/**
 *在 C 语言中编译出来的签名是 _decoder
 *而在 C++ 语言中，一般编译器的生成则类似于 _decode_float_float
 *虽然在编译阶段是没有问题的，
 *但是在链接阶段，如果不加 extern “C” 关键字的话，
 *那么将会链接 _decoder_float_float 这个方法签名；
 *而如果加了 extern “C” 关键字的话，那么寻找的方法签名就是 _decoder。
 *而 FFmpeg 就是 C 语言书写的，
 *编译 FFmpeg 的时候所产生的方法签名都是 C 语言类型的签名，
 *所以在 C 中引用 FFmpeg 必须要加 extern “C” 关键字
 */
extern "C"{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <iostream>

using namespace std;

// 视频信息
typedef struct _stVideoInfo
{
    int stream_idx =-1; // 视频索引
    enum AVCodecID  codec_id; // 解码器
    int format;// 格式
    int total_ms=0; // 视频总长度
    int width = 0; // 宽
    int height = 0; // 高
    double fps = 0; // fps
} stVideoInfo;

// 音频信息
typedef struct _stAudioInfo
{
    int stream_idx =-1;// 音频索引
    enum AVCodecID  codec_id; // 解码器
    int format;// 格式
    int sample_rate;// 采样率
    int channels;// 声道
    int frame_size;// 单通道样本数
} stAudioInfo;

// 媒体文件信息
typedef struct _stMediaInfo
{
    stVideoInfo video_info;
    stAudioInfo audio_info;
} stMediaInfo;


class EasyDemux
{

public:
    EasyDemux();
    ~EasyDemux();

    // 打开文件
    bool fileDemux(char * url);
    // 对外暴露文件信息
    stMediaInfo *getMediaInfo();
    // 获取视频信息
    AVCodecParameters* getVideoParam();
    // 获取音频信息
    AVCodecParameters* getAudioParam();
    // 对外暴露上下文
    AVFormatContext * getAVFmtCtx();



private:
    // 封装信息上下文
    AVFormatContext *m_pFmtCtx;
    // 记录打开文件信息
    stMediaInfo* m_pMediaInfo;

};

#endif // EASYDEMUX_H
