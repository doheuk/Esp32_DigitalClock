#define LED_blue 4
void setup() {
  // put your setup code here, to run once:
pinMode(LED_blue,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
digitalWrite(LED_blue,HIGH);
delay(500);
digitalWrite(LED_blue,LOW);
delay(500);
}
