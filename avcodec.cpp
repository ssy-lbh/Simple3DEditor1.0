#include "avcodec.h"

FFmpegCodec::FFmpegCodec(){}

FFmpegCodec::~FFmpegCodec(){
    if (errBuffer){
        delete[] errBuffer;
    }
}

bool FFmpegCodec::Open(const char* path){
    int err;
    if (ctx){
        avformat_close_input(&ctx);
    }
    err = avformat_open_input(&ctx, path, NULL, NULL);
    if (err){
        if (!errBuffer){
            errBuffer = new char[MAX_PATH];
        }
        av_strerror(err, errBuffer, MAX_PATH);
        return false;
    }
    return true;
}

void FFmpegCodec::Close(){
    if (ctx){
        avformat_close_input(&ctx);
    }
}

bool FFmpegCodec::Decode(AVCodecID id, void(*onFrame)(AVFrame*)){
    const AVCodec* avCodec;
    AVCodecContext* codecCtx;
    AVPacket* avPack;
    AVFrame* avFrame;
    int ret;

    avCodec = avcodec_find_decoder(id);
    if (!avCodec)
        return false;
    codecCtx = avcodec_alloc_context3(avCodec);
    avPack = av_packet_alloc();
    avFrame = av_frame_alloc();

    while (av_read_frame(ctx, avPack) >= 0){
        ret = avcodec_send_packet(codecCtx, avPack);
        if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
            return false;
        ret = avcodec_receive_frame(codecCtx, avFrame);
        if (ret < 0 && ret != AVERROR_EOF)
            return false;
        onFrame(avFrame);
    }

    av_frame_free(&avFrame);
    av_packet_free(&avPack);
    avcodec_free_context(&codecCtx);

    return true;
}