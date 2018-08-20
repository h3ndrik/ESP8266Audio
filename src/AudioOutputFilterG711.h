/*
  AudioOutputFilterG711
  Implements ulaw and alaw compression
  
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

#ifndef _AUDIOOUTPUTFILTERG711_H
#define _AUDIOOUTPUTFILTERG711_H

#include "AudioOutput.h"

class AudioOutputFilterG711 : public AudioOutput
{
  public:
    AudioOutputFilterG711(AudioOutput *sink, int codec = ULAW);
    virtual ~AudioOutputFilterG711() override;
    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    virtual bool SetChannels(int chan) override;
    virtual bool SetGain(float f) override;
    virtual bool begin() override;
    virtual bool ConsumeSample(int16_t sample[2]) override;
    virtual bool stop() override;

    enum : int { ULAW = 0, ALAW = 1 };

  protected:
    AudioOutput *sink;
    int codec;
};

#endif /* _AUDIOOUTPUTFILTERG711_H */

