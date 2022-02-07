#include <CircularBuffer.h>
#include <MAX30100.h>
#include <MAX30100_BeatDetector.h>
#include <MAX30100_Filters.h>
#include <MAX30100_PulseOximeter.h>
#include <MAX30100_Registers.h>
#include <MAX30100_SpO2Calculator.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <LiquidCrystal.h>
#include "MAX30100_PulseOximeter.h"
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define REPORTING_PERIOD_MS 1000
int c = 0, m = 0;
PulseOximeter pox;
uint32_t tsLastReport = 0;
void onBeatDetected()
{
  Serial.println("Beat!");
}
void setup()
{
  lcd.begin(16, 2);
  lcd.print("Initializing...");
  delay(10);
  delay(3000);
  lcd.clear();
  pox.setIRLedCurrent(MAX30100_LED_CURR_24MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}
void loop()
{
  pox.update();
  if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Heart rate:");
    double heart = pox.getHeartRate();
    lcd.print(heart);
    lcd.setCursor(0, 1);
    lcd.print("bpm / SpO2:");
    double o = pox.getSpO2();
    lcd.print(o);
    lcd.println("%");
    m++;
    if (heart > 100 && o < 93 ) {
      c++;
    }
    tone(6, 2000, 200);
    tsLastReport = millis();
  }
  if (m == 12 && (c == 6)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("COVID-19: Suspected");
    tone(6, 7000, 200);
    delay (3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    c = 0; m = 0;
  }
  else if (m == 12) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("You are fine");
    delay(3000);
    lcd.clear();
    lcd.setCursor(0, 0);
    c = 0; m = 0;
  }
}
