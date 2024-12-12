#include <Arduino.h>

#define CORDIC_ITERATIONS 16  // Die Anzahl der Iterationen für die CORDIC-Berechnung
#define PI 3.14159265358979

// CORDIC Parameter
int32_t K[CORDIC_ITERATIONS] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
int32_t angleTable[CORDIC_ITERATIONS] = {45, 26, 14, 8, 5, 3, 2, 1, 1, 1, 0, 0, 0, 0, 0, 0};  // Werte in Grad

// Sinus und Cosinus Initialisierung für CORDIC
int32_t x = 1024;  // Anfangswert für X (Einheitskreis)
int32_t y = 0;     // Anfangswert für Y
int32_t z = 0;     // Anfangswert für den Winkel (Phase)

// AB-Encoder-Signal
int encoderA = 2;
int encoderB = 3;

void setup() {
  Serial.begin(115200);
  pinMode(encoderA, OUTPUT);
  pinMode(encoderB, OUTPUT);
}

void loop() {
  // Beispielhafte Sinus- und Cosinus-Werte für die X- und Y-Position
  int16_t sinX = 512;  // Beispielwert für das Sinussignal der X-Achse
  int16_t cosY = 512;  // Beispielwert für das Cosinussignal der Y-Achse

  // CORDIC Algorithmus zur Berechnung des Winkels
  for (int i = 0; i < CORDIC_ITERATIONS; i++) {
    // Berechnungen nach der CORDIC-Methode
    if (y < 0) {
      // Rotieren gegen den Uhrzeigersinn
      x = x + (y >> i);
      y = y - (x >> i);
      z -= angleTable[i];  // Winkel umkehren
    } else {
      // Rotieren im Uhrzeigersinn
      x = x - (y >> i);
      y = y + (x >> i);
      z += angleTable[i];  // Winkel erhöhen
    }
  }

  // Der Wert von z ist der berechnete Winkel
  // Umwandlung in Encoder-A/B-Signale
  int encoderPosition = z / 360;  // Umwandlung in vollständige Umdrehungen

  // Setze die AB-Encoder-Signale basierend auf der Position
  if (encoderPosition % 2 == 0) {
    digitalWrite(encoderA, HIGH);
    digitalWrite(encoderB, LOW);
  } else {
    digitalWrite(encoderA, LOW);
    digitalWrite(encoderB, HIGH);
  }

  // Ausgabe des berechneten Winkels (Position)
  Serial.print("Winkel: ");
  Serial.println(z);
  
  delay(100);  // Verzögerung für Testzwecke
}
