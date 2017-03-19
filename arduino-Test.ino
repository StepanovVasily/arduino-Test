#include <DHT.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>
#include <SD.h>
#include <Ethernet.h>
#define DHTPIN 49
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 44, 5, 6, 7);
#include <iarduino_RTC.h>
iarduino_RTC time(RTC_DS1307);
byte mac[] = {  0x00, 0xBD, 0xCE, 0xFE, 0xEE, 0xFD};
IPAddress ip(192, 168, 1, 177);
EthernetServer server(80);
 String res="";


File myFile;
File dataFile;
bool posLed = false; //позиция экрана
int keyValue = 100;
int keyValueAfter = 100;
bool runRender = true;
int tOn = 15;
int tOff = 18;
unsigned long timeWater = millis();
int waterState = 0;
bool waterPost = false;
int waterTMax = 25;
int waterTMin = 24;
bool sdSetup = false;

void setup()
{
Serial.begin(9600);
 
  time.begin();
  //time.settime(-1,-1,-1,-1,-1,17);

  pinMode (23, OUTPUT);
  pinMode (25, OUTPUT);
  pinMode (27, OUTPUT);
  pinMode (29, OUTPUT);
pinMode(10, OUTPUT);//отключить ethernet
    digitalWrite(10, HIGH);
    
  pinMode(A2, INPUT); //датчик углекислоты

  byte charL[8] = {    B01111,    B01000,    B01001,    B01001,    B01001,    B01001,    B11001,    B00000  };
  byte charIO[8] = {    B10010,    B10101,    B10101,    B11101,    B10101,    B10101,    B10010,    B00000  };
  byte char4[8] = {    B10001,    B10001,    B10001,    B10001,    B01111,    B00001,    B00001,    B00000  };
  byte charN[8] = {    B10001,    B10001,    B10011,    B10101,    B10101,    B11001,    B10001,    B00000  };
  tOn = EEPROM.read(0);
  if (tOn < 5 || tOn > 40)
  {
    EEPROM.write(0, 15);
    tOn = 15;
  }
  tOff = tOn + 3;
  lcd.createChar(0, charL);
  lcd.createChar(1, charIO);
  lcd.createChar(2, char4);
  lcd.createChar(3, charN);

   if (SD.begin(4)) {    sdSetup = true;  } else{ Serial.println("SD Error");}
  // digitalWrite(4, 0);
  // digitalWrite(10, 1);
  lcd.begin(16, 2);
 
  Ethernet.begin(mac, ip);
  server.begin();
//SD.remove("20032017.TXT");
}

void loop()
{
  
float  t=dht.readTemperature();
    float h=  dht.readHumidity();
    int humGround = analogRead(A2)/10.24;

    int light = analogRead(A1);
    light = (1023 - light);
  
    if (sdSetup) //записывать только если карта инициализированна
    {
      char bst[6], bsh[6],bbt[6],bbh[6], bslight[5], bshumGround[4], bstOn[4], bswaterTMax[4];
      
      dtostrf(t, 5, 1, bbt);
      sprintf(bst, "%05s", bbt);
      dtostrf(h, 5, 1, bbh);
      sprintf(bsh, "%05s", bbh);
      sprintf(bslight, "%04i", light);
      sprintf(bshumGround, "%03i", humGround);
      sprintf(bstOn, "%03i", tOn);
      sprintf(bswaterTMax, "%03i", waterTMax);
      char fileName[11];
      sprintf(fileName,"%s.txt",time.gettime("dmY"));
      dataFile = SD.open(fileName, FILE_WRITE);
      if (dataFile) {
      dataFile.print(time.gettime("H:i:s"));
      
      dataFile.print(bst);//температура
      dataFile.print(bsh);// влажность
      dataFile.print(bslight);// свет
      dataFile.print(bshumGround);//влажПочв
      dataFile.print(bstOn);//включение нас
      dataFile.print(bswaterTMax);//включ разбрызг
      dataFile.flush();
      dataFile.close();
      res="";
      dataFile = SD.open(fileName);
      
      while(dataFile.available())
      {
      res+="<tr><td>";
      res+=(char)dataFile.read();
res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
            res+="</td><td>";
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
       res+="</td><td>";
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
       res+="</td><td>";
             res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
       res+="</td><td>";
             res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
       res+="</td><td>";
             res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
       res+="</td><td>";
             res+=(char)dataFile.read();
      res+=(char)dataFile.read();
      res+=(char)dataFile.read();
       res+="</td></tr>";
      }
      } else {
    // if the file didn't open, print an error:
    Serial.println("error opening .txt");
  }

    }
    



  

  for (int fi = 0; fi != 70; fi++) { //запись файла через каждые 27 секунд

    //чтение датчиков
    runRender = 1;
    float t = dht.readTemperature();
    float h = dht.readHumidity();
    int humGround = analogRead(A2)/10.24;

    int HOUR = time.Hours;
    int MINUTE = time.minutes;
    int light = analogRead(A1);
    light = (1023 - light);
    
    
       
    for (int i = 0; i != 90; i++) {
      delay(100);//ждать 9 секунд
    ethernetData();

      if (runRender) {

        render(h, t, light);
        runRender = 0;
      }
      myThreadCallback();



      switch (waterState)
      {
        case 0://если время больше 30 сек и температура выше, то обнулить время и перейти в режим полива
          if (t > waterTMax)waterPost = true; //работает если повысилось до 30
          if (t < waterTMin)waterPost = false; // выключается если уменьшилось до 26
          if (waterPost && (t > waterTMin)) //если больше 30 то включение
          {
            if ((millis() - timeWater) > 30000)
            {
              if (waterPost == true) { //включаться только если больше 30
                digitalWrite(27, 1);
                digitalWrite(29, 0);
                timeWater = millis();
                waterState = 1;
              }
            }
          }
          break;
        case 1://если время больше 5 сек, то  перейти в режим слива

          if ((millis() - timeWater) > 5000) {
            digitalWrite(27, 0);
            digitalWrite(29, 1);
            waterState = 2;
          }
          break;
        case 2://если время прошло больше 10 сек, то перейти в режим ожидания

          if (millis() - timeWater > 6000) {
            digitalWrite(27, 0);
            digitalWrite(29, 0);

            waterState = 0;
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




    lcd.clear();
  


  
  }



  


}

int ethernetData()
{
  int ii=0;

  EthernetClient client = server.available();

if(client){
  char rtr;
  digitalWrite(10,0);   
    for(int i=0;i!=6;i++)
    rtr=client.read();
    while(client.read()!='\n') ii++;
    delay(100);
    if(rtr==' '){
 
client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html; charset=utf-8");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><body style='background-color: #000000;color: #ffffff;'><table>");
 delay(1);

    
client.write(res.c_str(),res.length());
client.print("</table></body></html>");
    }
delay(1);
client.flush();
    client.stop();
      Ethernet.maintain();
        }
  
  
  return 0;
}
      
void myThreadCallback() {
  keyValue = analogRead(A0);
  if (keyValue > 50 && keyValue < 120) return;
  if (keyValue < 50) {
    posLed = !posLed;
    runRender = !runRender;
    return;
  };
  if (keyValue > 120 && keyValue < 230 && posLed && (tOn < 40)) {
    tOn++;
    tOff++;
    runRender = !runRender;
    return;
  };
  if (keyValue > 230 && keyValue < 410 && posLed && (tOn > 5)) {
    tOn--;
    tOff--;
    runRender = !runRender;
    return;
  };
}

void render(float h, float t, int light)
{
  lcd.clear();
  if (!posLed) {


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




