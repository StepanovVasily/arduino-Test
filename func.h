int ethernetData(sensors sens)
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
    if(rtr==' '){//если пустой запрос
 
client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html; charset=utf-8");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println();
          client.println("<!DOCTYPE HTML><html><body style='background-color: #000000;color: #ffffff;'>");
 delay(1);

    
client.print("Сейчас: ");client.print(sens.hour);client.print(":");client.print(sens.hour);client.print("<br>");
client.print("Температура: ");client.print(sens.t);client.print("°");client.print("<br>");
client.print("Влажность: ");client.print(sens.h);client.print("%");client.print("<br>");
client.print("Свет: ");client.print(sens.light);client.print("<br>");
client.print("Интервал включения опрыскивателя: ");client.print(timeWaterInterval);client.print(" секунд");client.print("<br>");
client.print("Насос отопления ");if(otoplStat==0)client.print("пока не включен"); else client.print("включен");client.print("<br>");
client.print("</body></html>");
    }
delay(1);
client.flush();
    client.stop();
      Ethernet.maintain();
        }
  
  
  return 0;
}


unsigned long interval(unsigned long time,unsigned long interval,void (*f)(sensors),sensors in)
{
  if((millis()-time)>interval)
  {
    time=millis();
  f(in);
  }
  return time;
  }

void releOtopl(sensors sens)//реле2
{
    if (!isnan(sens.h)) {
    if (sens.t < tOn) {
      digitalWrite(25, 1);
    
      otoplStat=1;
    }
    if (sens.t > tOff) {
      digitalWrite(25, 0);
   
      otoplStat=0;
    }
  }
  else
  {
    digitalWrite(25, 0);
  }
  
  
  }

void releLight(sensors sens)
{
  ///реле 1
  if ((sens.hour > 16) && (sens.hour < 22) && (sens.light < 940)) {
    digitalWrite(23, 1);
  }
  if ((sens.hour > 16) && (sens.hour < 22) && (sens.light > 950)) {
    digitalWrite(23, 0);
  }
  if ((sens.hour < 17) || (sens.hour > 21)) {
    digitalWrite(23, 0);
  }
  
}


void render(sensors sens)
{
  lcd.clear();
Serial.println(posLed);Serial.println(keyState);
Serial.println(analogRead(A0));
switch(posLed)
{
  case 0:
  
lcd.setCursor(6, 1);
  lcd.print(sens.hour); // выводим время
  lcd.print(":");
  lcd.print(sens.minute);
  if (!isnan(sens.h)) {
    lcd.setCursor(0, 0);
    lcd.print("B:");
    lcd.print(sens.h, 1);
    lcd.print("%");
    lcd.setCursor(6, 0);
    lcd.print(" T:");
    lcd.print(sens.t, 1);
    lcd.print((char)223);
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("DHT no connect");
  }
  lcd.setCursor(0, 1);
  lcd.print(sens.light);
  
  
  break;
    case 1:
    
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
    
break;
    case 2:
lcd.setCursor(0, 0);
    lcd.write(byte(3));
    lcd.print("HTEPBA");
    lcd.write(byte(0));
    lcd.print(" O");
    lcd.write(byte(6));
    lcd.print("P");
    lcd.write(byte(7));
    lcd.print("CK");
    lcd.write(byte(3));
      lcd.setCursor(2, 1);
      lcd.print(timeWaterInterval);
        lcd.setCursor(7, 1);
            lcd.print("CEKYH");
            lcd.write(byte(4));
     break;
      case 3:
lcd.setCursor(0, 0);
  lcd.print("BK");
  lcd.write(byte(0));
   lcd.write(byte(1));
   lcd.write(byte(2));
    lcd.print("EH");
    lcd.write(byte(3));
    lcd.print("E O");
    lcd.write(byte(6));
    lcd.print("P");
    lcd.write(byte(7));
    lcd.print("CK");
      lcd.write(byte(3));
   lcd.setCursor(0, 1);
    lcd.print("C ");
    lcd.print(waterTMin);
    lcd.print((char)223);
    lcd.print(" - ");
    lcd.print(waterTMax);
    lcd.print((char)223);
      break;
default:
posLed=0;
break;
}



 
    
  }



  
  void writeSensorsFile()
{
    sensors sensWriteFile;
  sensWriteFile.t=dht1.readTemperature();
     sensWriteFile.h=  dht1.readHumidity();
     sensWriteFile.humGround = analogRead(A2)/10.24;
sensWriteFile.light = analogRead(A1);
sensWriteFile.light = (1024 - sensWriteFile.light);

 if (sdSetup) //записывать только если карта инициализированна
     {
       char bst[6], bsh[6],bbt[6],bbh[6], bslight[5], bshumGround[4], bstOn[4], bswaterTMax[4];
       
       dtostrf(sensWriteFile.t, 5, 1, bbt);
       sprintf(bst, "%05s", bbt);
       dtostrf(sensWriteFile.h, 5, 1, bbh);
       sprintf(bsh, "%05s", bbh);
       sprintf(bslight, "%04i", sensWriteFile.light);
       sprintf(bshumGround, "%03i", sensWriteFile.humGround);
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
       }
       }
  
  }


  
void getButton(){
  keyValue=analogRead(A0);
  if(keyValue>50&&keyValue<120){keyState=0; return;}
  //запуск рендера
  
  if(keyValue<50&&(keyState!=1)){keyState=1;if(posLed==3)posLed=0;else posLed++;return;};
  if(keyValue>120&&keyValue<230&&(keyState!=2)){ keyState=2;
  switch(posLed)
{
  case 0:break;
  case 1:if(tOn<40){tOn++;tOff++;EEPROM.write(0, tOn);}return;break;
  case 2:if(timeWaterInterval<245){timeWaterInterval+=10;EEPROM.write(1,timeWaterInterval);}return;break;
  case 3:if(waterTMin<50){waterTMin++;waterTMax++;EEPROM.write(2,waterTMin);}return;break;
  default:posLed=0;break;
}
}
  if(keyValue>230&&keyValue<410&&(keyState!=3)){keyState=3;
  switch(posLed)
{
  case 0:break;//если в положении главного экрана то ничего не делать
  case 1:if(tOn>5){tOn--;tOff--;EEPROM.write(0, tOn);}return;break;
    case 2:if(timeWaterInterval>10){timeWaterInterval-=10;EEPROM.write(1,timeWaterInterval);}return;break;
     case 3:if(waterTMin>5){waterTMin--;waterTMax--;EEPROM.write(2,waterTMin);}return;break;
  default:posLed=0;break;
}
  }
 if(keyValue>410){keyState=4; return;}else{timeRender=timeRender-2000;}
}
