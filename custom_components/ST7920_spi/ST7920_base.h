#pragma once

#include "esphome/core/component.h"
#include "esphome/core/esphal.h"
#include "esphome/components/display/display_buffer.h"

namespace esphome
{
  namespace ST7920_base
  {

    enum ST7920Model
    {
      ST7920_MODEL_128_32 = 0,
      ST7920_MODEL_128_64,
      ST7920_MODEL_96_16,
      ST7920_MODEL_64_48,
      SSD1327_MODEL_128_128,
    };

    class ST7920 : public PollingComponent, public display::DisplayBuffer
    {
    public:
      void setup() override;

      void display();

      void update() override;

      void set_model(ST7920Model model) { this->model_ = model; }
      void set_reset_pin(GPIOPin *reset_pin) { this->reset_pin_ = reset_pin; }
      bool is_on();
      void turn_on();
      void turn_off();

      float get_setup_priority() const override { return setup_priority::PROCESSOR; }
      void fill(Color color) override;

    protected:
      virtual void command(uint8_t value) = 0;
      virtual void data(uint8_t value) = 0;
      void init_reset_();

      void draw_absolute_pixel_internal(int x, int y, Color color) override;

      int get_height_internal() override;
      int get_width_internal() override;
      size_t get_buffer_length_();
      const char *model_str_();

      ST7920Model model_{ST7920_MODEL_128_64};
      GPIOPin *reset_pin_{nullptr};
      bool is_on_{false};
    };

  } // namespace ST7920_base
} // namespace esphome
