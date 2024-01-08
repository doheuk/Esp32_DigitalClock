#include <LiquidCrystal.h>
#include <math.h>
#include <Ticker.h>
#include "pitches.h"

#define Freq 5000
#define Ch 6
#define Resolution 13
#define Button1 36 
#define Button2 39
#define Button3 34
#define TEMPER 35
#define LED 12
#define CDC 37 //3800 
#define BUZZER 20
#define MOTOR 22

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
void timerScreen();
void worldScreen();
void backDay();
void timerStart();
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
};
struct timerf {
  int hour;
  int min;
  int sec; 
};
int melody[] = {
  REST, NOTE_D4,
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5,
  NOTE_C5, 
  NOTE_A4,
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_F4, NOTE_GS4,
  NOTE_D4, 
  NOTE_D4,
  
  NOTE_G4, NOTE_AS4, NOTE_A4,
  NOTE_G4, NOTE_D5,
  NOTE_F5, NOTE_E5,
  NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_B4,
  NOTE_G4,
  NOTE_AS4,
   
  NOTE_D5, NOTE_AS4,
  NOTE_D5, NOTE_AS4,
  NOTE_DS5, NOTE_D5,
  NOTE_CS5, NOTE_A4,
  NOTE_AS4, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_D4,
  NOTE_D5, 
  REST, NOTE_AS4,  
  
  NOTE_D5, NOTE_AS4,
  NOTE_D5, NOTE_AS4,
  NOTE_F5, NOTE_E5,
  NOTE_DS5, NOTE_B4,
  NOTE_DS5, NOTE_D5, NOTE_CS5,
  NOTE_CS4, NOTE_AS4,
  NOTE_G4
};
int durations[] = {
  2, 4,
  4, 8, 4,
  2, 4,
  2, 
  2,
  4, 8, 4,
  2, 4,
  1, 
  4,
  
  4, 8, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1,
  4,
   
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1, 
  4, 4,  
  
  2, 4,
  2, 4,
  2, 4,
  2, 4,
  4, 8, 4,
  2, 4,
  1
};
Ticker timer;
Ticker timer2;

int set_select = 0, set_mode = 0; // setTime
int alarm_select = 0, alarm_mode = 0; //alarm
struct Alarm alarms[3]; // alarms
void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 3; i ++)
    alarms[i].hour = alarms[i].min = alarms[i].act = 0;
  timer.attach(1, timerIsr);
  pinMode(Button1,INPUT);
  pinMode(Button2,INPUT);
  pinMode(Button3,INPUT);
  pinMode(TEMPER,INPUT);
  pinMode(LED,OUTPUT);
  pinMode(CDC, INPUT);
  pinMode(BUZZER, OUTPUT);
  attachInterrupt(Button1, btnIn, FALLING);
  attachInterrupt(Button2, btnIn, FALLING);
  attachInterrupt(Button3, btnIn, FALLING);
  lcd.begin(20,4);
  ledcSetup(Ch, Freq, Resolution);
  ledcAttachPin(MOTOR, Ch);
  Serial.begin(9600);
}
struct timerf timerf1 = {0, 0, 0};
struct Time nowTime = {0,0,0};
struct Date nowDate = {2023,12,5,4};
int btn1 = 0, btn2 = 0, btn3 = 0;
int state = 0, menu_select = 0, world = 0, timer_start = 0;
int size = sizeof(durations) / sizeof(int);
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
      break;
    case 3:
      timerScreen();
      break;
    case 4:
      worldScreen();
  }
  int v = analogRead(CDC);
  if(v > 500)
    digitalWrite(LED,HIGH);
  else
    digitalWrite(LED,LOW);
  for(int i = 0; i < 3; i++)
    if(alarms[i].act && alarms[i].hour == nowTime.hour && alarms[i].min == nowTime.min && nowTime.sec == 0)
      for (int note = 0; note < size; note++) {
        int duration = 1000 / durations[note];
        tone(BUZZER, melody[note], duration);
        int pauseBetweenNotes = duration * 1.30;
        delay(pauseBetweenNotes);
        noTone(BUZZER);
        if(btn1 || btn2 || btn3) {
          btn1 = btn2 = btn3 = 0;
          break;
        }
      }
}

void mainScreen(){
  lcd.setCursor(6,0);
  lcd.printf("%04d/%02d/%02d ",nowDate.year,nowDate.month,nowDate.day);
  printYoil();
  lcd.setCursor(0,1);
  lcd.printf("%02d:%02d:%02d",nowTime.hour,nowTime.min,nowTime.sec);
  float temper = analogRead(TEMPER);
  float temper_c = map(temper,0,2047,2,150);
  lcd.setCursor(14,1);
  lcd.printf("%2d'C",(int)temper_c);
  if(btn2) {
    lcd.clear();
    state = 8;
    btn2 = 0;
  }
}
void alarmScreen(){
  lcd.setCursor(0,0);
  lcd.printf("Alarm 1");
  lcd.setCursor(0,1);
  lcd.printf("Alarm 2");
  lcd.setCursor(0,2);
  lcd.printf("Alarm 3");
  lcd.setCursor(0,3);
  lcd.printf("menu");
  lcd.setCursor(10,0);
  lcd.printf("%02d:%02d",alarms[alarm_select].hour,alarms[alarm_select].min);
  lcd.setCursor(10,1);
  if (alarms[alarm_select].act)
    lcd.printf("ON ");
   else
    lcd.printf("OFF");
  lcd.setCursor(10,2);
  lcd.printf("back");
  delay(500);
  if(alarm_mode == 0) {
  lcd.setCursor(0,alarm_select);
  lcd.printf("       ");
  delay(500);
  if (btn2 && alarm_select == 3) { //alarm mode == 0
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
  } else { // alarm_mode == 1
    switch(set_select) {
      case 0:
        lcd.setCursor(10,0);
        lcd.printf("  ");
        break;
      case 1:
        lcd.setCursor(13,0);
        lcd.printf("  ");
        break;
      case 2:
        lcd.setCursor(10,1);
        lcd.printf("   ");
        break;
      case 3:
        lcd.setCursor(10,2);
        lcd.printf("    ");
        break;
    }
    delay(500);
    if(set_mode == 0) { //set_mode == 0 
      if(btn2 && set_select == 3) {
        btn2 = 0;
        alarm_mode = 0;
        set_select =0;
      } else if(set_select == 2 && btn2) {
        btn2 = 0;
        alarms[alarm_select].act = alarms[alarm_select].act == 1 ? 0 : 1;
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
   } else { //set mode == 1
      if (btn2) {
        btn2 = 0;
        set_mode = 0;
      } else if (btn3) {
        switch(set_select) {
          case 0:
            alarms[alarm_select].hour++;
            break;
          case 1:
            alarms[alarm_select].min++;
            break;
        }
        btn3 = 0;
      } else if (btn3) {
        switch(set_select) {
          case 0:
            alarms[alarm_select].hour--;
            break;
          case 1:
            alarms[alarm_select].min--;
            break;
        }
       btn1 = 0;
      }
    }  
  }
}
void menuScreen(){
  if(menu_select < 4)
    lcd.setCursor(0,menu_select);
  else
    lcd.setCursor(10,menu_select-4);
  lcd.printf("          ");
  delay(500);
  lcd.setCursor(0,0);
  lcd.printf("main");
  lcd.setCursor(0,1);
  lcd.printf("set time");
  lcd.setCursor(0,2);
  lcd.printf("alarm");
  lcd.setCursor(0,3);
  lcd.printf("timer");
  lcd.setCursor(10,0);
  lcd.printf("world time");
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
void timerScreen(){
  lcd.setCursor(0,0);
  lcd.printf("%02d:%02d:%02d",timerf1.hour,timerf1.min,timerf1.sec);
  lcd.setCursor(0,1);
  lcd.printf("Start  Stop  menu");
  delay(500);
  switch(set_select){
    case 0:
      lcd.setCursor(0,0);
      lcd.printf("  ");
      break;
    case 1:
      lcd.setCursor(3,0);
      lcd.printf("  ");
      break;
    case 2:
      lcd.setCursor(6,0);
      lcd.printf("  ");
      break;
    case 3:
      lcd.setCursor(0,1);
      lcd.printf("     ");
      break;
    case 4: 
      lcd.setCursor(7,1);
      lcd.printf("     ");
      break;
    case 5:
      lcd.setCursor(13,1);
      lcd.printf("     ");
      break;
  }
  delay(500);
  if(timer_start && timerf1.hour == 0 && timerf1.min == 0 && timerf1.sec == 0) {
    timer2.detach();
    ledcWrite(Ch,8000);
    if(btn1||btn2||btn3){
      ledcWrite(Ch,0);
      btn1=btn2=btn3=0;
      timer_start = 0;
    }
  }
  if(set_mode == 0) {
    if(btn2 && set_select == 5) {
      btn2 = 0;
      set_select = 0;
      state = 8;
      lcd.clear();
    } else if(set_select == 3 && btn2) {
      btn2 = 0;
      timer2.attach(1,timerStart);
      timer_start = 1;
    } else if(set_select == 4 && btn2) {
      btn2 = 0;
      timer2.detach();
      timer_start = 0;
    } else if(btn2) {
      btn2 = 0;
      set_mode = 1;
    } else if(btn1 && set_select != 0){
      set_select--;
      btn1 = 0;
    } else if(btn3 && set_select != 5){
      set_select++;
      btn3 = 0;
    }
  } else if(set_mode == 1) {
    if(btn2) {
      btn2 = 0;
      set_mode = 0;
    } else if(btn1) {
      btn1 = 0;
      switch(set_select) {
        case 0:
          if(timerf1.hour != 0)
            timerf1.hour--;
          break;
        case 1:
          if(timerf1.min != 0)
            timerf1.min--;
          break;
        case 2:
          if(timerf1.sec != 0)
            timerf1.sec--;
      }
    } else if(btn3) {
      btn3 = 0;
      switch(set_select) {
        case 0:
          if(timerf1.hour != 99)
            timerf1.hour++;
          break;
        case 1:
          if(timerf1.min != 99)
            timerf1.min++;
          break;
        case 2:
          if(timerf1.sec != 99)
            timerf1.sec++;
          break;
      }
    }
  }
}
void timerStart(){
  timerf1.sec = timerf1.sec + timerf1.min*60 + timerf1.hour * 60 * 60;
  if(timerf1.sec != 0) {
    timerf1.sec--;
    timerf1.hour = timerf1.sec / 3600;
    timerf1.min = (timerf1.sec - timerf1.hour * 3600) / 60;
    timerf1.sec = timerf1.sec - 3600 * timerf1.hour - 60 * timerf1.min;
  }
}
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
void worldScreen(){
  lcd.setCursor(0,0);
  lcd.printf("Seoul");
  lcd.setCursor(0,1);
  lcd.printf("New York");
  lcd.setCursor(0,2);
  lcd.printf("Rome");
  lcd.setCursor(0,3);
  lcd.printf("Paris");
  lcd.setCursor(10,0);
  lcd.printf("London");
  lcd.setCursor(10,1);
  lcd.printf("Hanoi");
  lcd.setCursor(10,2);
  lcd.printf("Singapore");
  lcd.setCursor(10,3);
  lcd.printf("menu");
  delay(500);
  if(world < 4)
    lcd.setCursor(9,world);
  else
    lcd.setCursor(19,world-4);
  lcd.printf("*");
  if(set_select < 4 ) {
    lcd.setCursor(0, set_select);
    lcd.printf("         ");
  } else {
    lcd.setCursor(10, set_select - 4);
    lcd.printf("         ");
  }
  delay(500);
  if(btn2 && set_select == 7) {
    set_select = 0;
    lcd.clear();
    state = 8;
    btn2 = 0;
  } else if(btn2) {
    if(world < 4)
      lcd.setCursor(9,world);
    else
      lcd.setCursor(19,world-4);
    lcd.printf(" ");
    btn2 = 0;
    switch(world){
      case 1://new york
        nowTime.hour = nowTime.hour + 14;
        break;
      case 2://Rome
        nowTime.hour = nowTime.hour + 8;
        break;
      case 3://paris
        nowTime.hour = nowTime.hour + 8;
        break;
      case 4://london
        nowTime.hour = nowTime.hour + 9;
        break;
      case 5://hanoi
        nowTime.hour = nowTime.hour + 2;
        break;
      case 6://singapore
        nowTime.hour = nowTime.hour + 1;
        break;
    }
    switch(set_select) {
      case 1://new york
        nowTime.hour = nowTime.hour - 14;
        break;
      case 2://Rome
        nowTime.hour = nowTime.hour - 8;
        break;
      case 3://paris
        nowTime.hour = nowTime.hour - 8;
        break;
      case 4://london
        nowTime.hour = nowTime.hour- 9;
        break;
      case 5://hanoi
        nowTime.hour = nowTime.hour -2;
        break;
      case 6://singapore
        nowTime.hour = nowTime.hour -1;
        break;
    }
    world = set_select;
  } else if(btn1 && set_select != 0) {
    btn1 = 0;
    set_select--;
  } else if(btn3 && set_select != 7) {
    btn3 = 0;
    set_select++;
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
  if(nowTime.sec >= 60) {
    nowTime.min++;
    nowTime.sec = 0;
  }
  if(nowTime.min >= 60) {
    nowTime.hour++;
    nowTime.min = 0;
  }
  if(nowTime.hour >= 24) {
    nextDay();
    nowTime.hour = 0;
  }
  if(nowTime.hour < 0) {
    nowTime.hour = 24 + nowTime.hour;
    backDay(); 
  }
}
void backDay(){
  if(nowDate.month == 1 && nowDate.day == 1) {
    nowDate.day = 31;
    nowDate.month = 12;
    nowDate.year--;
  }
  nowDate.day--;
  if (nowDate.day==0){
    nowDate.month--;
    nowDate.day = getMaxday(nowDate.year, nowDate.month);
  }
}