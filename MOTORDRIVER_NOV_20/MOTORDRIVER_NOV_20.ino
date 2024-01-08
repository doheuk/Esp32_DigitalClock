#define freq 5000
#define ch1 6
#define ch2 7
#define resolution 13
#define op1 22
#define op2 19
#define RRS 37

#include <LiquidCrystal.h>
#include <math.h>

const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
void setup() {
  // put your setup code here, to run once:
lcd.begin(20,4);
  ledcSetup(ch1, freq, resolution);
  ledcAttachPin(op1, ch1);
  ledcSetup(ch2, freq, resolution);
  ledcAttachPin(op2, ch2);
  pinMode(RRS,INPUT);

  lcd.setCursor(0,0);
  lcd.printf("rotation:");
  lcd.setCursor(0,1);
  lcd.printf("velocity:");
}

void loop() {
  // put your main code here, to run repeatedly:
  long rrs = analogRead(RRS);
  long rrs1,rrs2;
  if(rrs>2000&&rrs<2080){
    rrs1=rrs2=0;
    lcd.setCursor(9,0);
    lcd.printf("stop         ");
    lcd.setCursor(9,1);
    lcd.printf("%d%%            ",0);
  }
  else if(rrs>pow(2,11)-1) {
    rrs1=map(rrs,(pow(2,11)-1),(pow(2,12)-1),pow(2,resolution)*0.4,pow(2,resolution));
    rrs2=0;
    lcd.setCursor(9,0);
    lcd.printf("clockwise    ");
    lcd.setCursor(9,1);
    lcd.printf("%d%%  ",(int)((rrs1-pow(2,resolution)*0.4)/pow(2,resolution)*10000/60));
  }else{
    rrs2=map(rrs,(pow(2,11)-1),0,pow(2,resolution)*0.4,pow(2,resolution));
    rrs1=0;
    lcd.setCursor(9,0);
    lcd.printf("unclockwise");
    lcd.setCursor(9,1);
    lcd.printf("%d%%  ",(int)((rrs2-pow(2,resolution)*0.4)/pow(2,resolution)*10000/60));
  }
  ledcWrite(ch1, rrs1);
  ledcWrite(ch2, rrs2);  
  }
