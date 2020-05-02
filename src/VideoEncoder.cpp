/**
 * @file
 * @brief Libav wrapper for video encoding
 */

#include "VideoEncoder.hpp"
#include <iostream>

VideoEncoder::VideoEncoder(std::string codecName, int bitrate, int width, int height, int framerate)
    : codec(avcodec_find_encoder_by_name(codecName.c_str())),
      context(avcodec_alloc_context3(codec)), packet(av_packet_alloc()), width(width),
      height(height) {
    if (!codec || !context || !packet) {
        std::cerr << "Cannot initialize encoder\n";
        std::abort();
    }

    context->bit_rate = bitrate;
    context->width = width;
    context->height = height;
    context->time_base = (AVRational){1, framerate};
    context->framerate = (AVRational){framerate, 1};
    context->gop_size = 10;
    context->max_b_frames = 1;
    context->pix_fmt = AV_PIX_FMT_YUV420P;
    context->color_range = AVCOL_RANGE_JPEG;

    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(context->priv_data, "preset", "slow", 0);
    }
}

VideoEncoder::~VideoEncoder() {
    avcodec_free_context(&context);
    av_packet_free(&packet);
}

int VideoEncoder::open(std::string fileName) {
    int ret = avcodec_open2(context, codec, NULL);
    if (ret < 0) {
        return -1;
    }

    file = fopen(fileName.c_str(), "wb");
    if (!file) {
        return -1;
    }

    frame = av_frame_alloc();
    if (!frame) {
        fclose(file);
        return -1;
    }
    frame->format = context->pix_fmt;
    frame->width = context->width;
    frame->height = context->height;

    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        fclose(file);
        av_frame_free(&frame);
        return -1;
    }

    ret = av_frame_make_writable(frame);
    if (ret < 0) {
        fclose(file);
        av_frame_free(&frame);
        return -1;
    }

    // The SWS context is used to convert from RGB24 to YUV420
    swsContext = sws_getContext(width, height, AV_PIX_FMT_RGB24, width, height, AV_PIX_FMT_YUV420P,
        SWS_BILINEAR, NULL, NULL, NULL);
    if (!swsContext) {
        fclose(file);
        av_frame_free(&frame);
        return -1;
    }

    // We really just want to set the destination range to true (full range), but there doesn't seem
    // to be a way to set only this specific option
    int* invTable;
    int srcRange;
    int* table;
    int dstRange;
    int brightness;
    int contrast;
    int saturation;
    sws_getColorspaceDetails(
        swsContext, &invTable, &srcRange, &table, &dstRange, &brightness, &contrast, &saturation);

    dstRange = true;
    ret = sws_setColorspaceDetails(
        swsContext, invTable, srcRange, table, dstRange, brightness, contrast, saturation);
    if (ret < 0) {
        sws_freeContext(swsContext);
        fclose(file);
        av_frame_free(&frame);
        return -1;
    }

    return 0;
}

void VideoEncoder::close() {
    encodeFrame(NULL);
    if (codec->id == AV_CODEC_ID_MPEG1VIDEO || codec->id == AV_CODEC_ID_MPEG2VIDEO) {
        uint8_t endcode[] = {0, 0, 1, 0xb7};
        fwrite(endcode, 1, sizeof(endcode), file);
    }

    sws_freeContext(swsContext);
    fclose(file);
    av_frame_free(&frame);
}

int VideoEncoder::encodeRgbData(const uint8_t* rgbData, int64_t timestamp) {
    static const int rgbStride[1] = {3 * width};
    const uint8_t* const rgbSlice[1] = {rgbData};

    // Convert from RGB24 to YUV420
    int outputHeight =
        sws_scale(swsContext, rgbSlice, rgbStride, 0, height, frame->data, frame->linesize);
    if (outputHeight != height)
        return -1;

    frame->pts = timestamp;
    int ret = encodeFrame(frame);
    if (ret < 0) {
        return -1;
    }

    return 0;
}

int VideoEncoder::encodeFrame(AVFrame* frame) {
    int ret = avcodec_send_frame(context, frame);
    if (ret < 0) {
        return -1;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(context, packet);
        if ((ret == AVERROR(EAGAIN)) || (ret == AVERROR_EOF))
            return 0;
        else if (ret < 0)
            return -1;

        size_t size = fwrite(packet->data, 1, packet->size, file);
        if (size != static_cast<size_t>(packet->size)) {
            av_packet_unref(packet);
            return -1;
        }

        av_packet_unref(packet);
    }

    return 0;
}
