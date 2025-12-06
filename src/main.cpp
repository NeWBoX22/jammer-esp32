// Novo Código Otimizado para ESP32 e PlatformIO
// Objetivo: Operação contínua de varredura de canal (Sweep) ou salto de canal (Hopping)
// usando nRF24L01+ e um botão (ezButton).

#include <Arduino.h>
#include <SPI.h>
#include "RF24.h"
#include <ezButton.h>
#include "esp_bt.h"
#include "esp_wifi.h"

// --- Configuração de Pinos ---
// O ESP32 possui 3 barramentos SPI (VSPI, HSPI, FSPI).
// Usaremos o HSPI, que é o mais comum para periféricos externos.
// Pinos padrão do HSPI no ESP32 (podem variar dependendo da placa):
#define HSPI_SCK 14
#define HSPI_MISO 12
#define HSPI_MOSI 13

// Pinos do nRF24L01+ (CE e CSN)
// Usando os mesmos pinos do seu código original: GPIO 16 (CE) e GPIO 15 (CSN)
#define RF24_CE_PIN 16
#define RF24_CSN_PIN 15

// Pino do botão (ezButton)
#define TOGGLE_BUTTON_PIN 33

// --- Variáveis Globais ---
SPIClass *hspi = nullptr; // Usaremos 'hspi' para maior clareza
RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);

// Variáveis para o Hopping
byte current_channel = 45;
bool hopping_up = true; // Flag para controlar se está subindo ou descendo o canal

// Variável para o botão
ezButton toggleSwitch(TOGGLE_BUTTON_PIN);

// --- Protótipos de Funções (Para compatibilidade com PlatformIO/C++) ---
void initRadio();
void channelHopping();
void channelSweep();

// --- Setup ---
void setup() {
  // Desinicializa Bluetooth e Wi-Fi para liberar recursos e evitar interferência
  esp_bt_controller_deinit();
  esp_wifi_stop();
  esp_wifi_deinit();
  
  Serial.begin(115200);
  Serial.println("Iniciando ESP32 e nRF24L01+...");
  
  // Configuração do botão
  toggleSwitch.setDebounceTime(50);
  
  // Inicialização do rádio
  initRadio();
}

// --- Inicialização do Rádio ---
void initRadio() {
  // Configura o barramento HSPI
  hspi = new SPIClass(HSPI);
  // Inicia o HSPI com os pinos definidos (SCK, MISO, MOSI, CSN)
  hspi->begin(HSPI_SCK, HSPI_MISO, HSPI_MOSI, RF24_CSN_PIN); 
  
  // Inicia o rádio nRF24L01+ usando o barramento HSPI
  if (radio.begin(hspi)) {
    delay(200);
    Serial.println("Rádio nRF24L01+ iniciado com sucesso no HSPI!");
    
    // Configurações do rádio
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPayloadSize(5);   
    radio.setAddressWidth(3);  
    radio.setPALevel(RF24_PA_MAX, true);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    
    // Inicia a portadora constante (Constant Carrier)
    // Mantenho a função original, mas com um aviso:
    // **ATENÇÃO:** startConstCarrier é para testes de conformidade e não para comunicação normal.
    radio.startConstCarrier(RF24_PA_MAX, current_channel);
    
    radio.printPrettyDetails();
  } else {
    Serial.println("Falha ao iniciar o rádio nRF24L01+! Verifique as conexões.");
  }
}

// --- Lógica de Salto de Canal (Hopping) ---
void channelHopping() {
  // Salto de canal com espaçamento de 2 (2MHz)
  if (hopping_up) {
    current_channel += 2;
  } else {
    current_channel -= 2;
  }

  // Inverte a direção ao atingir os limites (2 a 79)
  if (current_channel > 79) {
    hopping_up = false;
    current_channel = 79; // Garante que não ultrapasse o limite
  } else if (current_channel < 2) {
    hopping_up = true;
    current_channel = 2; // Garante que não ultrapasse o limite
  }

  radio.setChannel(current_channel);
  // Serial.print("Hopping: ");
  // Serial.println(current_channel);
}

// --- Lógica de Varredura de Canal (Sweep) ---
void channelSweep() {
  // Varre todos os canais de 0 a 79
  // **ATENÇÃO:** Esta varredura é extremamente rápida e pode não ser o que você deseja.
  // Para varredura visual ou com tempo de permanência, a lógica deve ser ajustada.
  for (int j = 0; j < 80; j++) {
    radio.setChannel(j);
  }
  // Serial.println("Sweep Completo");
}

// --- Loop Principal (Operação Contínua) ---
void loop() {
  // Processa o estado do botão
  toggleSwitch.loop();
  
  // Verifica o estado do botão (HIGH ou LOW)
  int state = toggleSwitch.getState();

  if (state == HIGH) {
    // Se o botão estiver pressionado (ou no estado HIGH), faz o Hopping
    channelHopping();
  } else {
    // Se o botão estiver solto (ou no estado LOW), faz o Sweep
    channelSweep();
  }
  
  // O loop é executado o mais rápido possível para garantir a operação contínua.
}
