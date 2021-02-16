#include "RR32CanEngineDb/util/ConfigDataStreamParser.h"

#ifdef ARDUINO
#include <Arduino.h>
#else
#include <cstdio>
#endif

#include <RR32Can_config.h>

namespace RR32Can {

void ConfigDataStreamParser::addMessage(const Data& data) {
  switch (streamState) {
    case StreamState::IDLE:
      printf("Received uninteresting config data (IDLE).\n");
      break;
    case StreamState::STREAM_DONE:
      printf("Received uninteresting config data (STREAM_DONE).\n");
      break;
    case StreamState::WAITING_FIRST_PACKET:
      // First packet has a length less than 8

      if (data.dlc == 6 || data.dlc == 7) {
        // Initial uncompressed
        remainingBytes_ = (static_cast<uint32_t>(data.data[0]) << 24) | (static_cast<uint32_t>(data.data[1]) << 16) |
                          (static_cast<uint32_t>(data.data[2]) << 8) | (data.data[3]);
        uint16_t crc = (data.data[4] << 8) | (data.data[5]);
        this->crc_.loadReference(crc);

#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_PACKETS)
        printf(
            " Stream length: %i Bytes. CRC requested: %#04x CRC actual: "
            "%#04x.\n",
            remainingBytes_, crc_.getReference(), this->crc_.getCrc());

        if (data.dlc == 7) {
          // Initial compressed
          printf("Compressed Data!\n");
        }
#endif

        if (remainingBytes_ > 0) {
          streamState = StreamState::WAITING_DATA_PACKET;
        } else {
          streamState = StreamState::STREAM_DONE;
          if (consumer_ != nullptr) {
            consumer_->setStreamComplete();
          }
        }

      } else if (data.dlc >= 8) {
        printf("Oversized first packet! Aborting parse.\n");
        reset();
        return;
      } else if (data.dlc < 6) {
        printf("Undersized first packet! Aborting parse.\n");
        reset();
        return;
      }
      break;
    case StreamState::WAITING_DATA_PACKET:
      if (data.dlc != 8) {
        printf("Undersized data packet! Aborting parse.\n");
        reset();
        return;
      } else {
        // regular data packet

#if (LOG_DUMP_CONFIG_DATA_STREAM == STD_ON)
        data.printAsText();
#endif

        remainingBytes_ -= data.dlc;
        for (int i = 0; i < data.dlc; ++i) {
          crc_.updateCrc(data.data[i]);
        }

        const BufferManager input(const_cast<char*>(data.dataAsString()), data.dlc, CanDataMaxLength);
        textParser_.addText(input);

        if (remainingBytes_ == 0) {
          if (crc_.isCrcValid()) {
            streamState = StreamState::STREAM_DONE;
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_EVENTS)
            printf("Stream complete!\n");
#endif
            if (consumer_ != nullptr) {
              consumer_->setStreamComplete();
            }

          } else {
            streamState = StreamState::IDLE;
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_EVENTS)
            printf(
                "CRC Error - Stream aborted. CRC requested: %#04x, CRC actual: "
                "%#04x.\n",
                crc_.getReference(), crc_.getCrc());
#endif
            if (consumer_ != nullptr) {
              consumer_->setStreamAborted();
            }
          }
        } else {
#if (LOG_CONFIG_DATA_STREAM_LEVEL >= LOG_CONFIG_DATA_STREAM_LEVEL_PACKETS)
          printf("Stream continues. Bytes remaining: %i, Current CRC: %#04x\n", remainingBytes_, crc_.getCrc());
#endif
        }
      }
      break;
  }
}

}  // namespace RR32Can
