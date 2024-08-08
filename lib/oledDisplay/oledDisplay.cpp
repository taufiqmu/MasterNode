#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "oledDisplay.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>
#include "FlowSensor.h"
#include "ScheduleWatering.h"
#include "db.h"

void ButtonMenu(void);

String id                   = "WATES";
String countryCode          = "ID";
String openWeatherMapApiKey = "accf82339c12902aaba436433d79c886";

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

String wind, jsonBuffer;
int temper = 0;
bool pumpControl, autoControl = 0;
bool relayState = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;
unsigned long preMillButton, currMillButton = 0;
unsigned long intervalButton = 3000;

void OledSetup(void){
        tft.initR(INITR_BLACKTAB);
        tft.setRotation(3);
        tft.fillScreen(ST7735_BLACK);

        delay(500);

        tft.fillScreen(ST7735_BLACK);
        //layout
        tft.drawFastHLine(0, 0, tft.width(), ST7735_WHITE);
        tft.drawFastHLine(1, 127, tft.width(), ST7735_WHITE);
        tft.drawFastVLine(0, 0, tft.height(), ST7735_WHITE);
        tft.drawFastVLine(159, 0, tft.height(), ST7735_WHITE);
        tft.drawFastVLine(80, 45, tft.height(), ST7735_WHITE);
        tft.drawFastHLine(1, 45, tft.width(), ST7735_WHITE);
        tft.drawFastHLine(1, 85, tft.width(), ST7735_WHITE);
        tft.setTextColor(ST7735_WHITE);
        tft.setCursor(50,10);
        tft.setTextSize(2);
        tft.print(id);
        // tft.setTextSize(1);
        // tft.setCursor(10,30);
        // tft.print(autoControl);
        
        tft.setTextSize(1);
        tft.setTextColor(ST7735_RED);
        tft.setCursor(15,55);
        tft.print("Humidity");
        tft.setCursor(90,55);
        tft.print("Soil Moist");
        tft.setCursor(15,95);
        tft.print("Lumen");
        tft.setCursor(90, 95);
        tft.print("Temperature");
        // tft.setTextColor(ST7735_GREEN);
        // tft.setCursor(140,10);
        // tft.print(" C");
        tft.setTextColor(ST7735_YELLOW);
        tft.setCursor(48,70); 
        tft.print("%");
        tft.setCursor(123,70);
        tft.print("%");
        tft.setCursor(48,110);
        tft.print("lx");
        tft.setCursor(123, 108);
        tft.print((char)248);
        tft.setCursor(128, 110);
        tft.print("C");
}
void OledDefault(void){
        tft.fillScreen(ST7735_BLACK);
        //layout
        tft.drawFastHLine(0, 0, tft.width(), ST7735_WHITE);
        tft.drawFastHLine(1, 127, tft.width(), ST7735_WHITE);
        tft.drawFastVLine(0, 0, tft.height(), ST7735_WHITE);
        tft.drawFastVLine(159, 0, tft.height(), ST7735_WHITE);
        tft.drawFastVLine(80, 45, tft.height(), ST7735_WHITE);
        tft.drawFastHLine(1, 45, tft.width(), ST7735_WHITE);
        tft.drawFastHLine(1, 85, tft.width(), ST7735_WHITE);
        tft.setTextColor(ST7735_WHITE);
        tft.setCursor(50,10);
        tft.setTextSize(2);
        tft.print(id);
        // tft.setTextSize(1);
        // tft.setCursor(10,30);
        // tft.print(autoControl);
        
        tft.setTextSize(1);
        tft.setTextColor(ST7735_RED);
        tft.setCursor(15,55);
        tft.print("Humidity");
        tft.setCursor(90,55);
        tft.print("Soil Moist");
        tft.setCursor(15,95);
        tft.print("Lumen");
        tft.setCursor(90, 95);
        tft.print("Temperature");
        // tft.setTextColor(ST7735_GREEN);
        // tft.setCursor(140,10);
        // tft.print(" C");
        tft.setTextColor(ST7735_YELLOW);
        tft.setCursor(48,70); 
        tft.print("%");
        tft.setCursor(123,70);
        tft.print("%");
        tft.setCursor(48,110);
        tft.print("lx");
        tft.setCursor(123, 108);
        tft.print((char)248);
        tft.setCursor(128, 110);
        tft.print("C");
}

void OledData(void){
    if ((millis() - lastTime) > timerDelay) {
      if(WiFi.status()== WL_CONNECTED){
        String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + id + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
        
        jsonBuffer = httpGETRequest(serverPath.c_str());
        Serial.println(jsonBuffer);
        JSONVar myObject = JSON.parse(jsonBuffer);
        if (JSON.typeof(myObject) == "undefined") {
          #if __DEBUG__
          Serial.println("Parsing input failed!");
          #endif
          return;
        }
        
        // String jsonString = JSON.stringify(myObject["main"]["temp"]);
        // temper=jsonString.toInt();
        // temper=(temper-273);
        // jsonString = JSON.stringify(myObject["main"]["pressure"]);
        // pres=jsonString.toInt();
        // jsonString = JSON.stringify(myObject["main"]["humidity"]);
        // hum=jsonString.toInt();
        // jsonString = JSON.stringify(myObject["wind"]["speed"]);
        // wind=jsonString;
        
        #if __DEBUG__
        Serial.println();
        Serial.print("JSON object = ");
        Serial.println(myObject);
        Serial.print("Temperature: ");
        Serial.println(temper);
        Serial.print("Humidity: ");
        Serial.println(moist);
        Serial.print("Soil Moist: ");
        Serial.println(soil);
        Serial.print("Lumen: ");
        Serial.println(lumen);
        #endif
      }else{
        #if __DEBUG__
        Serial.println("WiFi Disconnected");
        #endif
      }
      
      tft.flush();
      lastTime = millis();
    }
}

void OledRun(void){

  tft.fillRect(5, 30 , 200, 10, ST7735_BLACK);
  tft.fillRect(10, 70 , 32, 10, ST7735_BLACK);
  tft.fillRect(90, 70 , 32, 10, ST7735_BLACK);
  tft.fillRect(10, 110, 32, 10, ST7735_BLACK);
  tft.fillRect(90, 110, 32, 10, ST7735_BLACK);

  #if __DEBUG__
  Serial.print("PUMPbutton: ");
  Serial.println(digitalRead(PUMPBUTTON));
  Serial.print("ENTERbutton: ");
  Serial.println(digitalRead(ENTERBUTTON));
  Serial.print("pumpcontrol: ");
  Serial.println(pumpControl);
  #endif
  
  tft.setTextColor(ST7735_YELLOW);
  tft.setTextSize(1);
  tft.setCursor(15,70);
  tft.print(moist);
  tft.setCursor(90,70);
  tft.print(soil);
  tft.setCursor(15,110);
  tft.print(lumen);
  tft.setCursor(90, 110);
  tft.print(temp);
  tft.flush();
  
  ButtonMenu();
}


String httpGETRequest(const char* serverName) {
  HTTPClient http;
     
  http.begin(serverName);
  
  int httpResponseCode = http.GET();
  
  String payload = "{}"; 
  
  if (httpResponseCode>0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  http.end();

  return payload;
}

void ButtonMenu(void){
  if(autoControl == 1){
    tft.setTextColor(ST7735_GREEN);
    tft.setTextSize(1);
    tft.setCursor(130,30);
    tft.print("Auto");
    //Schedule_Loop();
  }
  
  if(pumpControl == 1){
    tft.setTextColor(ST7735_GREEN);
    tft.setTextSize(1);
    tft.setCursor(5,30);
    tft.print("Manual Control");   
    
    if(!digitalRead(ENTERBUTTON)){
      delay(20);
      if(!digitalRead(ENTERBUTTON)){
        while(!digitalRead(ENTERBUTTON));
        relayState = !relayState;
        digitalWrite(COILPIN, relayState);
      }
    }
  } 

  if(!digitalRead(STATUSBUTTON)){
    delay(40);
    if(!digitalRead(STATUSBUTTON)){tft.fillScreen(ST7735_BLACK);}
    while(!digitalRead(STATUSBUTTON)){
      tft.setTextColor(ST7735_WHITE);
      tft.setTextSize(1);
      tft.setCursor(5, 10);
      tft.print("Node: ");
      tft.setTextColor(ST7735_YELLOW);
      tft.print(node);   
      tft.setTextColor(ST7735_WHITE);
      tft.setTextSize(1);
      tft.setCursor(5, 50);
      tft.print("Jadwal 1: ");
      tft.setTextColor(ST7735_YELLOW);
      tft.print(waktu[0]);   
      tft.print(":");
      tft.print(waktu[1]);   
      tft.setTextColor(ST7735_WHITE);
      tft.setTextSize(1);
      tft.setCursor(5, 60);
      tft.print("Jadwal 2: ");
      tft.setTextColor(ST7735_YELLOW);
      tft.print(waktu[2]);   
      tft.print(":");
      tft.print(waktu[3]);   
      tft.setTextColor(ST7735_WHITE);
      tft.setTextSize(1);
      tft.setCursor(5, 70);
      tft.print("Jadwal 3: ");
      tft.setTextColor(ST7735_YELLOW);
      tft.print(waktu[4]);   
      tft.print(":");
      tft.print(waktu[5]);   
    }
    OledDefault();
  } 
  
  if(!digitalRead(PUMPBUTTON)){
    preMillButton = millis();
    while(!digitalRead(PUMPBUTTON)){
      currMillButton = millis();
      if(currMillButton - preMillButton > intervalButton){
        if(pumpControl == 0) {pumpControl = 1;}  else {pumpControl = 0;}
        break;
      }
    }
  }
  
  if(!digitalRead(AUTOBUTTON)){
    preMillButton = millis();
    while(!digitalRead(AUTOBUTTON)){
      currMillButton = millis();
      if(currMillButton - preMillButton > intervalButton){
        if(autoControl == 0) {autoControl = 1;}  else {autoControl = 0;}
        break;
      }
    }
  }
}