#ifndef EASYDECODE_H
#define EASYDECODE_H

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
}

#include <iostream>

#include "easydemux.h"

struct AVCodecParameters;
struct AVCodecContext;
struct SwsContext;

using namespace std;

typedef void (*rgbFrame) (AVFrame*);

class EasyDecode
{
public:
    EasyDecode();
    // 视频解码
    bool videoDecode(EasyDemux *demux,rgbFrame rgb);

private:
    // 解码上下文
    AVCodecContext* m_pCodecCtx;
    // RGB转换上下文
    SwsContext* m_pSwsCtx;
    // 普通帧
    AVFrame* m_pFrame;
    // RGB帧
    AVFrame* m_pFrameRgb;
};

#endif // EASYDECODE_H
