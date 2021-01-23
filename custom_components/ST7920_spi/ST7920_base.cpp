#include "ST7920_base.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome
{
  namespace ST7920_base
  {

    static const char *TAG = "ST7920";

#define LCD_CLS 0x01
#define LCD_HOME 0x02
#define LCD_ADDRINC 0x06
#define LCD_DISPLAYON 0x0C
#define LCD_DISPLAYOFF 0x08
#define LCD_CURSORON 0x0E
#define LCD_CURSORBLINK 0x0F
#define LCD_BASIC 0x30
#define LCD_EXTEND 0x34
#define LCD_GFXMODE 0x36
#define LCD_TXTMODE 0x34
#define LCD_STANDBY 0x01
#define LCD_SCROLL 0x03
#define LCD_SCROLLADDR 0x40
#define LCD_ADDR 0x80
#define LCD_LINE0 0x80
#define LCD_LINE1 0x90
#define LCD_LINE2 0x88
#define LCD_LINE3 0x98

const Color COLOR_OFF(0, 0, 0, 0);
const Color COLOR_ON(1, 1, 1, 1); 

static const unsigned char BitReverseTable256[] = 
{
  0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0, 0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0, 
  0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8, 0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8, 
  0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4, 0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4, 
  0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC, 0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC, 
  0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2, 0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2, 
  0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA, 0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
  0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6, 0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6, 
  0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE, 0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
  0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1, 0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
  0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9, 0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9, 
  0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5, 0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
  0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED, 0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
  0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3, 0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3, 
  0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB, 0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
  0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7, 0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7, 
  0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF, 0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

    void ST7920::setup()
    {
      this->init_internal_(this->get_buffer_length_());

      this->command(LCD_BASIC); /* 8 Bit interface (DL=1), basic instruction set (RE=0) */
      delay(1);
      this->command(LCD_BASIC); /* 8 Bit interface (DL=1), basic instruction set (RE=0) */
      delay(1);
      this->command(LCD_DISPLAYOFF); /* display on, cursor & blink off; 0x08: all off */
      delay(1);
      this->command(LCD_CLS);     /* clear RAM, needs 1.6 ms */
      delay(10);

      //this->command(LCD_ADDRINC); /* Entry mode: Cursor move to right ,DDRAM address counter (AC) plus 1, no shift  */
      //delay(1);
      //this->command(LCD_HOME);    /* disable scroll, enable CGRAM adress  */
      //delay(1);

      this->display(); // ...write buffer, which actually clears the display's memory
      this->turn_on(); // display ON
    }

    void ST7920::display()
    {
      this->command(LCD_GFXMODE);
      this->command(LCD_GFXMODE);
      for (int y = 0; y < 32; y++)
      {
        this->command(LCD_ADDR | y); // Vertical address range AC6...AC0
        this->command(LCD_ADDR);     // Horizontal address range AC3…AC0

        for (int x = 0; x < 16; x += 2)
        {
          this->data(BitReverseTable256[this->buffer_[y * 16 + x]]);
          this->data(BitReverseTable256[this->buffer_[y * 16 + x + 1]]);
        }

        for (int x = 0; x < 16; x += 2)
        {
          this->data(BitReverseTable256[this->buffer_[(y + 32) * 16 + x]]);
          this->data(BitReverseTable256[this->buffer_[(y + 32) * 16 + x + 1]]);
        }
      }
    }

    void ST7920::update()
    {
      this->do_update_();
      this->display();
    }

    bool ST7920::is_on() { return this->is_on_; }
    void ST7920::turn_on()
    {
      this->command(LCD_DISPLAYON);
      this->is_on_ = true;
    }
    void ST7920::turn_off()
    {
      this->command(LCD_DISPLAYOFF);
      this->is_on_ = false;
    }
    int ST7920::get_height_internal()
    {
      switch (this->model_)
      {
      case ST7920_MODEL_128_32:
        return 32;
      case ST7920_MODEL_128_64:
        return 64;
      case ST7920_MODEL_96_16:
        return 16;
      case ST7920_MODEL_64_48:
        return 48;
      case SSD1327_MODEL_128_128:
        return 128;
      default:
        return 0;
      }
    }
    int ST7920::get_width_internal()
    {
      switch (this->model_)
      {
      case ST7920_MODEL_128_32:
      case ST7920_MODEL_128_64:
      case SSD1327_MODEL_128_128:
        return 128;
      case ST7920_MODEL_96_16:
        return 96;
      case ST7920_MODEL_64_48:
        return 64;
      default:
        return 0;
      }
    }

    size_t ST7920::get_buffer_length_()
    {
      return size_t(this->get_width_internal()) * size_t(this->get_height_internal()) / 8;
    }

    void HOT ST7920::draw_absolute_pixel_internal(int x, int y, Color color)
    {
      if (x >= this->get_width_internal() || x < 0 || y >= this->get_height_internal() || y < 0)
        return;

      uint16_t pos = x / 8 + y * 16;
      uint8_t subpos = x % 8;
      if (color.is_on()) {
        this->buffer_[pos] |= (1 << subpos);
      } else {
        this->buffer_[pos] &= ~(1 << subpos);
      }
    }

    void ST7920::fill(Color color)
    {
      uint8_t fill = color.is_on() ? 0xFF : 0x00;
      for (uint32_t i = 0; i < this->get_buffer_length_(); i++)
        this->buffer_[i] = fill;
    }

    void ST7920::init_reset_()
    {
      if (this->reset_pin_ != nullptr)
      {
        this->reset_pin_->setup();
        // Trigger Reset
        this->reset_pin_->digital_write(false);
        delay(10);
        // Wake up
        this->reset_pin_->digital_write(true);
      }
    }

    const char *ST7920::model_str_()
    {
      switch (this->model_)
      {
      case ST7920_MODEL_128_32:
        return "ST7920 128x32";
      case ST7920_MODEL_128_64:
        return "ST7920 128x64";
      case ST7920_MODEL_96_16:
        return "ST7920 96x16";
      case ST7920_MODEL_64_48:
        return "ST7920 64x48";
      case SSD1327_MODEL_128_128:
        return "SSD1327 128x128";
      default:
        return "Unknown";
      }
    }

  } // namespace ST7920_base
} // namespace esphome
