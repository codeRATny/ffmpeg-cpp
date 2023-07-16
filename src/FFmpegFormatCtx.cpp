#include "FFmpegFormatCtx.hpp"

FFmpegFormatCtx::FFmpegFormatCtx()
{
    _format_ctx = avformat_alloc_context();
}

bool FFmpegFormatCtx::Open(std::string url)
{
    int ret = avformat_open_input(&_format_ctx, url.c_str(), NULL, NULL);

    if (ret != 0)
    {
        char buff[512];
        _format_ctx = nullptr;
        _format_ctx = _format_ctx = avformat_alloc_context();
        av_make_error_string(buff, 512, ret);
        return false;
    }

    return true;
}

bool FFmpegFormatCtx::Open(FFmpegIOCtx::Ptr avio, int buff_size)
{
    _buff_size = buff_size;
    _avio = std::move(avio);
    _avio_weak = _avio;
    _avio->Init(_buff_size, _avio_weak);

    _format_ctx->pb = _avio->get();

    int ret = avformat_open_input(&_format_ctx, NULL, NULL, NULL);

    if (ret != 0)
    {
        char buff[512];
        _format_ctx = nullptr;
        _format_ctx = _format_ctx = avformat_alloc_context();
        av_make_error_string(buff, 512, ret);
        return false;
    }

    return true;
}

std::vector<unsigned int> FFmpegFormatCtx::FindVideoStreamIDX()
{
    std::vector<unsigned int> result;

    for (unsigned int i = 0; i < _format_ctx->nb_streams; i++)
    {
        if (_format_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            if (_format_ctx->streams[i])
                if (_format_ctx->streams[i]->codecpar)
                    result.push_back(i);
        }
    }

    return result;
}

AVCodecID FFmpegFormatCtx::GetCodecID(unsigned int stream_idx)
{
    if (stream_idx < _format_ctx->nb_streams)
    {
        if (_format_ctx->streams[stream_idx])
            if (_format_ctx->streams[stream_idx]->codecpar)
                return _format_ctx->streams[stream_idx]->codecpar->codec_id;
    }

    return AV_CODEC_ID_NONE;
}

void FFmpegFormatCtx::TurnOnRealTimeMode()
{

}

void FFmpegFormatCtx::TurnOffRealTimeMode()
{
    
}

int FFmpegFormatCtx::ReadPacket(FFmpegPacket::Ptr &packet)
{
    packet = std::make_shared<FFmpegPacket>();
    return av_read_frame(_format_ctx, packet->get());
}

void FFmpegFormatCtx::DropEOS()
{
    _format_ctx->pb->eof_reached = 0;
}

FFmpegFormatCtx::~FFmpegFormatCtx()
{
    // _avio = nullptr;
    avformat_close_input(&_format_ctx);
}