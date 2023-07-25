#include "FFmpegCodecCtx.hpp"

#define STR_DECODER_THREADS_NUM "2"

bool FFmpegCodecCtx::Open(AVCodecID codec_id)
{
    _codec_ctx.reset(avcodec_alloc_context3(avcodec_find_decoder(codec_id)), [](AVCodecContext *ptr){
        avcodec_free_context(&ptr);
    });

    using DictionaryRaiiHelper = std::unique_ptr<AVDictionary *, decltype(&av_dict_free)>;

    AVDictionary        *dict  {nullptr};
    DictionaryRaiiHelper helper{&dict, av_dict_free};

    if (av_dict_set(&dict, "threads", STR_DECODER_THREADS_NUM, 0) < 0) {
        return false;
    }

    return avcodec_open2(_codec_ctx.get(), _codec_ctx->codec, &dict) == 0;
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
