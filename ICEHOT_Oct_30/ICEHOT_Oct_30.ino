#include <LiquidCrystal.h>
#define key 37
const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
char key_value;

int LONG_SHORT(){
  int count = 0;
  while(1){
    key_value = digitalRead(key);
    if(key_value==LOW){
      count++;
      delay(10);
    }
    else if((key_value == HIGH)&&(count> 10)){
      if(count < 70){
        return 0;
      }else if(count >= 70){
        return 1;
      }
    }
  }
}

void setup() {
  lcd.begin(20, 4);
  pinMode(key,INPUT);
}

void loop() {
  if(LONG_SHORT()==0){
      lcd.setCursor(7 ,1);
      lcd.print("Short Key");
      delay(500);
    }else{
      lcd.setCursor(7 ,1);
      lcd.print("Long Key ");
      delay(500);
    }
  }
 
  /*
  if(key_value == LOW){
      lcd.setCursor(7, 1);
      lcd.print("Hello");
      delay(500);
    }
    else{
      lcd.setCursor(7 ,1);
      lcd.print("     ");
      delay(500);
    }
  
  */
  
  

