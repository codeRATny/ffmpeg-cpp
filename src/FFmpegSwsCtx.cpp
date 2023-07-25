#include "FFmpegSwsCtx.hpp"

FFmpegSwsCtx::FFmpegSwsCtx()
    : _target_width{0},
      _target_height{0},
      _src_width{0},
      _src_height{0},
      _target_format{AV_PIX_FMT_NONE},
      _src_format{AV_PIX_FMT_NONE},
      _ctx{nullptr, &sws_freeContext}
{}

FFmpegFrame::Ptr FFmpegSwsCtx::Scale(const FFmpegFrame::Ptr &frame)
{   
    auto av_frame = frame->get();
    auto target_width = _target_width ? _target_width : frame->get()->width;
    auto target_height = _target_height ? _target_height : frame->get()->height;

    if (av_frame->format == _target_format && av_frame->width == target_width && av_frame->height == target_height)
    {
        return frame;
    }

    if (_ctx && (_src_width != av_frame->width || _src_height != av_frame->height || _src_format != static_cast<enum AVPixelFormat>(av_frame->format)))
    {
        _ctx.reset();
    }

    if (!_ctx)
    {
        _src_format = static_cast<enum AVPixelFormat>(frame->get()->format);
        _src_width = av_frame->width;
        _src_height = av_frame->height;
        _ctx.reset(sws_getContext(av_frame->width, av_frame->height, static_cast<enum AVPixelFormat>(av_frame->format),
                                  target_width, target_height, _target_format, SWS_FAST_BILINEAR, nullptr, nullptr, nullptr));
    }

    if (_ctx)
    {
        auto out = std::make_shared<FFmpegFrame>();

        if (!out->get()->data[0]) 
            out->fillPicture(_target_format, target_width, target_height);

        if (0 >= sws_scale(_ctx.get(), av_frame->data, av_frame->linesize, 0, av_frame->height, out->get()->data, out->get()->linesize))
        {
            return nullptr;
        }

        out->get()->format = _target_format;
        out->get()->width = target_width;
        out->get()->height = target_height;
        out->get()->pkt_dts = av_frame->pkt_dts;
        out->get()->pts = av_frame->pts;

        return out;
    }

    return nullptr;   
}

void FFmpegSwsCtx::SetTargetPixelFormat(AVPixelFormat format) noexcept
{
    _target_format = format;
}

void FFmpegSwsCtx::SetTargetWidth(int width) noexcept
{
    _target_width = width; 
}

void FFmpegSwsCtx::SetTargetHeight(int height) noexcept
{
    _target_height = height;
}

AVPixelFormat FFmpegSwsCtx::TargetPixelFormat() const noexcept
{
    return _target_format;
}

int FFmpegSwsCtx::TargetWidth() const noexcept
{
    return _target_width;
}

int FFmpegSwsCtx::TargetHeight() const noexcept
{
    return _target_height;
}
