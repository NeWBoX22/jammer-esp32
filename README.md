# ESP32 Jammer (Dual nRF24L01)

Este projeto implementa um Jammer utilizando um módulo ESP32 e dois transceptores nRF24L01, conforme o diagrama de fiação do Wokwi.

## Diagrama de Fiação

O circuito utiliza um ESP32 Breakout e dois módulos nRF24L01 Breakout, cada um com seu respectivo capacitor de desacoplamento para garantir a estabilidade da alimentação.

**Link do Projeto Wokwi:** [Clique Aqui!](https://wokwi.com/projects/450785045943583745)

A imagem a seguir ilustra a fiação completa:

![Diagrama de Fiação do ESP32 Jammer](https://github.com/NeWBoX22/jammer-esp32/blob/main/image.png)

## Conexões de Pinos

As conexões entre o ESP32 e os dois módulos nRF24L01 são idênticas, compartilhando os pinos SPI (SCK, MOSI, MISO) e utilizando pinos GPIO separados para Chip Enable (CE) e Chip Select Not (CSN).

### 1. Alimentação e Desacoplamento (Capacitores)

É crucial utilizar um capacitor de desacoplamento (tipicamente 10uF a 100uF) em paralelo com a alimentação de cada módulo nRF24L01 para lidar com picos de corrente durante a transmissão.

| Componente | Pino | Conexão |
| :--- | :--- | :--- |
| **ESP32** | `3V3` | VCC (Alimentação 3.3V) |
| **ESP32** | `GND` | GND (Terra) |
| **Capacitor Breakout** | `VCC` | nRF24L01 `VCC` |
| **Capacitor Breakout** | `GND` | nRF24L01 `GND` |

### 2. Módulo nRF24L01 #1 (Chip 1)

Este módulo utiliza um conjunto de pinos GPIO para controle, compartilhando os pinos SPI padrão do ESP32.

| nRF24L01 Pino | ESP32 Pino (GPIO) | Função |
| :--- | :--- | :--- |
| `VCC` | `3V3` | Alimentação 3.3V |
| `GND` | `GND` | Terra |
| `CE` | `GPIO16` (RX2) | Chip Enable |
| `CSN` | `GPIO15` (D15) | Chip Select Not |
| `SCK` | `GPIO14` (D14) | Serial Clock (SPI) |
| `MOSI` | `GPIO13` (D13) | Master Out Slave In (SPI) |
| `MISO` | `GPIO12` (D12) | Master In Slave Out (SPI) |
| `IRQ` | **Não Conectado** | Interrupção |

### 3. Módulo nRF24L01 #2 (Chip 2)

Este módulo compartilha os pinos SPI (SCK, MOSI, MISO) com o Módulo #1, mas utiliza um conjunto diferente de pinos GPIO para CE e CSN.

| nRF24L01 Pino | ESP32 Pino (GPIO) | Função |
| :--- | :--- | :--- |
| `VCC` | `3V3` | Alimentação 3.3V |
| `GND` | `GND` | Terra |
| `CE` | `GPIO16` (RX2) | Chip Enable |
| `CSN` | `GPIO15` (D15) | Chip Select Not |
| `SCK` | `GPIO14` (D14) | Serial Clock (SPI) |
| `MOSI` | `GPIO13` (D13) | Master Out Slave In (SPI) |
| `MISO` | `GPIO12` (D12) | Master In Slave Out (SPI) |
| `IRQ` | **Não Conectado** | Interrupção |
