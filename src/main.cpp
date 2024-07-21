#include <Arduino.h>
#include <ModbusIP_ESP8266.h>
#include <SPI.h>
#include <ModbusMaster.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include "mqttcomm.h"
#include "oledDisplay.h"
#include "MB_RTU.h"

#define __MODBUS_TCP__ (0)
#define __MQTT__ (1)

// put function declarations here:
void displayTask(void *parameters);
void dataGetMB(void *parameters);
void dataSendMQTT(void *parameters);
#if __MODBUS_TCP__
void dataSendMB(void *parameters);
#endif

const char *ssid            = "Top";
const char *pass            = "1234567890";

#if __MODBUS_TCP__
//Modbus Registers TCP
/******** Holding Register (0x03) *********/
const uint16_t regAirTempH = 0;
const uint16_t regAirTempL = 1;
const uint16_t regAirMoistH = 2;
const uint16_t regAirMoistL = 3;
const uint16_t regSoilMoistH = 4;
const uint16_t regSoilMoistL = 5;
const uint16_t regLuxH = 6;

/****** Discreate Output Coil(0x05) *******/
const uint16_t regCoil = 0;
const uint16_t coilPin = 27;

ModbusIP mbIP;
#endif

#if __MQTT__
mqttcomm mqtt;
const uint16_t coilPin = 27;
#endif

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  Serial.begin(115200);

  pinMode(coilPin, OUTPUT);
  pinMode(STATUSBUTTON, INPUT);
  pinMode(PUMPBUTTON, INPUT);
  pinMode(AUTOBUTTON, INPUT);
  pinMode(ENTERBUTTON, INPUT);


  MBSetup();
  mqtt.setup_mqtt();
  
  #if __MODBUS_TCP__
  mbIP.server();

  pinMode(coilPin, OUTPUT);

  mbIP.addHreg(regAirMoistH);  //moist int value
  mbIP.addHreg(regAirMoistL);  //moist dec value
  mbIP.addHreg(regAirTempH);  //temp int value
  mbIP.addHreg(regAirTempL);  //temp dec value
  mbIP.addHreg(regSoilMoistH);  //soil int value
  mbIP.addHreg(regAirMoistL);  //soil dec value
  mbIP.addHreg(regLuxH);  //lux int value

  //configure coil
  mbIP.addCoil(regCoil); //coil 0

  digitalWrite(coilPin, 0);
  #endif
  
  OledSetup();
  delay(1000); 

  #if __MODBUS_TCP__
  xTaskCreate(dataSendMB, "Send Modbus IP", 2048, NULL, 1, NULL);
  #endif

  xTaskCreate(dataGetMB, "Get Modbus Master", 2048, NULL, 1, NULL);
  xTaskCreate(displayTask, "Display Data", 4096, NULL, 1, NULL);
  xTaskCreate(dataSendMQTT, "Send Over MQTT", 2048, NULL, 1, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void displayTask(void *parameters){
  for(;;){
    //OledData();
    OledRun();
    vTaskDelay(500/portTICK_PERIOD_MS);
  }
}

void dataGetMB(void *parameters){
  for(;;){
    MBRun();
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

void dataSendMQTT(void *Parameters){
  for(;;){
    mqtt.loop_mqtt();
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

#if __MODBUS_TCP__
void dataSendMB(void *parameters){
  for(;;){    
    mbIP.task();

    int _tInt = temp;
    int _tDec = temp - _tInt;
    mbIP.Hreg(0, _tInt);
    mbIP.Hreg(1, _tDec);

    int _hInt = moist;
    int _hDec = moist - _hInt;
    mbIP.Hreg(2, _hInt);
    mbIP.Hreg(3, _hDec);

    int _sInt = soil;
    int _sDec = soil - _sInt;
    mbIP.Hreg(4, _sInt);
    mbIP.Hreg(5, _sDec);

    int _lInt = lumen;
    mbIP.Hreg(6, _lInt);

    mbIP.task();

    int _coil = mbIP.Coil(regCoil);
    digitalWrite(coilPin, _coil);
    //Serial.println(mbIP.Coil(regCoil));
    vTaskDelay(200/portTICK_PERIOD_MS);
    
  }
}
#endif

