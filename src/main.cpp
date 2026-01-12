// Código Otimizado para ESP32 e PlatformIO com Detecção Automática de Rádio
// Objetivo: Operação contínua de varredura/salto de canal, suportando 1 ou 2 módulos nRF24L01+.

#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include <ezButton.h>
#include "esp_bt.h"
#include "esp_wifi.h"

// --- Configuração de Pinos ---
// O ESP32 possui 3 barramentos SPI (VSPI, HSPI, FSPI). Usaremos o HSPI.
#define HSPI_SCK 14
#define HSPI_MISO 12
#define HSPI_MOSI 13

// Pinos do Módulo 1 (Baseado no código original do usuário)
#define RF24_CE_PIN_1 16
#define RF24_CSN_PIN_1 15

// Pinos do Módulo 2 (Usando GPIOs livres da sua pinagem: D4 e D2)
#define RF24_CE_PIN_2 4
#define RF24_CSN_PIN_2 2

// Pino do botão (ezButton)
#define TOGGLE_BUTTON_PIN 33

// --- Variáveis Globais ---
SPIClass *hspi = nullptr; 
RF24 radio1(RF24_CE_PIN_1, RF24_CSN_PIN_1);
RF24 radio2(RF24_CE_PIN_2, RF24_CSN_PIN_2);

// Variável para armazenar o número de rádios detectados (0, 1 ou 2)
int num_radios = 0; 

// Variáveis para o Hopping
byte current_channel = 45;
bool hopping_up = true; 

// Variável para o botão
ezButton toggleSwitch(TOGGLE_BUTTON_PIN);

// --- Protótipos de Funções ---
void initializeRadio(RF24& radio_obj, const char* name);
void setupRadios();
void channelHopping();
void channelSweep();

// --- Setup ---
void setup() {
  // Desinicializa Bluetooth e Wi-Fi
  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  
  Serial.begin(115200);
  Serial.println("Iniciando ESP32 e nRF24L01+...");
  
  // Configuração do botão
  toggleSwitch.setDebounceTime(50);
  
  // Configura o barramento HSPI uma única vez
  hspi = new SPIClass(HSPI);
  hspi->begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, RF24_CSN_PIN_1); // CSN_1 é o CSN padrão
  
  // Tenta inicializar e detectar os rádios
  setupRadios();
}

// --- Inicialização de um Rádio (OTIMIZADO para 250KBPS) ---
void initializeRadio(RF24& radio_obj, const char* name) {
  if (radio_obj.begin(hspi)) {
    Serial.printf("Rádio %s detectado e configurado para 250KBPS!\n", name);
    
    // CONFIGURAÇÕES OTIMIZADAS PARA MÁXIMO ALCANCE:
    radio_obj.setAutoAck(false);
    radio_obj.stopListening();
    radio_obj.setRetries(0, 0);
    radio_obj.setPayloadSize(5);          // Mantido 5 para compatibilidade
    radio_obj.setAddressWidth(3);  
    radio_obj.setPALevel(RF24_PA_MAX);    // Máxima potência para PA+LNA
    
    // ALTERAÇÃO PRINCIPAL: 250KBPS para máximo alcance
    radio_obj.setDataRate(RF24_250KBPS);  // Reduzido de 2MBPS para 250KBPS
    
    radio_obj.setCRCLength(RF24_CRC_DISABLED);  // Sem CRC para potência máxima
    
    // IMPORTANTE: Para 250KBPS, ajuste o tempo de estabilização
    radio_obj.setChannel(current_channel);
    
    // Inicia a portadora constante
    radio_obj.startConstCarrier(RF24_PA_MAX, current_channel);
    
    num_radios++;
  } else {
    Serial.printf("Rádio %s não detectado.\n", name);
  }
}

// --- Detecção e Configuração dos Rádios ---
void setupRadios() {
  // Tenta inicializar o Módulo 1 (Pinos originais)
  initializeRadio(radio1, "Módulo 1 (GPIO 16/15)");
  
  // Tenta inicializar o Módulo 2 (Pinos adicionais)
  // Nota: O Módulo 2 usa os mesmos pinos SPI (SCK, MISO, MOSI) do Módulo 1.
  initializeRadio(radio2, "Módulo 2 (GPIO 4/2)");
  
  Serial.printf("Total de Rádios Detectados: %d\n", num_radios);
  if (num_radios == 0) {
    Serial.println("Nenhum rádio detectado. O programa não fará varredura/salto.");
  }
}

// --- Lógica de Salto de Canal (Hopping) ---
void channelHopping() {
  // Atualiza o canal
  if (hopping_up) {
    current_channel += 2;
  } else {
    current_channel -= 2;
  }

  if (current_channel > 79) {
    hopping_up = false;
    current_channel = 79; 
  } else if (current_channel < 2) {
    hopping_up = true;
    current_channel = 2; 
  }

  // Aplica o canal a todos os rádios detectados
  if (num_radios >= 1) {
    radio1.setChannel(current_channel);
  }
  if (num_radios == 2) {
    radio2.setChannel(current_channel);
  }
  
  // Ajuste: delay maior para 250KBPS (estabilidade)
  delayMicroseconds(400);  // Aumentado de ~200 para 400µs
}

// --- Lógica de Varredura de Canal (Sweep) ---
void channelSweep() {
  // Varre todos os canais de 0 a 79
  for (int j = 0; j < 80; j++) {
    if (num_radios >= 1) {
      radio1.setChannel(j);
    }
    if (num_radios == 2) {
      radio2.setChannel(j);
    }
    // Ajuste: delay otimizado para 250KBPS
    delayMicroseconds(300);  // Reduzido de 400 para 300µs para sweep mais rápido
  }
}

// --- Loop Principal ---
void loop() {
  // Processa o estado do botão
  toggleSwitch.loop();
  
  if (num_radios == 0) {
    delay(1000);  // Pausa se não houver rádios
    return;
  }

  // Verifica o estado do botão (HIGH ou LOW)
  int state = toggleSwitch.getState();

  if (state == HIGH) {
    // Se o botão estiver pressionado, faz o Hopping
    channelHopping();
  } else {
    // Se o botão estiver solto, faz o Sweep
    channelSweep();
  }
  
  // O loop é executado o mais rápido possível para garantir a operação contínua.
}
