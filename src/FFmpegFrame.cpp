#include "FFmpegFrame.hpp"

#include <cassert>

FFmpegFrame::FFmpegFrame(std::shared_ptr<AVFrame> frame)
{
    if (frame)
    {
        _frame = std::move(frame);
    }
    else
    {
        _frame.reset(av_frame_alloc(), [](AVFrame *ptr){
            av_frame_free(&ptr);
        });
    }
}

void FFmpegFrame::fillPicture(AVPixelFormat target_format, int target_width, int target_height)
{
    assert(static_cast<bool>(_data) == false);
    _data.reset(new char[av_image_get_buffer_size(target_format, target_width, target_height, 1)]);
    av_image_fill_arrays(_frame->data, _frame->linesize, reinterpret_cast<uint8_t *>(_data.get()), target_format, target_width, target_height, 1);
}

AVFrame *FFmpegFrame::get()
{
    return _frame.get();
}
