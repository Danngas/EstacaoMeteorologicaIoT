#ifndef DISPLAY_H
#define DISPLAY_H
#include "ssd1306.h"
#include "font.h"

#define I2C_PORT_DISP i2c1
#define I2C_SDA_DISP 14
#define I2C_SCL_DISP 15
#define endereco 0x3C
ssd1306_t ssd;
bool cor = true;

char str_tmp1[5]; // Buffer para armazenar a string
char str_alt[5];  // Buffer para armazenar a string
char str_tmp2[5]; // Buffer para armazenar a string
char str_umi[5];  // Buffer para armazenar a string

// temperatura e umidade media
float temperatura_media_AHT20 = 0.0; // Temperatura media
float temperatura_media_BMP280 = 0.0; // Temperatura media
float temperatura_umidade_media = 0.0; // Temperatura media

// variaveis de calibracao ou Offsets do tipo float
float offset_bmp = 0.0;   // Offset para BMP280
float offset_aht = 0.0;   // Offset para AHT20
float offset_umi = 0.0;   // Offset para Umidade
float offset_alt = 0.0; // Offset para Altitude

// variaveis de alerta temperatura e umidade
uint16_t alerta_temp_min = 0;    // Temperatura limite minima
uint16_t alerta_temp_max = 50;   // Temperatura limite maxima
uint16_t alerta_umi_minima = 10; // Umidade limite minima
uint16_t alerta_umi_maxima = 80; // Umidade limite minima



uint16_t nivel_agua = 0; // 0-100%
bool bomba_ligada = false;
uint16_t lim_min = 10;
uint16_t lim_max = 90;
static uint32_t log_counter = 0;
char ip_str[24];

enum
{
    MENU_PRINCIPAL,
    TELA_MONITORAMENTO,
    TELA_LIMITES
} estado_menu = MENU_PRINCIPAL;

void iniciar_display()
{
    i2c_init(I2C_PORT_DISP, 400 * 1000);
    gpio_set_function(I2C_SDA_DISP, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_DISP, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_DISP);
    gpio_pull_up(I2C_SCL_DISP);

    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT_DISP);
    ssd1306_config(&ssd);
}


void draw_tela_limites(ssd1306_t *ssd)
{
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, 0, 0, 128, 64, true, false); // Borda retangular

    char buffer[10];
    sprintf(buffer, "%.1f", offset_bmp);
    ssd1306_draw_string(ssd, "BMP Ofset ",2, 8);
    ssd1306_draw_string(ssd, buffer, 80, 8);

    sprintf(buffer, "%.1f", offset_aht);
    ssd1306_draw_string(ssd, "AHT Ofset ",2, 20);
    ssd1306_draw_string(ssd, buffer, 80, 20);

    sprintf(buffer, "%.1f", offset_umi);
    ssd1306_draw_string(ssd, "Umi Ofet: ",2, 30);
    ssd1306_draw_string(ssd, buffer, 80, 30);

    sprintf(buffer, "%.0f", offset_alt);
    ssd1306_draw_string(ssd, "Alt Off: ",2, 40);
    ssd1306_draw_string(ssd, buffer, 80, 40);
}

void draw_tela_monitoramento(ssd1306_t *ssd)
{
    //  Atualiza o conteúdo do display com animações
    ssd1306_fill(ssd, false);                          // Limpa o display
    ssd1306_rect(ssd, 3, 3, 122, 60, cor, !cor);       // Desenha um retângulo
    ssd1306_line(ssd, 3, 25, 123, 25, cor);            // Desenha uma linha
    ssd1306_line(ssd, 3, 37, 123, 37, cor);            // Desenha uma linha
    ssd1306_draw_string(ssd, "CEPEDI   TIC37", 8, 6);  // Desenha uma string
    ssd1306_draw_string(ssd, "EMBARCATECH", 20, 16);   // Desenha uma string
    ssd1306_draw_string(ssd, "BMP280  AHT10", 10, 28); // Desenha uma string
    ssd1306_line(ssd, 63, 25, 63, 60, cor);            // Desenha uma linha vertical
    ssd1306_draw_string(ssd, str_tmp1, 14, 41);        // Desenha uma string
    ssd1306_draw_string(ssd, str_alt, 14, 52);         // Desenha uma string
    ssd1306_draw_string(ssd, str_tmp2, 73, 41);        // Desenha uma string
    ssd1306_draw_string(ssd, str_umi, 73, 52);         // Desenha uma string
    ssd1306_send_data(ssd);
}

void draw_menu_principal(ssd1306_t *ssd)
{
    ssd1306_fill(ssd, false);
    ssd1306_rect(ssd, 0, 0, 128, 64, true, false); // Borda retangular
    ssd1306_draw_string(ssd, "Menu", 48, 8);
    ssd1306_draw_string(ssd, "Monitorar: A", 10, 32);
    ssd1306_draw_string(ssd, "OFFSET: B", 10, 48);
}




void update_display(ssd1306_t *ssd)
{
    switch (estado_menu)
    {
    case MENU_PRINCIPAL:
        draw_menu_principal(ssd);
        break;
    case TELA_MONITORAMENTO:
        draw_tela_monitoramento(ssd); // Atualiza o display;
        break;
    case TELA_LIMITES:
        draw_tela_limites(ssd);
        break;
    }
    ssd1306_send_data(ssd);
    if (log_counter % 50 == 0)
    {
        printf("Display OLED atualizado: %s\n",
               estado_menu == MENU_PRINCIPAL ? "Menu Principal" : estado_menu == TELA_MONITORAMENTO ? "Monitoramento"
                                                                                                    : "Limites");
        printf("--------------------------------\n");
    }
}

#endif