#include <stdio.h>
#include <algorithm>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "LGFX_Config.hpp"

void calibrar_display();

// Instancia classe do display
static LGFX tft;
uint16_t calData[8] = { 505, 3701, 538, 245, 3721, 3694, 3726, 324 };

extern "C" void app_main(void) {
    
    // Inicializa hardware do display
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
    tft.setCursor(10, 40);
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
                tft.setCursor(offset, 70);
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
            tft.setCursor(10, 100);
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