#include <Arduino.h>
#include <ModbusIP_ESP8266.h>
#include <SPI.h>
#include <

// put function declarations here:
void displayTask(void *parameters);
void dataGetMB(void *parameters);
void dataSendMB(void *parameters);
void dataSendMQTT(void *parameters);

const char *ssid = "Top";
const char *pass = "9bdymykaqedqz72";

//Modbus Registers
/******** Holding Register (0x03) *********/
const uint16_t regAirMoistH = 0;
const uint16_t regAirMoistL = 1;
const uint16_t regAirTempH = 2;
const uint16_t regAirTempL = 3;
const uint16_t regSoilMoistH = 4;
const uint16_t regSoilMoistL = 5;
const uint16_t regLuxH = 6;

/****** Discreate Output Coil(0x05) *******/
const uint16_t regCoil = 0;

const uint16_t coilPin = 27;

ModbusIP mbIP;

String temp, moist, soil, lumen;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600);

  WiFi.begin(ssid, pass);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

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
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(coilPin, 1);
  Serial.println("nilai1");
  delay(5000);
  digitalWrite(coilPin, 0); 
  Serial.println("nilai0");
  delay(5000);
}

void displayTask(void *parameters){

}

void dataGetMB(void *parameters){

}

void dataSendMB(void *parameters){
  mbIP.task();

}

void dataSendMQTT(void *Parameters){

}