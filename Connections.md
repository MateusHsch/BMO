# Connections

## Display (ILI9488) <-> ESP32-S3 (SPI2 - 80MHz IO_MUX)
- VCC <-> 3v3
- GND <-> GND
- CS <-> GPIO 10
- RESET <-> GPIO 8
- DC/RS <-> GPIO 9
- SDI/MOSI <-> GPIO 11
- SCK <-> GPIO 12
- LED <-> 3v3
- SDO/MISO <-> Not Connected 

## SD Card Module <-> ESP32-S3 (SPI3 - Independent Bus)
- GND <-> GND
- VCC <-> 5V (VBUS)
- MISO <-> GPIO 37
- MOSI <-> GPIO 35
- SCK <-> GPIO 36
- CS <-> GPIO 38

## Touch (XPT2046) <-> ESP32-S3 (SPI3 - Shared with SD)
- T_CLK <-> GPIO 36 (Shared with SD SCK)
- T_CS <-> GPIO 14
- T_DIN <-> GPIO 35 (Shared with SD MOSI)
- T_DO <-> GPIO 37 (Shared with SD MISO)
- T_IRQ <-> Not Connected
