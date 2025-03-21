#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <stdio.h>
#include <math.h>

#define lgt8f328 // clone with 12bit ADC ,compatibile atmega 328p 
#ifdef lgt8f328
#define ANALOG_FACTORY 0.00061050F //  // 2.5v / 4095 = 0.00061050061
#else
#define  ANALOG_FACTORY 0.00244379F // 2.5v / 1023
#endif

// Definiujemy piny, do których jest podłączony wyświetlacz.
#define SCLK 7
#define DIN  6
#define DC   5
#define CS   4
#define RST  3

#define VOLTAGE_DIVIDER_ERROR_FACTOR 1.2F // error factor for voltage measurnig
#define CURRENT_RESITOR_VALUE 0.1F  // nominal 0.1 ohm
#define REFERENCE_VOLT  2.5F
#define VOLTAGE_DIVIDER_FACTORY  16.0F  // all 15k ohm, 1k ohm first step  


int voltageOutputPin = A0;
int currentOutputPin = A3;

float voltageScore = 0.0f;
float currentScore = 0.0f;


// Tworzymy obiekt klasy Adafruit_PCD8544.
Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DC, CS, RST);
void setup() {
  analogReference(EXTERNAL);
  // put your setup code here, to run once:
    // Inicjujemy komunikację z wyświetlaczem.
  display.begin();
  // Ustawiamy jasność wyświetlacza.
  display.setContrast(60);
  display.setTextSize(2);
  // Czyścimy ekran.
  display.clearDisplay();
  //Serial.begin(9600);
  
}

void loop() {
  //mesure voltage on voltage divider
  int voltageVal = analogRead(voltageOutputPin);
    //mesure voltage on current resitor
  int currentVal = analogRead(currentOutputPin);

  //subtracting the current resistor from the voltage divider
  voltageVal -= currentVal; 
  float voltageScore = (voltageVal * ANALOG_FACTORY * (VOLTAGE_DIVIDER_FACTORY - VOLTAGE_DIVIDER_ERROR_FACTOR));
  String voltage = String(voltageScore )+" V";


  currentScore = (currentVal * ANALOG_FACTORY) / CURRENT_RESITOR_VALUE;
  String current = String(currentScore ) + " A";

  String power = String(voltageScore * currentScore ) + " W";
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(voltage);
  display.setCursor(0, 16);
  display.println(current);
  display.setCursor(0, 32);
  display.println(power);
  // Wyświetlamy tekst na ekranie.
  display.display();
 
  _delay_ms(50);
}