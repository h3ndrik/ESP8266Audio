/*
  AudioOutputRTPStream
  Sends audio via network in an UDP RTP stream

  receive under Linux:
    nc -u -p 8266 -l | play -t raw -r 8000 -b 16 -c 1 -e signed-integer -
  or:
    vlc --demux=rawaud --rawaud-channels=1 --rawaud-samplerate=8000 udp://@:8266
  
  Copyright (C) 2017  Earle F. Philhower, III

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

#include <Arduino.h>
#ifdef ESP32
    #include <WiFi.h>
#else
    #include <ESP8266WiFi.h>
#endif
#include <WiFiUdp.h>
#include "AudioOutputRTPStream.h"

AudioOutputRTPStream::AudioOutputRTPStream(const IPAddress listener, const int port, const uint8_t payload_type)
{
  this->listener = listener;
  this->port = port;
  this->payload_type = payload_type;
  hertz = 8000;
  channels = 1;
  bps = 16;
  this->buff = NULL;
}

AudioOutputRTPStream::~AudioOutputRTPStream()
{
  free(buff);
}

bool AudioOutputRTPStream::SetRate(int hz)
{
  this->hertz = hz;

  if (buff != NULL) free(buff);
  buffLen = PACKET_MS * hz * (bps/8) * channels + RTP_HEADER_SIZE; // send 20ms in one packet
  buff = (uint8_t*)malloc(buffLen);
  cnt = RTP_HEADER_SIZE;

  rtp_header* header = reinterpret_cast<rtp_header*>(buff);
  header->ver=2;
  header->pad=0;
  header->ext=0;
  header->cc=0;
  header->m=0;
  header->pt = this->payload_type;
  header->seq = esp_random();
  header->ssrc = this->ssrc;
  return true;
}

bool AudioOutputRTPStream::begin()
{
  this->ssrc = esp_random();
  SetRate(hertz);
}

bool AudioOutputRTPStream::ConsumeSample(int16_t sample[2])
{
  if (bps == 16) {
    int16_t* buff16 = reinterpret_cast<int16_t*>(buff);
    if (channels == 1) {
      buff16[cnt] = sample[0];
    } else if (channels == 2) {
      buff16[channels*cnt] = sample[0];
      buff16[channels*cnt+1] = sample[1];
    } else { return false; }
  } else if (bps == 8) {
    if (channels == 1) {
      buff[cnt] = sample[0];
    } else if (channels == 2) {
      buff[channels*cnt] = sample[0];
      buff[channels*cnt+1] = sample[1];
Serial.println("test");
    } else { return false; }
  }
    cnt++;
    if (cnt*(bps/8)*channels < buffLen) return true;

  rtp_header* header = reinterpret_cast<rtp_header*>(buff);

  header->seq++;
  header->ts = header->seq * PACKET_MS * hertz;

  udp.beginPacket(listener, port);
  udp.write((uint8_t*)buff, buffLen);
  udp.endPacket();
  cnt = RTP_HEADER_SIZE;

  return true;
}

bool AudioOutputRTPStream::stop()
{
  return false;
}


