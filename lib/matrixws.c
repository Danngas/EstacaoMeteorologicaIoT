#include "matrixws.h"
#include "pico/stdlib.h" // Para sleep_us

// Variável estática para armazenar o último valor de abertura
static float ultima_abertura = -1.0f;

// Função auxiliar para enviar um pixel à matriz WS2812
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); // Envia o valor RGB ao PIO
}

// Função auxiliar para converter valores RGB em formato de 32 bits para a matriz WS2812
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); // Combina R, G e B em um único valor
}



void desligar_matriz() {
    uint32_t pixels[25] = {0};
    for (int i = 0; i < 25; i++) {
        put_pixel(pixels[i]);
    }
}

void piscar_matriz_vermelha() {
    uint32_t pixels[25] = {0};
    for (int i = 0; i < 2; i++) { // Executa apenas uma vez
        for (int j = 0; j < 25; j++) {
            pixels[j] = urgb_u32(10, 0, 0); // Vermelho com intensidade 10
        }
        for (int j = 0; j < 25; j++) {
            put_pixel(pixels[j]);
        }
        sleep_ms(50); // Pausa de 500ms com LEDs acesos
        for (int j = 0; j < 25; j++) {
            pixels[j] = 0; // Desliga todos os LEDs
        }
        for (int j = 0; j < 25; j++) {
            put_pixel(pixels[j]);
        }
        sleep_ms(50); // Pausa de 500ms com LEDs apagados
    }
}

