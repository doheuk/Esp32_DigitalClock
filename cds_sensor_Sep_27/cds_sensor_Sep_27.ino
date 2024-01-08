#define TR2222 25
#define AIN 38

void setup() {
  // put your setup code here, to run once:
  pinMode(TR2222, OUTPUT);
  Serial.begin(9600);
  pinMode(AIN, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  double voltage;
  voltage = analogRead(AIN);
  //Serial.println(voltage);
  //delay(330);
  if(voltage <800 )
    digitalWrite(TR2222,HIGH);
    else
    digitalWrite(TR2222,LOW); 
}
