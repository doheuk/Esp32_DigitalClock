#define TR2222 25
#define SW 5

void setup() {
  // put your setup code here, to run once:
  pinMode(TR2222,OUTPUT);
  pinMode(SW,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
static int cnt = 0;
static bool swState = LOW;
if(swState != digitalRead(SW)){
  delay(10);
  cnt++;
  swState = !swState;
}
if(cnt== 10)
  digitalWrite(TR2222,HIGH);
}