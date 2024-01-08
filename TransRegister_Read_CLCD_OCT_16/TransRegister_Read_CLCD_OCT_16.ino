#define AIN 37 
#include <LiquidCrystal.h>

const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  // put your setup code here, to run once:
  pinMode(AIN,INPUT);
  lcd.begin(20,4);
  lcd.setCursor(0,1);
  lcd.print("Voltage Value In I37");
  lcd.setCursor(11,2);
  lcd.print("(V)");
}

void loop() {
  // put your main code here, to run repeatedly:
  double voltage = analogRead(AIN) * 3.3 / 4096.0;
  lcd.setCursor(6,2);
  lcd.printf("%4.4g",voltage);
  delay(1000);
}
