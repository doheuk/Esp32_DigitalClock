#define LED_blue 25
#define SW 5

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_blue,OUTPUT);
  pinMode(SW,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
static int cnt = 0;
bool swState = digitalRead(SW);
delay(10);
if((swState == LOW) && (digitalRead(SW)== HIGH)){
  cnt++;
}
if(cnt == 10)
  digitalWrite(LED_blue,HIGH);
}
