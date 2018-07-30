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
    virtual ~AudioInputI2S() override;
    bool SetPinout(int bclkPin, int wclkPin, int dinPin);
    virtual bool SetRate(int hz) override;
    virtual bool SetBitsPerSample(int bits) override;
    uint32_t GetSample(void) override;
    virtual uint32_t read(void* data, uint32_t len) override;
  private:
  protected:
    uint8_t portNo;
    bool i2sOn;
    uint16_t hertz;
    uint8_t bps;
    uint8_t channels;
};

#endif // _AUDIOINPUTI2S_H
