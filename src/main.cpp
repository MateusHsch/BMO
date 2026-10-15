#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include "driver/spi_master.h"
#include "LGFX_Config.hpp"

void calibrar_display();
void teste_sd_card();

// Instancia classe do display
static LGFX tft;
uint16_t calData[8] = { 505, 3701, 538, 245, 3721, 3694, 3726, 324 };

extern "C" void app_main(void) {
    
    /* SD Card Init
    // 1. Initialize the SPI3 bus manually for the SD Card and Touch
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = 35,
        .miso_io_num = 37,
        .sclk_io_num = 36,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4000,
    };
    spi_bus_initialize(SPI3_HOST, &bus_cfg, SPI_DMA_CH_AUTO);

    // 2. Mount the SD Card on SPI3
    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    host.slot = SPI3_HOST;

    sdspi_device_config_t slot_config = SDSPI_DEVICE_CONFIG_DEFAULT();
    slot_config.gpio_cs = (gpio_num_t)38;
    slot_config.host_id = SPI3_HOST;

    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024
    };

    sdmmc_card_t *card;
    esp_err_t ret = esp_vfs_fat_sdspi_mount("/sdcard", &host, &slot_config, &mount_config, &card);

    */

    // 3. Inicializa hardware do display (LGFX will attach to the existing SPI3 bus)
    tft.init();
    tft.setTouchCalibrate(calData);
    tft.setRotation(1); // 1 = Landscape, 0 = Portrait

    //calibrar_display();

    tft.fillScreen(TFT_BLACK);
    
    // Estilo do texto
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(2);
    
    // Desenha texto inicial
    tft.setCursor(10, 10);
    tft.println("ESP-IDF + LovyanGFX");
    
    /* SD Mouted?
    // Exibe status do SD na tela para validacao visual
    tft.setCursor(10, 40);
    if (ret == ESP_OK) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.printf("SD Montado: %llu MB\n", ((uint64_t)card->csd.capacity * card->csd.sector_size) / (1024 * 1024));
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.printf("Falha SD: %s\n", esp_err_to_name(ret));
    }

    teste_sd_card();
    */

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setCursor(10, 70);
    tft.println("Toque em qualquer lugar...");

    uint8_t concluido = 0;
    uint8_t conta = 1;
    uint16_t offset = 10;

    TickType_t ultimo_tempo = xTaskGetTickCount();

    while (1) {
        uint16_t x, y;

        // Escreve de 1 a 10 na tela
        if (!concluido) {
            if ((xTaskGetTickCount() - ultimo_tempo) >= pdMS_TO_TICKS(1000)) {
                tft.setCursor(offset, 100);
                tft.printf("%d", conta);
                conta++;
                offset += 30;

                if (conta > 10) {
                    conta = 1;
                    offset = 10;
                    concluido = 1;
                }
                ultimo_tempo = xTaskGetTickCount();
            }
        }
        
        // Imprime as coordenadas do touch e desenha um circulo
        if (tft.getTouch(&x, &y)) {
            tft.setCursor(10, 130);
            tft.printf("Touch X: %03d | Y: %03d", x, y);
            tft.fillCircle(x, y, 3, TFT_RED);
        }
        
        // Task repete a cada 10ms
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// Função para calibração do display
void calibrar_display() {
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(10, 10);
    tft.println("Toque nas setas nos cantos...");

    // Array para armazenar os dados de calibração
    uint16_t calData[8];

    // Inicia a rotina gráfica de calibração do LovyanGFX
    tft.calibrateTouch(calData, TFT_WHITE, TFT_BLACK, std::max(tft.width(), tft.height()) >> 3);

    // Mostra os valores gerados na tela
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(10, 20);
    tft.println("Calibracao Concluida!");
    tft.setCursor(10, 50);
    tft.println("Tem 20 seg para anotar estes 8 numeros:");
    tft.setCursor(10, 80);
    tft.printf("%d, %d, %d, %d, %d, %d, %d, %d", 
            calData[0], calData[1], calData[2], calData[3], 
            calData[4], calData[5], calData[6], calData[7]);

    // Pausa dramática de 20 segundos para você conseguir anotar
    vTaskDelay(pdMS_TO_TICKS(20000));
}

void teste_sd_card() {
    const char* file_path = "/sdcard/teste.txt";
    
    // 1. Create and Write to the file
    // "w" mode creates a new file or truncates an existing one. Use "a" to append.
    FILE* f = fopen(file_path, "w");
    if (f == NULL) {
        printf("Failed to open file for writing\n");
        return;
    }
    fprintf(f, "Hello from ESP32-S3 SD Card!\n");
    fprintf(f, "File I/O is working perfectly.\n");
    fclose(f);
    printf("File written successfully.\n");

    // 2. Read back from the file
    // "r" mode opens the file strictly for reading.
    f = fopen(file_path, "r");
    if (f == NULL) {
        printf("Failed to open file for reading\n");
        return;
    }
    
    char line[128];
    printf("--- Reading file contents ---\n");
    
    // fgets reads until a newline or EOF is reached
    while (fgets(line, sizeof(line), f) != NULL) {
        printf("%s", line);
    }
    
    printf("-----------------------------\n");
    fclose(f);
}