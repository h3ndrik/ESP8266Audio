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
    AudioOutputRTPStream(const IPAddress listener, const int port = 8266);
    ~AudioOutputRTPStream() override;
    virtual bool SetRate(int hz) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;

  private:

  protected:
    IPAddress listener;
    int port;
    WiFiUDP udp;
    int16_t buffLen;
    uint8_t *buff;
    int16_t cnt;
};

#endif /* _AUDIOOUTPUTRTPSTREAM_H */

