# Estação Meteorológica - Monitoramento

## Descrição
Este projeto implementa uma estação meteorológica baseada em microcontroladores, utilizando sensores BMP280 e AHT20 para medir temperatura, pressão atmosférica, umidade e altitude. A estação inclui um servidor web, display OLED, matriz de LEDs WS2812 para alertas visuais, buzzer para alertas sonoros e um sistema de controle via botões e joystick. Os dados são exibidos em uma interface web acessível por meio de um navegador.

## Funcionalidades
- **Monitoramento em Tempo Real**: Leitura de temperatura (BMP280 e AHT20), umidade, pressão atmosférica e altitude estimada.
- **Interface Web**: Página HTML com seções para sensores, gráficos, calibração de offsets e configuração de alertas.
- **Alertas**: Notificações visuais (matriz LED e LEDs RGB) e sonoras (buzzer) quando os limites de temperatura ou umidade são ultrapassados.
- **Calibração**: Ajuste de offsets para compensar variações nos sensores.
- **Controle Local**: Navegação por menu via botões e joystick em um display OLED.

## Hardware Utilizado
- **Microcontrolador**: Raspberry Pi Pico W
- **Sensores**: BMP280 (temperatura e pressão), AHT20 (temperatura e umidade)
- **Display**: OLED SSD1306 via I2C
- **Matriz de LEDs**: WS2812 (5x5)
- **LEDs RGB**: Indicadores de status
- **Buzzer**: Alerta sonoro
- **Botões e Joystick**: Para interação com o menu

## Configuração
1. **Conexões**:
   - I2C para sensores (SDA: GPIO 0, SCL: GPIO 1)
   - I2C para display (SDA: GPIO 14, SCL: GPIO 15)
   - Matriz WS2812: GPIO 7
   - Botões: GPIO configurados para interrupções
2. **Wi-Fi**:
   - SSID: ""
   - Senha: "" (ajuste conforme necessário)
3. **Compilação**:
   - Use um ambiente de desenvolvimento compatível com Pico SDK e LWIP.

## Estrutura do Código
- **`interface.h`**: Contém o HTML para a interface web e funções de servidor HTTP.
- **`main.c`**: Lógica principal, leitura de sensores, controle de alertas e atualização do display.
- **`display.h`**: Funções para controle do display OLED.
- **`wifi_init.h`**: Inicialização da conexão Wi-Fi.
- **`matrixws.h`**: Controle da matriz WS2812.
- **`reles.h`**: Gerenciamento de relés (se aplicável).

## Uso
1. Compile e grave o firmware no Raspberry Pi Pico W.
2. Conecte o dispositivo a uma rede Wi-Fi.
3. Acesse a interface web digitando o IP do dispositivo no navegador (ex.: `http://192.168.x.x`).
4. Use os botões (A, B, Joystick) para navegar entre o menu principal, monitoramento e limites no display OLED.

## Alertas
- **Temperatura ou Umidade Fora dos Limites**: Ativa o buzzer, pisca a matriz LED em vermelho e muda os LEDs RGB para vermelho.
- **Normal**: Desliga o buzzer, apaga a matriz LED e mantém os LEDs RGB verdes.


## Licença
Este projeto é de uso livre para fins educacionais e pessoais. Para uso comercial, entre em contato com os desenvolvedores.