#ifndef __AVCODEC__
#define __AVCODEC__

#include <windef.h>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libavcodec/codec_id.h>
#include <libswscale/swscale.h>
}

class FFmpegCodec {
private:
    AVFormatContext* ctx;

    AVCodecContext* videoCtx;
    AVCodecContext* audioCtx;

    char* errBuffer = NULL;

public:
    FFmpegCodec();
    ~FFmpegCodec();

    bool Open(const char* path);
    void Close();
    bool Decode(AVCodecID id, void(*onFrame)(AVFrame*));
    char* GetError();
};

#endif