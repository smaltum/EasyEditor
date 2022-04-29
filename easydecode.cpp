#include "easydecode.h"

EasyDecode::EasyDecode()
{

}

// 视频解码
bool EasyDecode::videoDecode(EasyDemux *demux,rgbFrame rgb){

    // 基本判断
    if(demux==nullptr){
        return false;
    }

    // 解码器
    AVCodec *m_pCodec;

    // 获取解码器
    m_pCodec = avcodec_find_decoder(demux->getVideoParam()->codec_id);
    if (m_pCodec == nullptr)
    {
        cout<<"avcodec_find_decoder failed"<<endl;
        return false;
    }

    // 获取video解码器上下文
    m_pCodecCtx = avcodec_alloc_context3(m_pCodec);
    // 配置解码器上下文参数
    avcodec_parameters_to_context(m_pCodecCtx, demux->getVideoParam());

    // 设置线程数 | 按核心数获取
    m_pCodecCtx->thread_count = 16;

    // 打开解码器
    if (avcodec_open2(m_pCodecCtx, m_pCodec, nullptr) < 0)
    {
        cout<<"avcodec_open2 failed"<<endl;
        avcodec_free_context(&m_pCodecCtx);
        return false;
    }

    // 处理图像数据
    m_pFrame = av_frame_alloc();
    m_pFrameRgb = av_frame_alloc();

    // 创建单帧图像size内存
    unsigned char * out_buffer = (unsigned char *)av_malloc((size_t)av_image_get_buffer_size(AV_PIX_FMT_RGB32, m_pCodecCtx->width, m_pCodecCtx->height, 1));

    // 存储一帧像素数据缓冲区
    av_image_fill_arrays(m_pFrameRgb->data, m_pFrameRgb->linesize, out_buffer,
                         AV_PIX_FMT_RGB32, m_pCodecCtx->width, m_pCodecCtx->height, 1);

    AVPacket* packet = (AVPacket *)av_malloc(sizeof(AVPacket));

    // 初始化img_convert_ctx结构
    m_pSwsCtx = sws_getContext(m_pCodecCtx->width, m_pCodecCtx->height, m_pCodecCtx->pix_fmt,
                               m_pCodecCtx->width, m_pCodecCtx->height, AV_PIX_FMT_RGB32, SWS_BICUBIC, nullptr, nullptr, nullptr);

    int got_picture;

    // av_read_frame读取一帧未解码的数据
    while (av_read_frame(demux->getAVFmtCtx(), packet) >= 0)
    {
        // 解码一帧视频数据
        if (avcodec_decode_video2(m_pCodecCtx, m_pFrame, &got_picture, packet) < 0)
        {
            cout<<"avcodec_decode_video2 failed"<<endl;
            return false;
        }

        if (got_picture)
        {
            // 转换为Rgb Frame
            sws_scale(m_pSwsCtx, (const unsigned char* const*)m_pFrame->data, m_pFrame->linesize, 0, m_pCodecCtx->height,
                      m_pFrameRgb->data, m_pFrameRgb->linesize);
            rgb(m_pFrameRgb);

            //                QImage img((uchar*)m_pFrameRgb->data[0],m_pCodecCtx->width,m_pCodecCtx->height,QImage::Format_RGB32);
            //                m_pLabPlayer->setPixmap(QPixmap::fromImage(img)); // 在label上播放视频图片

        }
        av_free_packet(packet);
    }
}
