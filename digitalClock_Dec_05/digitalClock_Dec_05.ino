#include <LiquidCrystal.h>
#include <math.h>
#include <Ticker.h>

#define Freq 5000
#define Ch 6
#define Resolution 13
#define Outpin 22
#define Button1 39 
#define Button2 34
#define Button3 35
#define LED 12
#define CDC 37 //3800 

void nextDay();
void timerIsr();
int getMaxday(int month);
int isYun(int year);
void btnIn();
void mainScreen();
void menuScreen();
void alarmScreen();
void printYoil();
void setTimeScreen();
const int rs = 33, en = 26, d4 = 13, d5 = 15, d6 = 0, d7 = 14;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

struct Time {
  int hour;
  int min;
  int sec;
};
struct Date {
  int year;
  int month;
  int day;
  int yoil;
};
struct Alarm {
  int hour;
  int min;
  int act;
}

Ticker timer;

void setup() {
  // put your setup code here, to run once:
  timer.attach(1, timerIsr);
  pinMode(Button1,INPUT);
  pinMode(Button2,INPUT);
  pinMode(Button3,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(CDC, INPUT);
  attachInterrupt(Button1, btnIn, FALLING);
  attachInterrupt(Button2, btnIn, FALLING);
  attachInterrupt(Button3, btnIn, FALLING);
  lcd.begin(20,4);
  ledcSetup(Ch, Freq, Resolution);
  ledcAttachPin(Outpin, Ch);
  Serial.begin(9600);
}

struct Time nowTime = {0,0,0};
struct Date nowDate = {2023,12,5,4};
int btn1 = 0, btn2 = 0, btn3 = 0;
int state = 0, menu_select = 0;
void loop() {
  switch(state) {
    case 0: // main
      mainScreen();
      break;
    case 8: // menu
      menuScreen();
      break;
    case 1: // setTimeDate
      setTimeScreen();
      break;
    case 2:
      alarmScreen();
  }
  int v = analogRead(CDC);
  if(v > 500)
    digitalWrite(LED,HIGH);
  else
    digitalWrite(LED,LOW);
}

void mainScreen(){
  lcd.setCursor(6,0);
  lcd.printf("%04d/%02d/%02d ",nowDate.year,nowDate.month,nowDate.day);
  printYoil();
  lcd.setCursor(0,1);
  lcd.printf("%02d:%02d:%02d",nowTime.hour,nowTime.min,nowTime.sec);
  if(btn2) {
    lcd.clear();
    state = 8;
    btn2 = 0;
  }
}
int alarm_select = 0, alarm_mode = 0;
struct Alarm alarm[3] = {0, 0, 0};
void alarmScreen(){
  lcd.setCursor(0,0);
  lcd.printf("Alarm 1");
  lcd.setCursor(0,1);
  lcd.printf("Alarm 2");
  lcd.setCursor(0,2);
  lcd.printf("Alarm 3");
  lcd.setCursor(0,3);
  lcd.printf("menu");
  delay(500);
  if(alarm_mode == 0) {
  lcd.setCursor(0,alarm_select);
  lcd.printf("       ")
  delay(500);
  if (btn2 && alarm_select == 3) {
    state = 8;
    btn2 = 0;
    lcd.clear();
    } else if(btn2) {
    btn2 = 0;
    alarm_mode = 1;
    } else if(btn1 && alarm_select != 0) {
    btn1 = 0;
    alarm_select--;
    } else if(btn3 && alarm_select != 3) {
    btn3 = 0;
    alarm_select++;
    }
  } else {
    if(set_mode == 0) {
    if(btn2 && set_select == 3) {
      btn2 = 0;
      alarm_mode = 0;
    } else if(set_select == 2 && btn2) {
      btn2 = 0;
      alarm[alarm_select].act = !alarm[alarm_select].act;
    } else if(btn2) {
      btn2 = 0;
      set_mode = 1;
    } else if(btn1 && set_select != 0) {
      btn1 = 0;
      set_select--;
    } else if(btn3 && set_select != 3) {
      btn3 = 0;
      set_select++;
    }
  } else {
    if (btn2) {
      btn2 = 0;
      set_mode = 0;
    } else if (btn3) {
      switch(set_select) {
        case 0:
          alarm[alarm_select].hour++;
          break;
        case 1:
          alarm[alarm_select].min++;
          break;
      }
      btn3 = 0;
    } else if (btn3) {
      switch(set_select) {
        case 0:
          alarm[alarm_select].hour--;
          break;
        case 1:
          alarm[alarm_select].min--;
          break;
      }
      btn1 = 0;
    }
}
}
void menuScreen(){
  lcd.setCursor(0,menu_select);
  lcd.printf("          ");
  delay(500);
  lcd.setCursor(0,0);
  lcd.printf("main");
  lcd.setCursor(0,1);
  lcd.printf("setTime");
  lcd.setCursor(0,2);
  lcd.printf("alarm");
  delay(500);
  if(btn2){
    lcd.clear();
    state = menu_select;
    btn2 = 0;
  }
  else if(btn1 && menu_select != 0){
    menu_select--;
    btn1 = 0;
  }
  else if (btn3 && menu_select < 8){
    menu_select++;
    btn3 = 0;
  }
}
int set_select = 0, set_mode = 0;
void setTimeScreen(){
  switch(set_select){
    case 0:
      lcd.setCursor(0,1);
      lcd.printf("  ");
      break;
    case 1:
      lcd.setCursor(3,1);
      lcd.printf("  ");
      break;
    case 2:
      lcd.setCursor(6,1);
      lcd.printf("  ");
      break;
    case 3:
      lcd.setCursor(0,2);
      lcd.printf("    ");
      break;
    case 4:
      lcd.setCursor(5,2);
      lcd.printf("  ");
      break;
    case 5:
      lcd.setCursor(8,2);
      lcd.printf("  ");
      break;
    case 6:
      lcd.setCursor(0,3);
      lcd.printf("    ");
      break;
  }
  delay(500);
  lcd.setCursor(0,1);
  lcd.printf("%2d:%2d:%2d",nowTime.hour,nowTime.min,nowTime.sec);
  lcd.setCursor(0,2);
  lcd.printf("%4d/%2d/%2d",nowDate.year,nowDate.month,nowDate.day);
  lcd.setCursor(0,3);
  lcd.printf("menu");
  delay(500);
  if(set_mode == 0) {
    if(btn2 && set_select == 6) {
      btn2 = 0;
      state = 8;
      lcd.clear();
    } else if(btn2) {
      btn2 = 0;
      set_mode = 1;
    } else if(btn1 && set_select != 0) {
      btn1 = 0;
      set_select--;
    } else if(btn3 && set_select != 6) {
      btn3 = 0;
      set_select++;
    }
  } else {
    if (btn2) {
      btn2 = 0;
      set_mode = 0;
    } else if (btn3) {
      switch(set_select) {
        case 0:
          nowTime.hour++;
          break;
        case 1:
          nowTime.min++;
          break;
        case 2:
          nowTime.sec++;
          break;
        case 3:
          nowDate.year++;
          break;
        case 4:
          nowDate.month++;
          break;
        case 5:
          nowDate.day++;
      }
      btn3 = 0;
    } else if (btn3) {
      switch(set_select) {
        case 0:
          nowTime.hour--;
          break;
        case 1:
          nowTime.min--;
          break;
        case 2:
          nowTime.sec--;
          break;
        case 3:
          nowDate.year--;
          break;
        case 4:
          nowDate.month--;
          break;
        case 5:
          nowDate.day--;
      }
      btn1 = 0;
    }
  }
}
void printYoil(){
  int gap = 0;
  int year = nowDate.year, month = nowDate.month, day = nowDate.day;
  while(month!=12){
    gap -= getMaxday(year, month);
    month++;
  }
  while(year!=2023)
    if(year>2023){
      if(isYun(year))
        gap += 366;
      else
        gap += 365;
      year--;
    } else {
      if(isYun(year+1))
        gap -= 366;
      else
        gap -= 365;
      year++;
    }
  gap += (day-3);
  gap = gap%7;
  if(gap<0)
    gap += 7;
  switch(gap){
    case 0:
      lcd.printf("SUN");
      break;
    case 1:
      lcd.printf("MON");
      break;
    case 2:
      lcd.printf("TUE");
      break;
    case 3:
      lcd.printf("WED");
      break;
    case 4:
      lcd.printf("THR");
      break;
    case 5:
      lcd.printf("FRI");
      break;
    case 6:
      lcd.printf("SAT");
      break;
  }
}
void btnIn() {
  if(digitalRead(Button1)==LOW){
    while(1)
      if(digitalRead(Button1)==HIGH)
        break;
    btn1 = 1;
  }
  if(digitalRead(Button2)==LOW){
    while(1)
      if(digitalRead(Button2)==HIGH)
        break;
    btn2 = 1;
  }
  if(digitalRead(Button3)==LOW){
    while(1)
      if(digitalRead(Button3)==HIGH)
        break;
    btn3 = 1;
  }
}
void nextDay(){
  nowDate.day++;
  int max_day = getMaxday(nowDate.year, nowDate.month);
  if (max_day<nowDate.day){
    nowDate.month++;
    nowDate.day = 1;
  }
  if (nowDate.month == 13){
    nowDate.year++;
    nowDate.month = 1;
  }
}
int getMaxday(int year, int month) {
  switch(month){
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
      return 31;
    case 2:
      if(isYun(year))
        return 29;
      else
        return 28;
    default:
      return 30;
  }
}
int isYun(int year){
  if((year % 400) == 0)
    return 1;
  else if((year % 100) == 0)
    return 0;
  else if((year % 4) == 0)
    return 1;
  else
    return 0;
}
void timerIsr(){
  nowTime.sec++;
  if(nowTime.sec == 60) {
    nowTime.min++;
    nowTime.sec = 0;
  }
  if(nowTime.sec == 60) {
    nowTime.hour++;
    nowTime.min = 0;
  }
  if(nowTime.hour == 24) {
    nextDay();
    nowTime.hour = 0;
  }
}