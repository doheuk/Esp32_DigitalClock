#include <LiquidCrystal.h>

const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
#define AIN 37

void setup() {
  // put your setup code here, to run once:
  pinMode(AIN, INPUT);
  lcd.begin(20, 4);
  lcd.setCursor(6,1);
  lcd.print("Voltage");
}
void loop() {
  double voltage;
  
  voltage = analogRead(AIN) *3.3 / 4096.0;
  // put your main code here, to run repeatedly:
  lcd.setCursor(6,2);
  lcd.printf("%g(V)",voltage);
  delay(1000);
}
