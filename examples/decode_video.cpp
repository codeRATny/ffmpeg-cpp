#include <iostream>
#include "ffmpeg-cpp.hpp"
#include "Utils.hpp"

#define NUMBER_OF_FRAME 100

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        std::cout << "Usage: ./decode_video <filename>" << std::endl;
        return 1;
    }

    FFmpegFormatCtx::Ptr format_ctx = std::make_shared<FFmpegFormatCtx>();
    FFmpegCodecCtx::Ptr codec_ctx = std::make_shared<FFmpegCodecCtx>();
    FFmpegPacket::Ptr packet;

    if (format_ctx->Open(argv[1]) == false)
    {
        std::cout << "File open error" << std::endl;
        return 1;
    }

    auto video_streams_idx = format_ctx->FindVideoStreamIDX();

    if (video_streams_idx.empty())
    {
        std::cout << "No video stream in file" << std::endl;
        return 1;
    }

    auto codec_id = format_ctx->GetCodecID(video_streams_idx[0]);
    codec_ctx->Open(codec_id);

    int frame_counter = 0;
    while (true)
    {
        if (format_ctx->ReadPacket(packet) != 0)
        {
            std::cout << "Error reading packet" << std::endl;
            return 1;
        }

        if (packet->GetStreamIdx() == video_streams_idx[0])
        {
            auto frame = codec_ctx->Decode(packet);
            if (frame)
            {
                if (frame_counter == NUMBER_OF_FRAME)
                {
                    SaveAvFrame(frame, "decoded_frame.yuv");
                    break;
                }
                frame_counter++;
            }
        }
    }

    return 0;
}