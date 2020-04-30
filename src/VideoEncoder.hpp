/**
 * @file
 * @brief Libav wrapper for video encoding
 */

#ifndef VIDEO_ENCODER_HPP
#define VIDEO_ENCODER_HPP

#include <string>

// Why extern? See https://libav.org/documentation/faq.html
extern "C" {
#include <libavcodec/avcodec.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}

/**
 * @brief Libav wrapper for video encoding
 */
class VideoEncoder {
public:
    /**
     * @brief Encoder constructor
     *
     * If the encoder cannot be instantiated, the program is aborted.
     *
     * @param[in] codecName Name of the video codec
     * @param[in] bitrate Video bitrate
     * @param[in] width Video width in pixels
     * @param[in] height Video height in pixels
     * @param[in] framerate Video framerate in frames per second
     */
    VideoEncoder(std::string codecName, int bitrate, int width, int height, int framerate);

    /**
     * @brief Encoder destructor
     */
    ~VideoEncoder();

    VideoEncoder(const VideoEncoder&) = delete;
    VideoEncoder& operator=(const VideoEncoder&) = delete;

    /**
     * @brief Opens the output file and allocates internal objects for encoding
     *
     * @param[in] fileName Name of the output video file
     * @returns 0 on success, negative error code on failure
     */
    int open(std::string fileName);

    /**
     * @brief Closes the output file and deallocates internal objects
     */
    void close();

    /**
     * @brief Encodes one RGB video frame
     *
     * The encoder must have been opened before calling this function, and the RGB array must have
     * the proper size (i.e. 3 * width * height).
     *
     * @param[in] rgbData Frame data in RGB24 format (R, G, B, R, G, B, ...)
     * @param[in] timestamp Presentation timestamp in units of framerate
     * @returns 0 on success, negative error code on failure
     */
    int encodeRgbData(const uint8_t* rgbData, int64_t timestamp);

private:
    AVCodec* codec;
    AVCodecContext* context;
    AVPacket* packet;
    AVFrame* frame;
    struct SwsContext* swsContext;
    FILE* file;
    int width;
    int height;

    /**
     * @brief Encodes one video frame
     *
     * @param[in] frame Frame to encode
     * @returns 0 on success, negative error code on failure
     */
    int encodeFrame(AVFrame* frame);
};

#endif
