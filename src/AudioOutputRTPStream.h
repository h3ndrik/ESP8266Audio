/*
  AudioOutputRTPStream
  Sends audio via network in an UDP RTP stream

  receive under Linux:
    nc -u -p 8266 -l | play -t raw -r 11025 -b 16 -c 2 -e signed-integer -
  
  Copyright (C) 2018  Hendrik Langer

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _AUDIOOUTPUTRTPSTREAM_H
#define _AUDIOOUTPUTRTPSTREAM_H

#include "AudioOutput.h"

class AudioOutputRTPStream : public AudioOutput 
{
  public:
    AudioOutputRTPStream(const IPAddress listener, const int port = 8266, const uint8_t payload_type = 11);
    ~AudioOutputRTPStream() override;
    virtual bool SetRate(int hz) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;

    enum : uint8_t { ULAW = 0, ALAW = 8, G722 = 9, L16 = 11 };

  private:

  protected:
    IPAddress listener;
    int port;
    uint8_t payload_type;
    WiFiUDP udp;
    int16_t buffLen;
    uint8_t *buff;
    int16_t cnt;
    uint16_t seq;
    static constexpr int RTP_HEADER_SIZE = 12;
    static constexpr float PACKET_MS = 0.02;
    uint32_t ssrc;
};

/** Defines the RTP header */
struct rtp_header {
  uint8_t  cc:4;      /**< CSRC count             */
  bool     ext:1;     /**< Extension headers      */
  bool     pad:1;     /**< Padding bit            */
  uint8_t  ver:2;     /**< RTP version number     */

  uint8_t  pt:7;      /**< Payload type           */
  bool     m:1;       /**< Marker bit             */

  uint16_t seq:16;    /**< Sequence number        */
  uint32_t ts:32;      /**< Timestamp              */
  uint32_t ssrc:32;    /**< Synchronization source */
};

#endif /* _AUDIOOUTPUTRTPSTREAM_H */

