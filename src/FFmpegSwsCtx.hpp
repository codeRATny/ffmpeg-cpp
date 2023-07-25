#ifndef _FFMPEG_SWS_CTX_HPP
#define _FFMPEG_SWS_CTX_HPP

#include "FFmpegFrame.hpp"

extern "C"
{
#include "libswscale/swscale.h"
}

class FFmpegSwsCtx final {
public:
    using Ptr = std::shared_ptr<FFmpegSwsCtx>;

    FFmpegSwsCtx();

    void SetTargetPixelFormat(AVPixelFormat format) noexcept;
    void SetTargetWidth(int width) noexcept;
    void SetTargetHeight(int height) noexcept;

    AVPixelFormat TargetPixelFormat() const noexcept;
    int TargetWidth() const noexcept;
    int TargetHeight() const noexcept;

    FFmpegFrame::Ptr Scale(const FFmpegFrame::Ptr &frame);

private:
    int _target_width;
    int _target_height;
    int _src_width;
    int _src_height;

    AVPixelFormat _target_format;
    AVPixelFormat _src_format;

    std::unique_ptr<SwsContext, decltype(&sws_freeContext)> _ctx;
};

#endif
