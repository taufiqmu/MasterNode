#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "oledDisplay.h"
#include <WiFi.h>
#include <Arduino.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>


String city                 = "Surabaya";
String countryCode          = "ID";
String openWeatherMapApiKey = "accf82339c12902aaba436433d79c886";

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

String wind, jsonBuffer;
int temper = 0;

unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void OledData(void){
    if ((millis() - lastTime) > timerDelay) {
      if(WiFi.status()== WL_CONNECTED){
        String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
        
        jsonBuffer = httpGETRequest(serverPath.c_str());
        Serial.println(jsonBuffer);
        JSONVar myObject = JSON.parse(jsonBuffer);
        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        String jsonString = JSON.stringify(myObject["main"]["temp"]);
        temper=jsonString.toInt();
        temper=(temper-273);
        jsonString = JSON.stringify(myObject["main"]["pressure"]);
        pres=jsonString.toInt();
        jsonString = JSON.stringify(myObject["main"]["humidity"]);
        hum=jsonString.toInt();
        jsonString = JSON.stringify(myObject["wind"]["speed"]);
        wind=jsonString;
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
      }else{
        Serial.println("WiFi Disconnected");
      }
      tft.flush();
      lastTime = millis();
    }
}

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
        tft.setCursor(10,10);
        tft.setTextSize(2);
        tft.print(city);
        tft.setTextSize(1);
        tft.setCursor(10,30);
        tft.print(countryCode);
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
        tft.setTextColor(ST7735_GREEN);
        tft.setCursor(140,10);
        tft.print(" C");
        tft.setTextColor(ST7735_YELLOW);
        tft.setCursor(48,70);
        tft.print("%");
        tft.setCursor(123,70);
        tft.print("%");
        tft.setCursor(48,110);
        tft.print("%");
        tft.setCursor(123, 108);
        tft.print((char)248);
        tft.setCursor(128, 110);
        tft.print("C");
}

void OledRun(void){ 
        tft.fillRect(105, 8 , 38, 30, ST7735_BLACK);
        tft.fillRect(10, 70 , 32, 10, ST7735_BLACK);
        tft.fillRect(90, 70 , 32, 10, ST7735_BLACK);
        tft.fillRect(10, 110, 32, 10, ST7735_BLACK);
        tft.fillRect(90, 110, 32, 10, ST7735_BLACK);

        tft.setTextColor(ST7735_GREEN);
        tft.setTextSize(3);
        tft.setCursor(108,10);
        tft.print(temper);
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