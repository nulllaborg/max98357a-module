#define CLOG_PREFIX_DATE (0)
#define CLOG_PREFIX_PID (0)
#define CLOG_PREFIX_TID (0)

#include "clog.hpp"
#include "driver/gpio.h"
#include "driver/i2s_std.h"

namespace {
#include "pcm.h"
i2s_chan_handle_t g_tx_handle = 0;

}  // namespace

void setup() {
  Serial.begin(115200);
  CLOGI << "setup";

  i2s_chan_config_t tx_chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_1, I2S_ROLE_MASTER);

  i2s_new_channel(&tx_chan_cfg, &g_tx_handle, NULL);

  i2s_std_config_t tx_std_cfg = {
      .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000),
      .slot_cfg = {
        .data_bit_width = I2S_DATA_BIT_WIDTH_16BIT,
        .slot_bit_width = I2S_SLOT_BIT_WIDTH_AUTO,
        .slot_mode = I2S_SLOT_MODE_MONO,
        .slot_mask = I2S_STD_SLOT_BOTH,
        .ws_width = I2S_DATA_BIT_WIDTH_16BIT,
        .ws_pol =false,
        .bit_shift = false,
        .msb_right = true,
      },
      .gpio_cfg =
          {
              .mclk = I2S_GPIO_UNUSED,
              .bclk = GPIO_NUM_33,
              .ws = GPIO_NUM_32,
              .dout = GPIO_NUM_23,
              .din = I2S_GPIO_UNUSED,
              .invert_flags =
                  {
                      .mclk_inv = false,
                      .bclk_inv = false,
                      .ws_inv = false,
                  },
          },
  };

  ESP_ERROR_CHECK(i2s_channel_init_std_mode(g_tx_handle, &tx_std_cfg));
  ESP_ERROR_CHECK(i2s_channel_enable(g_tx_handle));

  size_t bytes_written = 0;
  i2s_channel_write(g_tx_handle, _MERRY_CHRISTMAS_PCM, sizeof(_MERRY_CHRISTMAS_PCM), &bytes_written, 1000 * 10);
  CLOGI << "setup OK";
}

void loop() {

}
