// Copyright @ 2016 Caoyang Jiang

#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
}

/**
 * @brief     User must ensure to feed exactly one frame of encoded bitstream.
 */
class StreamDecode
{
 public:
  StreamDecode()
  {
  }
  ~StreamDecode()
  {
  }

  bool Destroy()
  {
    avcodec_close(codecctx_);
    av_free(codecctx_);
    av_frame_free(&frame_);
    return true;
  }

  /**
   * @brief      Initialize a streaming decoder.
   *
   * @return     True if initialization is successful, false otherwise.
   */
  bool Initialize()
  {
    avcodec_register_all();

    if (!(codec_ = avcodec_find_decoder(AV_CODEC_ID_H264)))
    {
      std::cout << "Codec not found\n" << std::endl;
      return false;
    }

    if (!(codecctx_ = avcodec_alloc_context3(codec_)))
    {
      std::cout << "Could not allocate video codec context" << std::endl;
      return false;
    }

    if (codec_->capabilities & AV_CODEC_CAP_TRUNCATED)
      codecctx_->flags |= AV_CODEC_FLAG_TRUNCATED;

    if (avcodec_open2(codecctx_, codec_, NULL) < 0)
    {
      std::cout << "Could not open Codec" << std::endl;
      return false;
    }

    if (!(frame_ = av_frame_alloc()))
    {
      std::cout << "Could not allocate frame" << std::endl;
      return false;
    }

    av_init_packet(&avpkt_);

    std::memset(buf_ + kPADDINGSIZE, 0, kPADDINGSIZE);

    return true;
  }

  /**
   * @brief      Must pass exactly one frame of data.
   *
   * @param[in]  bitstream  The bitstream
   * @param[in]  size       The size
   *
   * @return     Return true if successfully decodes a frame. false otherwise.
   */
  bool DecodeAFrame(uint8_t *bitstream,
                    size_t size,
                    uint8_t *(&data)[3],
                    int &width,
                    int &height)
  {
    int remainbytes = static_cast<int>(size);

    avpkt_.data = bitstream;
    avpkt_.size = remainbytes;

    int ret = avcodec_send_packet(codecctx_, &avpkt_);

    while (ret == AVERROR(EAGAIN))
    {
      ret = avcodec_send_packet(codecctx_, &avpkt_);
    }

    if (ret != 0 && ret != AVERROR(EAGAIN))
    {
      std::cout << "[ERROR]: Send packet failed" << std::endl;
      return false;
    }

    ret = avcodec_receive_frame(codecctx_, frame_);

    while (ret == AVERROR(EAGAIN))
    {
      ret = avcodec_receive_frame(codecctx_, frame_);
    }

    if (ret != 0 && ret != AVERROR(EAGAIN))
    {
      std::cout << "[ERROR]: receive frame failed" << std::endl;
      return false;
    }

    std::cout << frame_->linesize[0] << " " << frame_->linesize[1] << " "
              << frame_->linesize[2] << std::endl;
    data[0] = frame_->data[0];
    data[1] = frame_->data[1];
    data[2] = frame_->data[2];
    width   = frame_->width;
    height  = frame_->height;

    return true;
  }

 private:
  AVCodec *codec_;
  AVCodecContext *codecctx_;
  AVFrame *frame_;
  AVPacket avpkt_;
  static const int kINBUFSIZE   = 4096;
  static const int kPADDINGSIZE = 32;
  uint8_t buf_[kINBUFSIZE + kPADDINGSIZE];
};

int main(int argc, char **argv)
{
  uint8_t *yuv[3];
  std::unique_ptr<uint8_t[]> bitstream;
  int width = 0, height = 0, size = 4 * 1024 * 1024;
  size_t filesize;
  StreamDecode decoder;

  if (argc < 3)
  {
    std::cout << "not enough argument" << std::endl;
    return 1;
  }

  bitstream = std::unique_ptr<uint8_t[]>(new uint8_t[size]);
  std::ifstream fs(argv[1], std::ifstream::in | std::ifstream::binary);
  fs.seekg(0, fs.end);
  filesize = fs.tellg();
  fs.seekg(0, fs.beg);
  fs.read(reinterpret_cast<char *>(bitstream.get()), filesize);
  fs.close();

  if (!decoder.Initialize()) return 1;

  if (!decoder.DecodeAFrame(bitstream.get(), filesize, yuv, width, height))
    return 1;

  std::cout << "First frame "
            << "Width: " << width << " Height: " << height << std::endl;

  fs.open(argv[2], std::ifstream::in | std::ifstream::binary);
  fs.seekg(0, fs.end);
  filesize = fs.tellg();
  fs.seekg(0, fs.beg);
  fs.read(reinterpret_cast<char *>(bitstream.get()), filesize);
  fs.close();

  if (!decoder.DecodeAFrame(bitstream.get(), filesize, yuv, width, height))
    return 1;

  std::cout << "Second frame "
            << "Width: " << width << " Height: " << height << std::endl;

  fs.open(argv[3], std::ifstream::in | std::ifstream::binary);
  fs.seekg(0, fs.end);
  filesize = fs.tellg();
  fs.seekg(0, fs.beg);
  fs.read(reinterpret_cast<char *>(bitstream.get()), filesize);
  fs.close();

  if (!decoder.DecodeAFrame(bitstream.get(), filesize, yuv, width, height))
    return 1;

  std::cout << "Third frame "
            << "Width: " << width << " Height: " << height << std::endl;

  if (!decoder.Destroy()) return 1;
  return 0;
}
