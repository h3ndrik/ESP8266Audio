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

AudioOutputRTPStream::AudioOutputRTPStream(const IPAddress listener, const int port)
{
  this->listener = listener;
  this->port = port;
  hertz = 8000;
  channels = 1;
  bps = 32;
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
  buffLen = 0.02 * hz * sizeof(int16_t) * channels; // send 20ms in one packet
  buff = (uint8_t*)malloc(buffLen);
  return true;
}

bool AudioOutputRTPStream::begin()
{
  SetRate(hertz);
}

bool AudioOutputRTPStream::ConsumeSample(int16_t sample[2])
{
  int16_t* buff16 = reinterpret_cast<int16_t*>(buff);
  if (channels == 1) {
    buff16[cnt] = sample[0];
  } else if (channels == 2) {
    buff16[channels*cnt] = sample[0];
    buff16[channels*cnt+1] = sample[1];
  } else { return false; }
  cnt++;
  if (cnt*sizeof(int16_t)*channels < buffLen) return true;

  udp.beginPacket(listener, port);
  udp.write((uint8_t*)buff, buffLen);
  udp.endPacket();
  cnt = 0;

  return true;
}

bool AudioOutputRTPStream::stop()
{
  return false;
}


