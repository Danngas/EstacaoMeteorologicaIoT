#ifndef RELES_H
#define RELES_H

#include "pico/stdlib.h"
#include "pico/bootrom.h"

//Modificações:
#include "hardware/pwm.h"
#include "hardware/clocks.h"


//  Bibllotecas para os sensores bmp280 e aht20
#include "aht20.h"
#include "bmp280.h"

// Definições de pinos para os sensores bmp280 e aht20
#define I2C_PORT i2c0               // i2c0 pinos 0 e 1, i2c1 pinos 2 e 3
#define I2C_SDA 0                   // 0 ou 2
#define I2C_SCL 1                   // 1 ou 3
#define SEA_LEVEL_PRESSURE 101325.0 // Pressão ao nível do mar em Pa

 struct bmp280_calib_param params;

// Configuração do pino do buzzer

#define rele1 12
#define BOTAO_A 5
#define JOYSTICK_SW 22
#define BOTAO_B 6
#define JOYSTICK_Y 27
#define LED_G 11 
#define LED_B 12 
#define LED_R 13
#define BUZZER_PIN 10
#define RELE_PIN 16          // PINO PARA CONEXÃO COM O RELÉ NO PROJETO FISICO

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 3500

// Definição de uma função para inicializar o PWM no pino do buzzer


// ADIÇÃO DO RELÉ PARA CONREXÃO FÍSICA
void init_rele(){
    gpio_init(RELE_PIN);
    gpio_set_dir(RELE_PIN, GPIO_OUT);  
}

void init_led_rgb() {
    gpio_init(LED_R);
    gpio_set_dir(LED_R, GPIO_OUT);
    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_init(LED_B);
    gpio_set_dir(LED_B, GPIO_OUT);
    printf("LED RGB inicializado\n");
}

void set_led_red(bool ligada) {
    if (ligada) {
        gpio_put(LED_R, 1);
        gpio_put(LED_G, 0); 
        gpio_put(LED_B, 0);
                    // AQUI DEVE SER INSERIDO A CONEXÃO COM O RELÉ PARA CONTROLE
       
    } else {
        gpio_put(LED_R, 0);
        gpio_put(LED_G, 0);
        gpio_put(LED_B, 0);
                // AQUI DEVE SER INSERIDO A CONEXÃO COM O RELÉ PARA CONTROLE
        
    }
}

void set_led_gren(bool ligada) {
    if (ligada) {
        gpio_put(LED_R, 0);
        gpio_put(LED_G, 1); 
        gpio_put(LED_B, 0);
             // AQUI DEVE SER INSERIDO A CONEXÃO COM O RELÉ PARA CONTROLE
        
    } else {
        gpio_put(LED_R, 0);
        gpio_put(LED_G, 0);
        gpio_put(LED_B, 0);
             // AQUI DEVE SER INSERIDO A CONEXÃO COM O RELÉ PARA CONTROLE
       
    }
}





void init_buzzer() {
    gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
    uint clk_div = clock_get_hz(clk_sys) / (1000 * 4096); // Frequência ~1000Hz
    pwm_set_clkdiv(slice_num, clk_div);
    pwm_set_wrap(slice_num, 4095); // Resolução PWM
    pwm_set_enabled(slice_num, true);
    printf("Buzzer inicializado\n");
}

void set_buzzer(bool ligado) {
    pwm_set_gpio_level(BUZZER_PIN, ligado ? 256 : 0);   
}


void iniciar_botoes()
{
    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);

    gpio_init(BOTAO_B);
    gpio_set_dir(BOTAO_B, GPIO_IN);
    gpio_pull_up(BOTAO_B);

    gpio_init(JOYSTICK_SW);
    gpio_set_dir(JOYSTICK_SW, GPIO_IN);
    gpio_pull_up(JOYSTICK_SW);

    gpio_init(rele1);
    gpio_set_dir(rele1, GPIO_OUT);
}

#include "hardware/adc.h"
#define potenciometro 26
#define Relé 

uint16_t adc_value_x;
float volume;

void adc_gpio28()
{
    // Leitura dos valores analógicos
    adc_select_input(2);                  // Alteração do canal para utilizar o potenciometro na GPIO28
    adc_value_x = adc_read();
    volume = (adc_value_x / 4095.0) * 100.0;
}
//modificações (intervalo de limites)


//inicializacao dos sensores bmp280 e aht20

void init_i2c(){
    // Inicializa o I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}


void init_sensors_bmp_aht() {
 // Inicializa o BMP280
    bmp280_init(I2C_PORT);
   
    bmp280_get_calib_params(I2C_PORT, &params);

    // Inicializa o AHT20
    aht20_reset(I2C_PORT);
    aht20_init(I2C_PORT);

}

// funcao para acionar o buzeer quando atingir niveis criticos

void buzzer_alert() {
    set_buzzer(true);
    sleep_ms(50); // Buzzer ligado por 1 segundo
    set_buzzer(false);
}











#endif
