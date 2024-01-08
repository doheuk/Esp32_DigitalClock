#include <LiquidCrystal.h>

const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  // put your setup code here, to run once:
  lcd.begin(20,4);
  Serial.begin(9600);
}
char RCV_data, pc;
void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
    RCV_data = Serial.read();
    if(pc == 'O' && RCV_data == 'N'){
      lcd.clear();
      lcd.setCursor(1, 1);
      lcd.print("(ON)");
    } else if(pc == 'O' && RCV_data == 'F'){
      lcd.setCursor(1, 1);
      lcd.print("(OFF)");
    }
    pc = RCV_data;
  }
  delay(100);
}
