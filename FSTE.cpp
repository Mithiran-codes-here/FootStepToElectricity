#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   

const int sensePin = A0;              
const int ledPin   = 8;               
long stepCount = 0;

int prevAdc = 0;
unsigned long previousMillis = 0;

void setup() {
  pinMode(ledPin, OUTPUT);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("FOOT STEP POWER");
  lcd.setCursor(0, 1);
  lcd.print("GENERATOR");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STEP:");
  lcd.setCursor(0, 1);
  lcd.print("VOLT:");
  Serial.begin(9600);
}

void loop() {
  int adc = analogRead(sensePin);

  // simple edge-detect for step counting
  if (adc > 10 && prevAdc <= 10) {   // threshold; adjust if needed
    stepCount++;
    digitalWrite(ledPin, HIGH);
    previousMillis = millis();
  }

  // auto‑off LED after 100 ms
  if (millis() - previousMillis > 100) {
    digitalWrite(ledPin, LOW);
  }

  // convert ADC to sensed voltage
  float vOut = (adc * 5.0) / 1023.0;   // voltage at A0
  // if you use 100k (top) and 10k (bottom) divider:
  float vIn = vOut * (100.0 + 10.0) / 10.0;   // = vOut * 11

  // update LCD
  lcd.setCursor(6, 0);
  lcd.print("       ");
  lcd.setCursor(6, 0);
  lcd.print(stepCount);

  lcd.setCursor(6, 1);
  lcd.print("       ");
  lcd.setCursor(6, 1);
  lcd.print(vIn, 2);
  lcd.print("V");

  prevAdc = adc;
  delay(100);
}
