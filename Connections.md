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
- MISO <-> GPIO 5
- MOSI <-> GPIO 4
- SCK  <-> GPIO 6
- CS   <-> GPIO 7

## Touch (XPT2046) <-> ESP32-S3 (SPI3 - Shared with SD)
- T_CLK <-> GPIO 12 (Shared with Display SCK)
- T_CS  <-> GPIO 14 (Unchanged)
- T_DIN <-> GPIO 11 (Shared with Display MOSI)
- T_DO  <-> GPIO 13 (Dedicated MISO for Touch on SPI2)
