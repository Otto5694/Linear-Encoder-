#include <Arduino.h>
#include "stm32g4xx_hal.h"

// Puffer zum Speichern der ADC-Werte
uint16_t adcBuffer[6]; // 6 Kanäle

// ADC Handle für alle 3 ADCs
ADC_HandleTypeDef hadc1, hadc2, hadc3;
DMA_HandleTypeDef hdma_adc1, hdma_adc2, hdma_adc3;

void setup() {
  Serial.begin(115200);

  // Systemtakt auf 170 MHz setzen (normalerweise im Systemstart konfiguriert)
  SystemClock_Config();

  // ADCs konfigurieren
  ADC_Config();

  // DMA konfigurieren
  DMA_Config();

  // Alle ADCs starten
  ADC_Start();
}

void loop() {
  // Hier kannst du die ADC-Werte verarbeiten, die im Puffer gespeichert sind
  for (int i = 0; i < 6; i++) {
    Serial.print(adcBuffer[i]);
    Serial.print("\t");
  }
  Serial.println();

  delay(100);  // Einfacher Delay, damit die Ausgabe lesbar bleibt
}

// Konfiguriere die 3 ADCs (ADC1, ADC2, ADC3) für parallelen Betrieb
void ADC_Config() {
  // ADC1 (PA0, PA1)
  __HAL_RCC_ADC12_CLK_ENABLE();
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 2;  // PA0 und PA1
  hadc1.Init.DMAContinuousRequests = ENABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK) {
    Serial.println("ADC1 Fehler");
  }

  // ADC2 (PA2, PA3)
  __HAL_RCC_ADC12_CLK_ENABLE();
  hadc2.Instance = ADC2;
  hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc2.Init.Resolution = ADC_RESOLUTION_12B;
  hadc2.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc2.Init.ContinuousConvMode = ENABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 2;  // PA2 und PA3
  hadc2.Init.DMAContinuousRequests = ENABLE;
  if (HAL_ADC_Init(&hadc2) != HAL_OK) {
    Serial.println("ADC2 Fehler");
  }

  // ADC3 (PA4, PA5)
  __HAL_RCC_ADC12_CLK_ENABLE();
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_ENABLE;
  hadc3.Init.ContinuousConvMode = ENABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 2;  // PA4 und PA5
  hadc3.Init.DMAContinuousRequests = ENABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK) {
    Serial.println("ADC3 Fehler");
  }
}

// DMA für alle ADCs konfigurieren
void DMA_Config() {
  __HAL_RCC_DMA1_CLK_ENABLE();  // DMA1 aktivieren

  // DMA für ADC1
  hdma_adc1.Instance = DMA1_Channel1;
  hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc1.Init.Mode = DMA_CIRCULAR;
  hdma_adc1.Init.Priority = DMA_PRIORITY_HIGH;
  if (HAL_DMA_Init(&hdma_adc1) != HAL_OK) {
    Serial.println("DMA1 Fehler");
  }
  __HAL_LINKDMA(&hadc1, DMA_Handle, hdma_adc1);

  // DMA für ADC2
  hdma_adc2.Instance = DMA1_Channel2;
  hdma_adc2.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc2.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc2.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc2.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc2.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc2.Init.Mode = DMA_CIRCULAR;
  hdma_adc2.Init.Priority = DMA_PRIORITY_HIGH;
  if (HAL_DMA_Init(&hdma_adc2) != HAL_OK) {
    Serial.println("DMA2 Fehler");
  }
  __HAL_LINKDMA(&hadc2, DMA_Handle, hdma_adc2);

  // DMA für ADC3
  hdma_adc3.Instance = DMA1_Channel3;
  hdma_adc3.Init.Direction = DMA_PERIPH_TO_MEMORY;
  hdma_adc3.Init.PeriphInc = DMA_PINC_DISABLE;
  hdma_adc3.Init.MemInc = DMA_MINC_ENABLE;
  hdma_adc3.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma_adc3.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  hdma_adc3.Init.Mode = DMA_CIRCULAR;
  hdma_adc3.Init.Priority = DMA_PRIORITY_HIGH;
  if (HAL_DMA_Init(&hdma_adc3) != HAL_OK) {
    Serial.println("DMA3 Fehler");
  }
  __HAL_LINKDMA(&hadc3, DMA_Handle, hdma_adc3);
}

// Alle 3 ADCs starten und DMA aktivieren
void ADC_Start() {
  if (HAL_ADC_Start_DMA(&hadc1, (uint32_t*)adcBuffer, 2) != HAL_OK) {
    Serial.println("Fehler beim Starten von ADC1");
  }
  if (HAL_ADC_Start_DMA(&hadc2, (uint32_t*)adcBuffer + 2, 2) != HAL_OK) {
    Serial.println("Fehler beim Starten von ADC2");
  }
  if (HAL_ADC_Start_DMA(&hadc3, (uint32_t*)adcBuffer + 4, 2) != HAL_OK) {
    Serial.println("Fehler beim Starten von ADC3");
  }
}
