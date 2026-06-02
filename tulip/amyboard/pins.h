// pins.h
// pins (and other MCU constants)

#define SPI_LL_DATA_MAX_BIT_LEN (1 << 18)

#define I2C_NUM I2C_NUM_0
#define I2C_CLK_FREQ 400000
#define ESP_INTR_FLAG_DEFAULT 0

#ifdef AMYSYNTH_N16R8
// AMYSYNTH_N16R8: ESP32-S3 N16R8, PCM5102 DAC
// I2S: BCLK=4, LRCLK=5, DOUT=6  (PCM5102 does not need MCLK)
// UART MIDI RX on GPIO16 (UART1), no MIDI TX
#define CONFIG_I2S_BCLK   4
#define CONFIG_I2S_LRCLK  5
#define CONFIG_I2S_DOUT   6
#define CONFIG_I2S_DIN    -1
#define CONFIG_I2S_MCLK   -1
#define CONFIG_I2S_NUM    0
#define MIDI_IN_PIN       16
#define MIDI_OUT_PIN      UART_PIN_NO_CHANGE
// Stub values so amyboard_support.c compiles; i2c_follower_init() is never
// called for this board (guarded by #ifdef AMYBOARD in main.c)
#define I2C_FOLLOWER_SCL  -1
#define I2C_FOLLOWER_SDA  -1
#define I2C_MASTER_SCL    18
#define I2C_MASTER_SDA    17

#else
// stuff in the eagle
#define CONFIG_I2S_MCLK  3
#define CONFIG_I2S_BCLK  8
#define CONFIG_I2S_LRCLK 2
#define CONFIG_I2S_DOUT 6 // data going to the codec, eg DAC data, also called AMYOUT
#define I2C_FOLLOWER_SCL 5
#define I2C_FOLLOWER_SDA 4
#define I2C_MASTER_SCL 18
#define I2C_MASTER_SDA 17
#define CONFIG_I2S_DIN 9 // data coming from the codec, eg ADC  data, also called AMYIN

#define MIDI_OUT_PIN_A 14
#define MIDI_OUT_PIN_B 15

#define MIDI_IN_PIN 21
#define MPIO_C0 7

#define SPI0_CS0 10
#define SPI0_MOSI 11
#define SPI0_SCK 12
#define SPI0_MISO 13

#define CV_IN1 16
#define CV_IN2 15
#endif










