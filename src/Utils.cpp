#include "Utils.hpp"

void SaveAvFrame(FFmpegFrame::Ptr frame, const char *filename)
{
    FILE *fDump = fopen(filename, "ab");

    auto frame_ptr = frame->get();

    uint32_t pitchY = frame_ptr->linesize[0];
    uint32_t pitchU = frame_ptr->linesize[1];
    uint32_t pitchV = frame_ptr->linesize[2];

    uint8_t *avY = frame_ptr->data[0];
    uint8_t *avU = frame_ptr->data[1];
    uint8_t *avV = frame_ptr->data[2];

    for (int i = 0; i < frame_ptr->height; i++)
    {
        fwrite(avY, frame_ptr->width, 1, fDump);
        avY += pitchY;
    }

    for (int i = 0; i < frame_ptr->height / 2; i++)
    {
        fwrite(avU, frame_ptr->width / 2, 1, fDump);
        avU += pitchU;
    }

    for (int i = 0; i < frame_ptr->height / 2; i++)
    {
        fwrite(avV, frame_ptr->width / 2, 1, fDump);
        avV += pitchV;
    }

    fclose(fDump);
}