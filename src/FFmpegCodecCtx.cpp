#include "FFmpegCodecCtx.hpp"

void FFmpegCodecCtx::Open(AVCodecID codec_id)
{
    _codec_ctx.reset(avcodec_alloc_context3(avcodec_find_decoder(codec_id)), [](AVCodecContext *ptr){
        avcodec_free_context(&ptr);
    });

    avcodec_open2(_codec_ctx.get(), _codec_ctx->codec, NULL);
}

FFmpegFrame::Ptr FFmpegCodecCtx::Decode(FFmpegPacket::Ptr &packet)
{
    auto ret = avcodec_send_packet(_codec_ctx.get(), packet->get());

    if (ret < 0)
    {
        return nullptr;
    }

    auto out_frame = std::make_shared<FFmpegFrame>();
    ret = avcodec_receive_frame(_codec_ctx.get(), out_frame->get());

    if (ret != 0)
    {
        char buff[512];
        av_make_error_string(buff, 512, ret);
        return nullptr;
    }

    return out_frame;
}

AVCodecContext *FFmpegCodecCtx::get()
{
    return _codec_ctx.get();
}