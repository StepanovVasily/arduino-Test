#include <DHT.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#define DHTPIN 53
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1307);

bool posLed=false;//позиция экрана
int keyValue=100;
int keyValueAfter=100;
bool runRender=true;
  int tOn=15;
  int tOff=18;
unsigned long timeWater=millis();
  int waterState=0;
  bool waterPost=false;
  int waterTMax=22;
  int waterTMin=20.5;
void render(float h,float t, int light)
{
  lcd.clear();
    if(!posLed){
  
  
  lcd.setCursor(6, 1);
  lcd.print(time.gettime("H:i")); // выводим время

  if (!isnan(h)) {
    lcd.setCursor(0, 0);
    lcd.print("B:");
    lcd.print(h, 1);
    lcd.print("%");
    lcd.setCursor(6, 0);
    lcd.print(" T:");
    lcd.print(t, 1);

    lcd.print((char)223);
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("DHT no connect");
  }


  lcd.setCursor(0, 1);


  lcd.print(light);
    }
else
{
  lcd.setCursor(0, 0);
  lcd.print("BK");
  lcd.write(byte(0));
   lcd.write(byte(1));
   lcd.write(byte(2));
    lcd.print("EH");
    lcd.write(byte(3));
    lcd.print("E HACOCA");
    lcd.setCursor(0, 1);
    lcd.print("C ");
    lcd.print(tOn);
    lcd.print((char)223);
    lcd.print(" - ");
    lcd.print(tOff);
    lcd.print((char)223);
  }
    
  }
void setup()
{

  delay(300);
  time.begin();
  pinMode (23, OUTPUT);
  pinMode (25, OUTPUT);
  pinMode (27, OUTPUT);
  pinMode (29, OUTPUT);
  pinMode(A2, INPUT); //датчик углекислоты
    
    byte charL[8] = {    B01111,    B01000,    B01001,    B01001,    B01001,    B01001,    B11001,    B00000  };
    byte charIO[8] = {    B10010,    B10101,    B10101,    B11101,    B10101,    B10101,    B10010,    B00000  };
      byte char4[8] = {    B10001,    B10001,    B10001,    B10001,    B01111,    B00001,    B00001,    B00000  };
       byte charN[8] = {    B10001,    B10001,    B10011,    B10101,    B10101,    B11001,    B10001,    B00000  };
tOn=EEPROM.read(0);
if(tOn<5||tOn>40)
  {
    EEPROM.write(0, 15);
    tOn=15;
  }
tOff=tOn+3;
  lcd.createChar(0, charL);
   lcd.createChar(1, charIO);
   lcd.createChar(2, char4);
   lcd.createChar(3, charN);

 Serial.begin(9600);
  lcd.begin(16, 2);

}

void loop()
{

      
  //чтение датчиков
runRender=1;

  float h = dht.readHumidity();
  float t = dht.readTemperature();
 int HOUR = time.Hours;
  int MINUTE = time.minutes;
  int light = analogRead(A1);
  light = (1023 - light);
  for(int i=0;i!=90;i++){
  delay(100);//ждать 9 секунд

  
if(runRender){

render(h,t,light);
  runRender=0;
}
    myThreadCallback();



switch(waterState)
{
  case 0://если время больше 30 сек и температура выше, то обнулить время и перейти в режим полива
  if(t>waterTMax)waterPost=true;//работает если повысилось до 30
  if(t<waterTMin)waterPost=false;// выключается если уменьшилось до 26
 if(waterPost&&(t>waterTMin))//если больше 30 то включение
  {
  if((millis()-timeWater)>30000)
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


  }
 


  ///реле 1
  if ((HOUR > 16) && (HOUR < 22) && (light < 940)) {
    digitalWrite(23, 1);
  }
  if ((HOUR > 16) && (HOUR < 22) && (light > 950)) {
    digitalWrite(23, 0);
  }
  if ((HOUR < 17) || (HOUR > 21)) {
    digitalWrite(23, 0);
  }

  
///////реле 2
  if (!isnan(h)) {
    if (t < tOn) {
      digitalWrite(25, 1);
      lcd.setCursor(14, 1);
    }
    if (t > tOff) {
      digitalWrite(25, 0);
      lcd.setCursor(14, 1);
    }
  }
  else
  {
    digitalWrite(25, 0);
  }

  ////реле 3
/*//реле 3 выкл
  if ((MINUTE > 0) && (MINUTE < 30)) {
    digitalWrite(27, 1);
    lcd.setCursor(13, 1);
  }
  else {
    digitalWrite(27, 0);
    lcd.setCursor(13, 1);
  }

*/


  lcd.clear();

  
  

}


void myThreadCallback(){
  keyValue=analogRead(A0);
  if(keyValue>50&&keyValue<120) return;
  if(keyValue<50){posLed=!posLed;runRender=!runRender;return;};
  if(keyValue>120&&keyValue<230&&posLed&&(tOn<40)){tOn++;tOff++;runRender=!runRender;return;};
  if(keyValue>230&&keyValue<410&&posLed&&(tOn>5)){tOn--;tOff--;runRender=!runRender;return;};
}


