/*
  AudioInputI2S
  I2S audio source

*/

#ifndef _AUDIOINPUTI2S_H
#define _AUDIOINPUTI2S_H



class AudioInputI2S
{
  public:
    AudioInputI2S(int port=0, int use_apll=APLL_DISABLE);
    virtual ~AudioInputI2S();
    bool SetPinout(int bclkPin, int wclkPin, int dinPin);
    virtual bool SetRate(int hz);
    virtual bool SetBitsPerSample(int bits);
    uint32_t GetSample(void);
    virtual uint32_t read(void* data, size_t len);

    enum : int { APLL_AUTO = -1, APLL_ENABLE = 1, APLL_DISABLE = 0 };
  private:
  protected:
    uint8_t portNo;
    bool i2sOn;
    uint16_t hertz;
    uint8_t bps;
    uint8_t channels;
};

#endif // _AUDIOINPUTI2S_H
