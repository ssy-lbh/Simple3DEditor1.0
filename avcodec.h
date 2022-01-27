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
    char* GetError();
    const char* GetSuffix(const char* file);
    AVCodecID GetDecoderId(const char* file);
    AVCodecID GetWAVDecoderId(const char* file);
    bool Decode(AVCodecID id, void(*onFrame)(AVFrame*));
    bool Decode(AVCodecID id, void(*onFrame)(AVFrame*, void*), void* user);
};

#endif