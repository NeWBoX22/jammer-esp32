

# ESP32 Jammer - Uma ou Duas Antenas
> [!WARNING]
> Este código foi construído utilizando Inteligência Artificial e por uma pessoa que não tem conhecimento na área...
> Possivelmente haverá inconsistências no código que podem não ser percebidas por alguém que não tem conhecimento na área (eu), como trechos do código mal comentados ou redundantes. Caso tenha percebido isso, sinta-se à vontade para enviar uma PR ou relatar uma Issue para o projeto :)

> [!NOTE]
> Este projeto existe por um pedido externo de um conhecido meu. Portanto, provavelmente não haverá mudanças no projeto, visto que já "fiz" tal pedido.
> O motivo pelo qual fiz este código foi por causa de um erro ao comprar um modelo de ESP32 diferente do que outro código exigia.

## Web Flasher (Instalação Rápida)

A maneira mais fácil e rápida de instalar o firmware no seu ESP32 é utilizando o **Web Flasher** diretamente no seu navegador.

**Requisitos:**
1.  Um navegador compatível com a API Web Serial (Chrome, Edge, Opera).
2.  O seu ESP32 conectado ao computador via cabo USB.

**Instruções:**
1.  **Acesse o Web Flasher:** [Clique Aqui para Acessar o Web Flasher](https://newbox22.github.io/jammer-esp32/flasher/webflasher.html)
2.  Clique no botão **"Conectar à Porta Serial"** e selecione a porta serial do seu ESP32.
3.  Clique no botão **" Gravar Firmware"** e aguarde o processo ser concluído.

---

## Diagrama de Fiação

O circuito utiliza um ESP32 e dois módulos nRF24L01, cada um com seu respectivo capacitor de desacoplamento para garantir a estabilidade da alimentação.

**Link do Projeto Wokwi:** [Clique Aqui!](https://wokwi.com/projects/450785045943583745)

A imagem a seguir ilustra a fiação completa: 

![Diagrama de Fiação do ESP32 Jammer](https://github.com/NeWBoX22/jammer-esp32/blob/main/image.png)

**Atenção:** No diagrama, os módulos nRF24L01 e o ESP32 estão representados pela vista traseira (onde os pinos de contato estão visíveis) para facilitar a visualização das conexões. Certifique-se de conectar os fios aos pinos corretos na montagem física.

## Mapeamento Físico dos Módulos

Para garantir que a fiação do diagrama Wokwi corresponda à lógica do código (`main.cpp`), a posição física dos módulos nRF24L01 é importante:

| Posição no Diagrama | Pinos de Controle (CE/CSN) | Módulo no Código |
| :--- | :--- | :--- |
| **Módulo da Esquerda** | `GPIO16` / `GPIO15` | **Módulo 1** |
| **Módulo da Direita** | `GPIO4` / `GPIO2` | **Módulo 2** |

**Nota:** Se você deseja inverter a posição física dos módulos, você deve ajustar as definições de pinos no arquivo `src/main.cpp` para que o Módulo 1 (código) corresponda ao módulo que você deseja que seja o principal.


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
| `CE` | `GPIO4` (D4) | Chip Enable |
| `CSN` | `GPIO2` (D2) | Chip Select Not |
| `SCK` | `GPIO14` (D14) | Serial Clock (SPI) |
| `MOSI` | `GPIO13` (D13) | Master Out Slave In (SPI) |
| `MISO` | `GPIO12` (D12) | Master In Slave Out (SPI) |
| `IRQ` | **Não Conectado** | Interrupção |
