#pragma once
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

class LGFX : public lgfx::LGFX_Device {
  lgfx::Panel_ILI9488     _panel_instance;
  lgfx::Bus_SPI           _bus_instance;
  lgfx::Touch_XPT2046     _touch_instance;

public:
  LGFX(void) {
    { // 1. Configuração do Barramento SPI
      auto cfg = _bus_instance.config();
      cfg.spi_host    = SPI2_HOST;
      cfg.spi_mode    = 0;
      cfg.freq_write  = 27000000;
      cfg.freq_read   = 16000000;
      cfg.spi_3wire   = false;
      cfg.use_lock    = true;
      cfg.dma_channel = SPI_DMA_CH_AUTO;
      cfg.pin_sclk    = 12; // SCK <-> GPIO 12[cite: 3]
      cfg.pin_mosi    = 13; // SDI/MOSI <-> GPIO 13[cite: 3]
      cfg.pin_miso    = 15; // SDO/MISO <-> GPIO 15 (Atualizado)
      cfg.pin_dc      = 11; // DC/RS <-> GPIO 11[cite: 3]
      _bus_instance.config(cfg);
      _panel_instance.setBus(&_bus_instance);
    }

    { // 2. Configuração do Painel ILI9488
      auto cfg = _panel_instance.config();
      cfg.pin_cs           = 10; // CS <-> GPIO 10[cite: 3]
      cfg.pin_rst          = 9;  // RESET <-> GPIO 9[cite: 3]
      cfg.pin_busy         = -1;
      cfg.panel_width      = 320;
      cfg.panel_height     = 480;
      cfg.offset_x         = 0;
      cfg.offset_y         = 0;
      cfg.offset_rotation  = 0;
      cfg.dummy_read_pixel = 8;
      cfg.dummy_read_bits  = 1;
      cfg.readable         = true;
      cfg.invert           = false;
      cfg.rgb_order        = false;
      cfg.dlen_16bit       = false; 
      cfg.bus_shared       = true;  
      _panel_instance.config(cfg);
    }

    { // 3. Configuração do Touch XPT2046
      auto cfg = _touch_instance.config();
      cfg.x_min      = 0;
      cfg.x_max      = 319;
      cfg.y_min      = 0;
      cfg.y_max      = 479;
      cfg.pin_int    = -1;
      cfg.bus_shared = true;
      cfg.offset_rotation = 0;
      cfg.spi_host   = SPI2_HOST;
      cfg.freq       = 2500000;
      cfg.pin_sclk   = 12;
      cfg.pin_mosi   = 13;
      cfg.pin_miso   = 16; // MISO do touch compartilhado no GPIO 15
      cfg.pin_cs     = 14; // T_CS <-> GPIO 14
      _touch_instance.config(cfg);
      _panel_instance.setTouch(&_touch_instance);
    }

    setPanel(&_panel_instance);
  }
};