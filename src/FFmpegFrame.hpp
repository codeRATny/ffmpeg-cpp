#ifndef _FFMPEG_FRAME_HPP_
#define _FFMPEG_FRAME_HPP_

#include <memory>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/codec.h>
#include <libavutil/avutil.h>
#include <libavutil/imgutils.h>
#include <libavutil/log.h>
}

class FFmpegFrame
{
public:
    using Ptr = std::shared_ptr<FFmpegFrame>;

    FFmpegFrame(std::shared_ptr<AVFrame> frame = nullptr);
    
    void fillPicture(AVPixelFormat target_format, int target_width, int target_height);

    AVFrame *get();

private:
    std::unique_ptr<char[]>  _data;
    std::shared_ptr<AVFrame> _frame;
};

#endif
