#include "easydemux.h"

EasyDemux::EasyDemux()
{

}

// 回收处理
EasyDemux::~EasyDemux(){
    if (m_pMediaInfo){
        free(m_pMediaInfo);
    }
}

// 打开文件
bool EasyDemux::fileDemux(char * url){
    // 注册 | 新版本已废弃
    av_register_all();

    // 初始化AVFormatContext
    m_pFmtCtx = avformat_alloc_context();

    // 打开音视频流
    if (avformat_open_input(&m_pFmtCtx, url, nullptr, nullptr) != 0)
    {
        cout<<"avformat_open_input failed"<<endl;
        return false;
    }

    // 探测文件信息
    if (avformat_find_stream_info(m_pFmtCtx, nullptr) < 0)
    {
        cout<<"avformat_find_stream_info failed"<<endl;
        return false;
    }

    // 申请内存
    size_t total_size = sizeof(stVideoInfo)+sizeof(stAudioInfo);
    m_pMediaInfo=(stMediaInfo *)malloc(total_size);
    if (m_pMediaInfo==nullptr){
        cout<<"media info memory malloc failed"<<endl;
        return false;
    }

    // 分配内存
    memset(m_pMediaInfo,0,total_size);

    // 获取媒体总时长 | 毫秒
    m_pMediaInfo->video_info.total_ms = m_pFmtCtx->duration / (AV_TIME_BASE / 1000);

    // 打印视频流详细信息
    av_dump_format(m_pFmtCtx, 0, url, 0);

    // 获取视频流索引
    int stream_idx = av_find_best_stream(m_pFmtCtx, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (stream_idx == -1) {
        cout << "find videoStream failed!" << endl;
        return false;
    }

    // 视频信息
    AVStream *p_v_stream = m_pFmtCtx->streams[stream_idx];

    m_pMediaInfo->video_info.stream_idx=stream_idx;
    m_pMediaInfo->video_info.codec_id=p_v_stream->codecpar->codec_id;
    m_pMediaInfo->video_info.format=p_v_stream->codecpar->format;
    m_pMediaInfo->video_info.width=p_v_stream->codecpar->width;
    m_pMediaInfo->video_info.height=p_v_stream->codecpar->height;
    //    m_pMediaInfo->video_info.fps=r2d(p_v_stream->avg_frame_rate);

    // 获取输入视频的宽高
    m_pMediaInfo->video_info.width = p_v_stream->codecpar->width;
    m_pMediaInfo->video_info.height = p_v_stream->codecpar->height;

    // 获取音频流索引
    stream_idx = av_find_best_stream(m_pFmtCtx, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    if (stream_idx == -1) {
        cout << "find audioStream failed!" << endl;
        return true;
    }

    // 音频信息
    AVStream *p_a_stream = m_pFmtCtx->streams[stream_idx];

    m_pMediaInfo->audio_info.stream_idx=stream_idx;
    m_pMediaInfo->audio_info.codec_id=p_a_stream->codecpar->codec_id;
    m_pMediaInfo->audio_info.format=p_a_stream->codecpar->format;
    m_pMediaInfo->audio_info.sample_rate=p_a_stream->codecpar->sample_rate;
    m_pMediaInfo->audio_info.channels=p_a_stream->codecpar->channels;
    m_pMediaInfo->audio_info.frame_size=p_a_stream->codecpar->frame_size;

    return true;
}

// 对外暴露文件信息
stMediaInfo *EasyDemux::getMediaInfo(){
    return m_pMediaInfo;
}

// 暴露上下文
AVFormatContext * EasyDemux::getAVFmtCtx(){
    return m_pFmtCtx;
}

// 获取视频信息
AVCodecParameters* EasyDemux::getVideoParam(){
    if (!m_pFmtCtx)
        return nullptr;

    AVCodecParameters *pCodecPara = avcodec_parameters_alloc();
    avcodec_parameters_copy(pCodecPara, m_pFmtCtx->streams[m_pMediaInfo->video_info.stream_idx]->codecpar);

    return pCodecPara;
}

// 获取音频信息
AVCodecParameters* EasyDemux::getAudioParam(){
    if (!m_pFmtCtx)
        return nullptr;

    AVCodecParameters *pCodecPara = avcodec_parameters_alloc();
    avcodec_parameters_copy(pCodecPara, m_pFmtCtx->streams[m_pMediaInfo->audio_info.stream_idx]->codecpar);

    return pCodecPara;
}
