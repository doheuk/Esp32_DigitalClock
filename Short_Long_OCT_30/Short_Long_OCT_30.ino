#include <LiquidCrystal.h>
#define key 37
const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int key_value;

int LONG_SHORT();
void MENU(int i);
void ICE_HOT(int i);

void setup() {
  // put your setup code here, to run once:
  pinMode(key,INPUT);
  lcd.begin(20,4);
}
int n = 0;
int mode = 0;
void loop() {
  key_value = LONG_SHORT();
  if((key_value == 0)&&(mode==0)) {
    n = (n+1)%2;
  } else if(key_value ==1){
    lcd.clear();
    mode = 1;
  } else if(key_value ==0 && mode==1){
    mode = 0;
  }
  if(mode==1){
    ICE_HOT(n);
  }
   else if(mode==0){
     MENU(n);
  }
}
void MENU(int i) {
  lcd.setCursor(5,1);
  lcd.print("ICE Coffee");
  lcd.setCursor(5,2);
  lcd.print("HOT Coffee");
  delay(500);
  lcd.setCursor(5,i+1);
  lcd.print("          ");
  delay(500);
}
void ICE_HOT(int i) {
  lcd.setCursor(5,1);
  if(i == 0){
    lcd.print("ICE Coffee");
    lcd.setCursor(5,2);
    lcd.print("4500");
  }else if(i==1){
    lcd.print("HOT Coffee");
    lcd.setCursor(5,2);
    lcd.print("5500");
  }
  delay(500);
  lcd.setCursor(5,2);
  lcd.print("    ");
  delay(500);
}

int LONG_SHORT(){
  int count = 0;
  char a;
  while(1){
    a = digitalRead(key);
    if(a==LOW){
      count++;
      delay(10);
    } else if((a==HIGH)&&(count!=0)){
      if(count<70)
        return 0;
      else if(count >= 70)
      return 1;
    }else
    return -1;
  }
}