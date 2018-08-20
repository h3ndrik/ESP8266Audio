/*
  AudioOutputFilterG711
  Implements ulaw and alaw compression
  ToDo: implement g722: https://github.com/sippy/libg722 or https://github.com/quatanium/foscam-ios-sdk/tree/master/g726lib

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

#include <Arduino.h>
#include "g726lib/g711.h"
#include "AudioOutputFilterG711.h"

AudioOutputFilterG711::AudioOutputFilterG711(AudioOutput *sink, int codec)
{
  this->sink = sink;
  this->codec = codec;
}

AudioOutputFilterG711::~AudioOutputFilterG711()
{
}

bool AudioOutputFilterG711::SetRate(int hz)
{
  return sink->SetRate(hz);
}

bool AudioOutputFilterG711::SetBitsPerSample(int bits)
{
  return sink->SetBitsPerSample(bits);
}

bool AudioOutputFilterG711::SetChannels(int channels)
{
  return sink->SetChannels(channels);
}

bool AudioOutputFilterG711::SetGain(float gain)
{
  return sink->SetGain(gain);
}

bool AudioOutputFilterG711::begin()
{
  return sink->begin();
}

bool AudioOutputFilterG711::ConsumeSample(int16_t sample[2])
{
    int16_t out[2];
    if (codec == ULAW) {
      out[LEFTCHANNEL] = linear2ulaw(sample[LEFTCHANNEL]);
    } else if (codec == ALAW) {
      out[RIGHTCHANNEL] = linear2alaw(sample[RIGHTCHANNEL]);
    } else {
      return false;
    }
    return sink->ConsumeSample(out);
}

bool AudioOutputFilterG711::stop()
{
  return sink->stop();
}
