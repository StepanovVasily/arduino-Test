#include <DHT.h>
#include <LiquidCrystal.h>
#define DHTPIN 53
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8,9,4,5,6,7);
//  int key=0;
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1307);



void setup()
{
delay(300);
//Serial.begin(9600); 
  //Serial.println("DHTxx test!");
  time.begin();
  pinMode (23,OUTPUT);
  pinMode (25,OUTPUT);
  pinMode (27,OUTPUT);
  pinMode(A2,INPUT);//датчик углекислоты 
  //pinMode(A0, INPUT);
  //pinMode(A1, INPUT);
  //pinMode(A9, INPUT);
  lcd.begin(16,2); 
  //time.settime(30,34,17,2,10,16,7);
  //Timer1.initialize(100000);
  //Timer1.attachInterrupt(keyPressed);
}
void loop()
{
////////////вывод времени


lcd.setCursor(6, 1);
lcd.print(time.gettime("H:i")); // выводим время

//lcd.print(time.gettime("H:i:s")); // выводим время
//lcd.print(time.gettime("d.m.Y")); // выводим время

  
  float h = dht.readHumidity(); 
  float t = dht.readTemperature();

  
  
     //if(key!=analogRead(A0))
  ///lcd.setCursor(8, 1);
 //  lcd.print("    ");
//lcd.setCursor(8, 1);
 //  lcd.print(analogRead(A0)); 
 
  
    
lcd.setCursor(0, 0);
    lcd.print("B:");
    lcd.print(h,1);
    lcd.print("%");
   lcd.setCursor(6, 0);
     lcd.print(" T:");
    lcd.print(t,1); 
    
    lcd.print((char)223);
    //lcd.setCursor(15, 0);
//lcd.print((char)223);

    //int t1,t2,t3;
     // analogRead(A9);
    //  delay(30);
//t1=analogRead(A9)/2; 
//      delay(30);
//t2=analogRead(A9)/2;
 //     delay(30);
//t3=analogRead(A9)/2; 

  //lcd.setCursor(14, 0);
 //lcd.print((t1+t2+t3)/3); 
 lcd.setCursor(0, 1);

 int light=analogRead(A1);
 // if (isnan(t) || isnan(h)) { 
    
  light=(1023-light);

   lcd.print(light); 
 //readLight();



int HOUR=time.Hours;
int MINUTE=time.minutes;


///реле 1
if((HOUR>0)&&(HOUR<13)&&(light<940)){digitalWrite(23,1);lcd.setCursor(15,1);lcd.print('I');}
if((HOUR>0)&&(HOUR<13)&&(light>950)){digitalWrite(23,0);lcd.setCursor(15,1);lcd.print(' ');}
if((HOUR<0)||(HOUR>13)){digitalWrite(23,0);lcd.setCursor(15,1);lcd.print(' ');}

//// реле 2
if(t<15){digitalWrite(25,1);lcd.setCursor(14,1);lcd.print('O');}
if(t>18){digitalWrite(25,0);lcd.setCursor(14,1);lcd.print(' ');}


////реле 3

if((MINUTE>0)&&(MINUTE<30)){digitalWrite(27,1);lcd.setCursor(13,1);lcd.print('I');}
else {digitalWrite(27,0);lcd.setCursor(13,1);lcd.print(' ');}



//датчик углекислоты
int f=0;
for(int i=0;i!=50;i++){
  delay(10);
f=f+analogRead(A2);}
f=f/50;

int f1=0;
for(int i=0;i!=50;i++){
  delay(10);
f1=f1+analogRead(A2);}
f1=f1/50;


f=(f+f1)/2;
f=341-f;
float ff=f*28.3+350;



//


 

  delay(9000);
  lcd.clear();
   
    
    
}

void keyPressed()
{
   
  
}

void readLight()
{
    
int light=analogRead(A1);
 // if (isnan(t) || isnan(h)) { 
    
 lcd.print("L ");
  light=(1023-light);

   lcd.print(light); 

}
