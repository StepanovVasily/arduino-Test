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


lcd.setCursor(8, 1);
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
    
 lcd.print("L ");
  light=(1023-light);

   lcd.print(light); 
 //readLight();



int HOUR=time.Hours;
int MINUTE=time.minutes;


///реле 1
if((HOUR>1)&&(HOUR<16)&&(light<940))digitalWrite(23,1);
if((HOUR>1)&&(HOUR<16)&&(light>950))digitalWrite(23,0);
if((HOUR<1)||(HOUR>16))digitalWrite(23,0);

//// реле 2
if(t<20)digitalWrite(25,1);
if(t>26)digitalWrite(25,0);


////реле 3

if((MINUTE>0)&&(MINUTE<30))digitalWrite(27,1);else digitalWrite(27,0);




 
  delay(10000);
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




/*
  int pin =5;
  #include "ardumidi.h"

  int ledPin = 13;
  int note_on = 0;
  int st=0;
  void setup()
  {
  Serial.begin(256000);
  pinMode(ledPin, OUTPUT);
  pinMode(pin, INPUT);

  }

  void loop()
  {

  digitalWrite(5,1);

  if (!digitalRead(5))
  {
  // play a Cminor chord on channel 0, at maximum volume (127)
  if(st==0){
  st=1;
    midi_note_on(0, MIDI_C, 127);
    midi_note_on(0, MIDI_E + MIDI_FLAT + MIDI_OCTAVE, 127);
    midi_note_on(0, MIDI_G + MIDI_OCTAVE, 127);

  // The MIDI_* macros were created to make your life easier, but you
  // don't have to use them. You may enter numbers instead if you know
  // what you're doing.

    note_on = 1;
    digitalWrite(ledPin, HIGH);
  }
  }

  else
  {
    if(st==1){
    st=0;
  // stop the Cminor chord
    midi_note_off(0, MIDI_C, 127);
    midi_note_off(0, MIDI_E + MIDI_FLAT + MIDI_OCTAVE, 127);
    midi_note_off(0, MIDI_G + MIDI_OCTAVE, 127);

    note_on = 0;
    digitalWrite(ledPin, LOW);
    delay(100);
    }
  }


  }*/

// Available commands:
//
// Start/stop playing a certain note:
//   midi_note_on(byte channel, byte key, byte velocity);
//   midi_note_off(byte channel, byte key, byte velocity);
//
// Change pressure of specific keys:
//   midi_key_pressure(byte channel, byte key, byte value);
//
// Change controller value (used for knobs, etc):
//   midi_controller_change(byte channel, byte controller, byte value);
//
// Change "program" (change the instrument):
//   midi_program_change(byte channel, byte program);
//
// Change key pressure of entire channels:
//   midi_channel_pressure(byte channel, byte value);
//
// Change pitch-bend wheel:
//   midi_pitch_bend(byte channel, int value);
//
// Send a comment:
//   midi_comment(char* str);
//
// Send a series of bytes (to be interpreted by another program):
//   midi_printbytes(char* bytes, int len);
//
// Parameters:
//   channel             an integer from 0 to 15
//   pitch-bend value    an integer from 0 to 16383
//   all other values    an integer from 0 to 127
//


