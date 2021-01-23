#pragma once

#include "esphome/core/component.h"
#include "ST7920_base.h"
#include "esphome/components/spi/spi.h"

namespace esphome
{
  namespace ST7920_spi
  {
    class ST7920Component : public ST7920_base::ST7920,
                            public spi::SPIDevice<spi::BIT_ORDER_MSB_FIRST, spi::CLOCK_POLARITY_LOW, spi::CLOCK_PHASE_TRAILING,
                                                  spi::DATA_RATE_500KHZ> //DATA_RATE_1KHZ DATA_RATE_8MHZ
    {
    public:
      void setup() override;
      void dump_config() override;

    protected:
      void command(uint8_t value) override;
      void data(uint8_t value) override;
    };

  } // namespace ST7920_spi
} // namespace esphome
