#define BLYNK_TEMPLATE_ID "TMPL3NEiga_p8"
#define BLYNK_TEMPLATE_NAME "energy meter"
#define BLYNK_AUTH_TOKEN "kbbWOeC8GzEUADD-TnVY2enwo7W_IvLh"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <LiquidCrystal.h>

// Wi-Fi credentials
char ssid[] = "Prayag";
char pass[] = "12345678";

// LCD Pins
const int rs = D2, en = D3, d4 = D4, d5 = D5, d6 = D6, d7 = D7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Analog and Relay Pins
const int volValue = A0;
int readValue, count = 0, unit = 0, flag = 0;
float Voltage, Value;

void setup() {
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass, "blynk.cloud", 80);

  lcd.begin(16, 2);

  pinMode(volValue, INPUT);
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);

  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
}

void updateDisplay() {
  lcd.setCursor(0, 0);
  lcd.print("CA:");
  lcd.setCursor(0, 1);
  lcd.print(Value, 2);
  
  lcd.setCursor(5, 0);
  lcd.print("Count:");
  lcd.setCursor(6, 1);
  lcd.print(count);

  lcd.setCursor(12, 0);
  lcd.print("Unit:");
  lcd.setCursor(13, 1);
  lcd.print(unit);
}

void generateBill() {
  int Amount = unit * 7;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Bill generated:");
  Blynk.virtualWrite(V4, "Bill Generated:");
  Blynk.virtualWrite(V4, "\n");

  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Total Bill:");
  lcd.setCursor(0, 1);
  lcd.print(Amount);
  Blynk.virtualWrite(V3, "Total Bill:");
  Blynk.virtualWrite(V3, Amount);
  Blynk.virtualWrite(V3, "\n");

  delay(2000);
  unit = 0;
  count = 0;
}

void loop() {
  Blynk.run();

  // Read voltage and calculate current
  readValue = analogRead(volValue);
  Voltage = (5.0 / 1023) * readValue;
  Value = Voltage / 10.680;

  // Update the display
  updateDisplay();
  delay(500);

  // Count increment logic
  if (Value >= 0.01 && Value <= 0.02) {
    count++;
    delay(3000);
  } else if (Value >= 0.03 && Value <= 0.06) {
    count++;
    delay(1000);
  } else if (Value >= 0.07 && Value <= 0.10) {
    count++;
    delay(500);
  }

  // Unit increment logic
  if (count >= 5) {
    unit++;
    count = 0;
  }

  // Bill generation logic
  if (unit >= 7) {
    generateBill();
  }
}

// Blynk virtual pin writes for relay control
BLYNK_WRITE(V0) {
  if (param.asInt() == 1) {
    digitalWrite(D0, LOW);
  } else {
    digitalWrite(D0, HIGH);
  }
}

BLYNK_WRITE(V1) {
  if (param.asInt() == 1) {
    digitalWrite(D1, LOW);
  } else {
    digitalWrite(D1, HIGH);
  }
}

BLYNK_WRITE(V2) {
  if (param.asInt() == 1) {
    flag = 1;
  }
}

