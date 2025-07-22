

#include "lwip/tcp.h"
#include "display.h"
#include "interface.h"
#include "wifi_init.h"

#include "lib/matrixws.h"
#include "reles.h"
#include "pico/bootrom.h"

// #include "hardware/i2c.h"

#include <math.h>

// pinos para os senssores bmp280 e aht20

#define I2C_PORT i2c0               // i2c0 pinos 0 e 1, i2c1 pinos 2 e 3
#define I2C_SDA 0                   // 0 ou 2
#define I2C_SCL 1                   // 1 ou 3
#define SEA_LEVEL_PRESSURE 101325.0 // Pressão ao nível do mar em Pa
// Display na I2C
#define I2C_PORT_DISP i2c1
#define I2C_SDA_DISP 14
#define I2C_SCL_DISP 15
#define endereco 0x3C

#define WS2812_PIN 7 // GPIO para matriz de LEDs WS2812

#define WIFI_SSID "Tesla"
#define WIFI_PASS "123456788"

char str_x[5], str_v[5]; // Buffer para armazenar a string

void gpio_irq_handler(uint gpio, uint32_t events)
{
    static uint32_t last_press = 0;
    uint32_t current_time = to_ms_since_boot(get_absolute_time());
    if (current_time - last_press < 200)
        return;
    last_press = current_time;

    if (gpio == BOTAO_A)
    { //
        estado_menu = TELA_MONITORAMENTO;
    }
    else if (gpio == BOTAO_B)
    { // entra nomodo bootsel
        estado_menu = TELA_LIMITES;
    }
    else if (gpio == JOYSTICK_SW)
    {
        estado_menu = MENU_PRINCIPAL;
    }
}

// Função para calcular a altitude a partir da pressão atmosférica
double calculate_altitude(double pressure)
{
    return (44330.0 * (1.0 - pow(pressure / SEA_LEVEL_PRESSURE, 0.1903))); // Fórmula de barometria para calcular a altitude
}

int main()
{

    stdio_init_all();
    sleep_ms(2000);
    printf("Iniciando Estaçao Metereologica  ...\n");

    iniciar_display();
    iniciar_wifi(WIFI_SSID, WIFI_PASS);
    init_rele(bomba_ligada);

    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
    gpio_set_irq_enabled(BOTAO_B, GPIO_IRQ_EDGE_FALL, true);
    gpio_set_irq_enabled(JOYSTICK_SW, GPIO_IRQ_EDGE_FALL, true);
    printf("Interrupções dos botões configuradas\n");

    adc_init();
    adc_gpio_init(JOYSTICK_Y);
    printf("ADC e joystick inicializados\n");

    PIO pio = pio0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, 0, offset, WS2812_PIN, 800000, false);
    printf("PIO para WS2812 inicializado\n");

    init_led_rgb();
    init_buzzer();
    start_http_server();
    iniciar_botoes();

    // Inicializa i2c para os sensores bmp280 e aht20
    init_i2c();
    printf("I2C inicializado para sensores\n");
    // Inicializa os sensores bmp280 e aht20
    init_sensors_bmp_aht();
    printf("Sensores BMP280 e AHT20 inicializados\n");

    printf("--------------------------------\n");

    // Estrutura para armazenar os dados do sensor
    AHT20_Data data;
    int32_t raw_temp_bmp;
    int32_t raw_pressure;

    bool cor = true;
    gpio_put(LED_R, 0);
    gpio_put(LED_G, 1);
    gpio_put(LED_B, 0);

    while (true)
    {
        cyw43_arch_poll(); // Polling para manter a conexão Wi-Fi

        // Leitura do BMP280
        bmp280_read_raw(I2C_PORT, &raw_temp_bmp, &raw_pressure);
        int32_t temperature = bmp280_convert_temp(raw_temp_bmp, &params) + offset_bmp;
        int32_t pressure = bmp280_convert_pressure(raw_pressure, raw_temp_bmp, &params);

        // Cálculo da altitude
        double altitude = calculate_altitude(pressure);

        printf("Pressao = %.3f kPa\n", pressure / 1000.0);
        printf("Temperatura BMP: = %.2f C\n", temperature / 100.0);
        printf("Altitude estimada: %.2f m\n", altitude);

        // Leitura do AHT20
        if (aht20_read(I2C_PORT, &data))
        {
            printf("Temperatura AHT: %.2f C\n", data.temperature + offset_aht);
            printf("Umidade: %.2f %%\n\n\n", data.humidity + offset_umi);
        }
        else
        {
            printf("Erro na leitura do AHT10!\n\n\n");
        }

        sprintf(str_tmp1, "%.1fC", temperature / 100.0 + offset_aht); // Converte o inteiro em string
        sprintf(str_alt, "%.0fm", altitude + offset_alt);             // Converte o inteiro em string
        sprintf(str_tmp2, "%.1fC", data.temperature + offset_bmp);    // Converte o inteiro em string
        sprintf(str_umi, "%.1f%%", data.humidity + offset_umi);       // Converte o inteiro em string

        printf("Temperature AHT20: %s\n", str_tmp1);
        printf("Altitude: %s\n", str_alt);
        printf("Temperature BMP280: %s\n", str_tmp2);
        printf("Humidity: %s\n", str_umi);

        update_display(&ssd); // Atualiza o display

        // teste logico para acionar o buzzer alerta
        if (((temperature / 100.0 + offset_aht) >= alerta_temp_max) || ((data.humidity + offset_umi) >= alerta_umi_maxima) || ((data.temperature + offset_bmp) >= alerta_temp_max))
        {
            buzzer_alert();
            piscar_matriz_vermelha();
            printf("Alerta: Temperatura ou Umidade acima dos limites!\n");
            gpio_put(LED_R, 1);
            gpio_put(LED_G, 0);
            gpio_put(LED_B, 0);
        }
        else if (((temperature / 100.0 + offset_aht) <= alerta_temp_min) || ((data.humidity + offset_umi) <= alerta_umi_minima) || ((data.temperature + offset_bmp) <= alerta_temp_min))
        {
            buzzer_alert();
            piscar_matriz_vermelha();
            printf("Alerta: Temperatura ou Umidade abaixo dos limites!\n");
            gpio_put(LED_R, 1);
            gpio_put(LED_G, 0);
            gpio_put(LED_B, 0);
        }
        else
        {
            set_buzzer(false);
            desligar_matriz();
            gpio_put(LED_R, 0);
            gpio_put(LED_G, 1);
            gpio_put(LED_B, 0);
        }

        sleep_ms(100);
        // update_display(&ssd);
        // sleep_ms(10000); // Aguarda um pouco antes de atualizar novamente
    }

    // cyw43_arch_deinit(); // desconecta o Wi-Fi
    return 0;
}