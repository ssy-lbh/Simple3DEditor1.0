#include "avcodec.h"

#include <windows.h>

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

bool FFmpegCodec::Decode(AVCodecID id, void(*onFrame)(AVFrame*, void*), void* user){
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
        onFrame(avFrame, user);
    }

    av_frame_free(&avFrame);
    av_packet_free(&avPack);
    avcodec_free_context(&codecCtx);

    return true;
}

const char* FFmpegCodec::GetSuffix(const char* file){
    return strrchr(file, '.');
}

AVCodecID FFmpegCodec::GetDecoderId(const char* file){
    const char* suffix = GetSuffix(file);
    if (!suffix || *suffix != '.'){
        return AV_CODEC_ID_NONE;
    }
    suffix++;
    if (!strcmp(suffix, "wav")){
        return GetWAVDecoderId(file);
    }
    return AV_CODEC_ID_NONE;
}

AVCodecID FFmpegCodec::GetWAVDecoderId(const char* file){
    HANDLE hFile = CreateFile(
        file,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    DWORD specNum;
    DWORD readLen;
    if (hFile == INVALID_HANDLE_VALUE){
        return AV_CODEC_ID_NONE;
    }
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    if (specNum != *(DWORD*)"RIFF"){
        return AV_CODEC_ID_NONE;
    }
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    if (specNum != *(DWORD*)"WAVE"){
        return AV_CODEC_ID_NONE;
    }
    ReadFile(hFile, &specNum, 4, &readLen, NULL);
    while (readLen == 4){
        if (specNum == *(DWORD*)"fmt "){
            ReadFile(hFile, &specNum, 4, NULL, NULL);
            if (specNum != 0x10){
                return AV_CODEC_ID_NONE;
            }
            WAVEFORMATEX wav;
            ReadFile(hFile, &wav, 0x10, NULL, NULL);
            if (wav.wFormatTag == WAVE_FORMAT_PCM){
                if (wav.nBlockAlign == wav.nChannels){
                    return AV_CODEC_ID_PCM_U8;
                }else if (wav.nBlockAlign == wav.nChannels << 1){
                    return AV_CODEC_ID_PCM_S16LE;
                }
                return AV_CODEC_ID_NONE;
            }
            return AV_CODEC_ID_NONE;
        }
        ReadFile(hFile, &specNum, 4, NULL, NULL);
        SetFilePointer(hFile, specNum, NULL, FILE_CURRENT);
        ReadFile(hFile, &specNum, 4, &readLen, NULL);
    }
    return AV_CODEC_ID_NONE;
}