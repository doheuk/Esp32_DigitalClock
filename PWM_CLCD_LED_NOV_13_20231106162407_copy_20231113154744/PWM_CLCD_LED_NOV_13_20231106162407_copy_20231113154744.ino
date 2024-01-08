#define freq 5000
#define channel 6
#define resolution 13
#define out_pin 22
#define VIN 37

#include <LiquidCrystal.h>
#include <math.h>

const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20,4);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(out_pin, channel);
  pinMode(VIN,INPUT);
  lcd.setCursor(0,0);
  lcd.printf("Freq:%dHz",freq);
  lcd.setCursor(0,1);
  lcd.printf("Resolution:%d",resolution);
  lcd.setCursor(0,2);
  lcd.printf("Duty:");

}
float vr;
float duty_value = (pow(2, resolution)-1)/100;
void loop() {
  vr = analogRead(VIN)/4096.0*100;
  lcd.setCursor(5,2);
  lcd.printf("%d%%   ",(int)vr);
  ledcWrite(channel, (int)(duty_value*vr));
}
