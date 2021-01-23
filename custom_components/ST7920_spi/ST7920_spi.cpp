#include "ST7920_spi.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome
{
  namespace ST7920_spi
  {

    static const char *TAG = "ST7920_spi";

    void ST7920Component::setup()
    {
      ESP_LOGCONFIG(TAG, "Setting up SPI ST7920...");
      this->spi_setup();
      if (this->cs_) this->cs_->setup(); // OUTPUT
      delay(40);
      this->init_reset_();
      ST7920::setup();
    }
    void ST7920Component::dump_config()
    {
      LOG_DISPLAY("", "SPI ST7920", this);
      ESP_LOGCONFIG(TAG, "  Model: %s", this->model_str_());
      if (this->cs_)
        LOG_PIN("  CS Pin: ", this->cs_);
      if (this->reset_pin_)
        LOG_PIN("  Reset Pin: ", this->reset_pin_);
      LOG_UPDATE_INTERVAL(this);
    }

    void ST7920Component::command(uint8_t value)
    {
      uint8_t buf[3];
      buf[0] = 0b11111000;
      buf[1] = value & 0xf0;
      buf[2] = (value & 0x0f) << 4;

      this->enable();
      if (this->cs_) this->cs_->digital_write(true);
      this->write_array(buf, 3);
      if (this->cs_) this->cs_->digital_write(false);
      this->disable();
    }

    void ST7920Component::data(uint8_t value)
    {
      uint8_t buf[3];
      buf[0] = 0b11111010;
      buf[1] = value & 0xf0;
      buf[2] = (value & 0x0f) << 4;

      this->enable();
      if (this->cs_) this->cs_->digital_write(true);
      this->write_array(buf, 3);
      if (this->cs_) this->cs_->digital_write(false);
      this->disable();
    }
  } // namespace ST7920_spi
} // namespace esphome
