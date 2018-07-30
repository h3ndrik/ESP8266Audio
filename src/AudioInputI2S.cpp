/*
  AudioInputI2S
  I2S audio source

*/

#include <Arduino.h>
#ifdef ESP32
  #include "driver/i2s.h"
#else
  #include <i2s.h>
#endif
#include "AudioInputI2S.h"

AudioInputI2S::AudioInputI2S(int port=0, int use_apll=APLL_DISABLE)
{
  this->portNo = port;
  this->i2sOn = false;
#ifdef ESP32
  if (!i2sOn) {
    if (use_apll == APLL_AUTO) {
      // don't use audio pll on buggy rev0 chips
      use_apll = APLL_DISABLE;
      esp_chip_info_t out_info;
      esp_chip_info(&out_info);
      if(out_info.revision > 0) {
        use_apll = APLL_ENABLE;
      }
    }

    i2s_config_t i2s_config_adc = {
      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 44100,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
      .communication_format = (i2s_comm_format_t)(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_LSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1, // high interrupt priority
      .dma_buf_count = 8,
      .dma_buf_len = 8,
      .use_apll = use_apll // Use audio PLL
    };
    Serial.printf("+%d %p\n", portNo, &i2s_config_dac);
    if (i2s_driver_install((i2s_port_t)portNo, &i2s_config_adc, 0, NULL) != ESP_OK) {
      Serial.println("ERROR: Unable to install I2S drives\n");
    }
    SetPinout(21, 25, 19);

    i2s_zero_dma_buffer((i2s_port_t)portNo);
  } 
#else
  (void) use_apll;
  if (!i2sOn) {
    i2s_begin();
  }
#endif
  i2sOn = true;
  mono = false;
  bps = 32;
  channels = 1;
}

AudioInputI2S::~AudioInputI2S()
{
#ifdef ESP32
  if (i2sOn) {
    Serial.printf("UNINSTALL I2S\n");
    i2s_driver_uninstall((i2s_port_t)portNo); //stop & destroy i2s driver
  }
#else
  if (i2sOn) i2s_end();
#endif
i2sOn = false;
}

uint32_t AudioInputI2S::GetSample()
{
  if (!i2sOn) return 0;

  uint32_t sampleIn=0;
  i2s_pop_sample((i2s_port_t)portNo, (char*)&sampleIn, portMAX_DELAY);
  sampleIn>>=14; // right channel data

  return sampleIn;
}

uint32_t AudioInputI2S::read(void* data, uint32_t len)
{
  if (!i2sOn) return 0;

  i2s_read_bytes((i2s_port_t)portNo, (char*)data, (size_t)len, portMAX_DELAY);
}

bool AudioOutputI2S::SetPinout(int bclk, int wclk, int din)
{
#ifdef ESP32
  i2s_pin_config_t pin_config = {
    .bck_io_num = bclk,
    .ws_io_num = wclk,
    .data_out_num = I2S_PIN_NO_CHANGE,
    .data_in_num = din
  };
  i2s_set_pin((i2s_port_t)portNo, &pin_config);
  return true;
#else
  (void) bclk;
  (void) wclk;
  (void) din;
  return false;
#endif
}

bool AudioOutputI2S::SetRate(int hz)
{
  // TODO - have a list of allowable rates from constructor, check them
  this->hertz = hz;
#ifdef ESP32
  i2s_set_sample_rates((i2s_port_t)portNo, hz); 
#else
  i2s_set_rate(hz);
#endif
  return true;
}

bool AudioOutputI2S::SetBitsPerSample(int bits)
{
  if ( (bits != 32) && (bits != 16) && (bits != 8) ) return false;
  this->bps = bits;
  return true;
}
