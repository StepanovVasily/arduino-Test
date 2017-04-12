#include <DHT.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SD.h>
#include <Ethernet.h>
#define DHTPIN 49
#define DHTTYPE DHT22
DHT dht1(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 44, 5, 6, 7);
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1307);
byte mac[] = {  0x00, 0xBD, 0xCE, 0xFE, 0xEE, 0xFD};
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);

struct sensors{
  float t;
  float h;
  int light;
  int hour;
  int minute;
  int humGround;
  };
  
sensors sens;

 File dataFile;
int posLed=0;//позиция экрана
int keyValue=100;
int keyState=0;
bool runRender=true;
  int tOn=15;
  int tOff=18;
unsigned long timeWater=millis();
unsigned long timeWaterInterval=30;
unsigned long timeRender=millis();
unsigned long timeWriteSensorsFile=millis();
unsigned long timeReleLight=millis();
unsigned long timeReleOtopl=millis();
unsigned long timeSensorsUpdate=millis();
bool otoplStat=0;
  int waterState=0;
  bool waterPost=false;
  int waterTMax=25;
  int waterTMin=24;
  bool sdSetup=false;

void setup()
{
pinMode(10, OUTPUT);//отключить ethernet
    digitalWrite(10, HIGH);
  delay(300);
  time.begin();
  pinMode (23, OUTPUT);
  pinMode (25, OUTPUT);
  pinMode (27, OUTPUT);
  pinMode (29, OUTPUT);
  pinMode(A2, INPUT); //датчик углекислоты
   pinMode(10, OUTPUT);//отключить ethernet
    digitalWrite(10, HIGH);
    byte charL[8] = {    B01111,    B01001,    B01001,    B01001,    B01001,    B01001,    B11001,    B00000  };
    byte charIO[8] = {    B10010,    B10101,    B10101,    B11101,    B10101,    B10101,    B10010,    B00000  };
      byte char4[8] = {    B10001,    B10001,    B10001,    B10001,    B01111,    B00001,    B00001,    B00000  };
       byte charN[8] = {    B10001,    B10001,    B10011,    B10101,    B10101,    B11001,    B10001,    B00000  };
       byte charD[8] = {    B01110,    B01010,    B01010,    B01010,    B01010,    B01010,    B11111,    B10001  };
     
       byte charP[8] = {    B11111,    B10001,    B10001,    B10001,    B10001,    B10001,    B10001,    B00000  };
       byte charIoI[8] = {    B10001,    B10001,    B11101,    B10101,    B10011,    B10101,    B11101,    B00000  };
tOn=EEPROM.read(0);
if(tOn<5||tOn>40)
  {
    EEPROM.write(0, 15);
    tOn=15;
  }
tOff=tOn+3;

timeWaterInterval=EEPROM.read(1);

waterTMin=EEPROM.read(2);
waterTMax=waterTMin+1;
  lcd.createChar(0, charL);
   lcd.createChar(1, charIO);
   lcd.createChar(2, char4);
   lcd.createChar(3, charN);
   lcd.createChar(4, charD);
    lcd.createChar(6, charP);
 lcd.createChar(7, charIoI);
 Serial.begin(9600);
  lcd.begin(16, 2);
  if (SD.begin(4)) {
  sdSetup=true;
 
  }

 Ethernet.begin(mac, ip);
  server.begin();

  sens.t = dht1.readTemperature();
  sens.h = dht1.readHumidity();
  sens.humGround=-1;
   sens.hour = time.Hours;
  sens.minute = time.minutes;
  sens.light=1023-analogRead(A1);
}

#include "func.h"


void loop()
{


 if((millis()-timeSensorsUpdate)>10000)//каждые 10 сек
  {
    timeSensorsUpdate=millis();
sens.t = dht1.readTemperature();
  sens.h = dht1.readHumidity();
  sens.humGround=-1;
   sens.hour = time.Hours;
  sens.minute = time.minutes;
  sens.light=1023-analogRead(A1);
  }
  
ethernetData(sens);
  
 timeWriteSensorsFile=interval(timeWriteSensorsFile,600000,writeSensorsFile,sens);//запись файла через каждые 10 минут

timeRender=interval(timeRender,2000,render,sens);

timeReleLight=interval(timeReleLight,10000,releLight,sens);

timeReleOtopl=interval(timeReleOtopl,10000,releOtopl,sens);


switch(waterState)
{
  case 0://если время больше 30 сек и температура выше, то обнулить время и перейти в режим полива
  if(sens.t>waterTMax)waterPost=true;//работает если повысилось до 30
  if(sens.t<waterTMin)waterPost=false;// выключается если уменьшилось до 26
 if(waterPost&&(sens.t>waterTMin))//если больше 30 то включение
  {
  if((millis()-timeWater)>(timeWaterInterval*1000))
  {
    if(waterPost==true){//включаться только если больше 30
      digitalWrite(27, 1);
    digitalWrite(29, 0);
    timeWater=millis();
    waterState=1;}    
    }
  }
  break;
  case 1://если время больше 5 сек, то  перейти в режим слива
  
    if((millis()-timeWater)>5000){
     digitalWrite(27, 0);
    digitalWrite(29, 1);
    waterState=2;}
  break;
  case 2://если время прошло больше 10 сек, то перейти в режим ожидания
   
    if(millis()-timeWater>6000){
        digitalWrite(27, 0);
  digitalWrite(29, 0);
 
    waterState=0;
    }
    break;
 
  }

    

    getButton();



  

}
