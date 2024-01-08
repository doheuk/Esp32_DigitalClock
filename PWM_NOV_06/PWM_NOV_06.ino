#define freq 5000
#define channel 6
#define resolution 10
#define out_pin 22
#define duty_value 511 //50?% 

const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // put your setup code here, to run once:
  lcd.begin(20,4);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(out_pin, channel);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledcWrite(channel, 0);
  delay(500);
  ledcWrite(channel, 255);
  delay(500);
  ledcWrite(channel, 511);
  delay(500);
  ledcWrite(channel, 765);
  delay(500);
  ledcWrite(channel, 1023);
  delay(500);
}
