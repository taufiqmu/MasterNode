#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include "oledDisplay.h"
#include "FlowSensor.h"

void printLocalTime();

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 6*3600;
const int   daylightOffset_sec = 3600;

int timeHour, timeMinute, timeSec = 0;
int waktu[6];
unsigned long prevMillisTime = 0;
unsigned long intervalTime = 1000;
uint16_t wateringDefault = 250;
uint16_t totalPlants;

void Schedule_Setup(void){
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    printLocalTime();
}

void Schedule_Loop(void){
    unsigned long currMillisTime = millis();
    while(currMillisTime - prevMillisTime >= intervalTime){
      prevMillisTime = currMillisTime;
      printLocalTime();
      
      if((timeHour == waktu[0] && timeMinute == waktu[1] && (timeSec >= 00 && timeSec <= 03)) || (timeHour == waktu[2] && timeMinute == waktu[3] && (timeSec >= 00 && timeSec <= 03)) || (timeHour == waktu[4] && timeMinute == waktu[5] && (timeSec >= 00 && timeSec <= 03))){
        totalMilliLitres = 0;
        relayState = 1;
        while(totalMilliLitres <= wateringDefault * totalPlants){
            if(relayState == 0){
              break;
            }
            digitalWrite(COILPIN, relayState);
            Flow_Read();
            delay(1000);
        }
        digitalWrite(COILPIN, 0);
      }
    }
}


void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%H:%M:%S");
  timeHour = timeinfo.tm_hour;
  timeMinute = timeinfo.tm_min;
  timeSec = timeinfo.tm_sec;

  Serial.println(timeHour);
  Serial.println(timeMinute);
  delay(1000);
}