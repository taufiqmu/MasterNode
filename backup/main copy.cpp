#include <Arduino.h>
#include <ModbusIP_ESP8266.h>
#include <SPI.h>
#include <ModbusMaster.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <Arduino_JSON.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include "mqttcomm.h"

#define __MODBUS_TCP__ (0)
#define __MQTT__ (1)

// put function declarations here:
void displayTask(void *parameters);
void dataGetMB(void *parameters);

#if __MODBUS_TCP__
void dataSendMB(void *parameters);
#endif

void dataSendMQTT(void *parameters);
void CheckDataChanged();
void UpdateValues();
void preTransmission();
void postTransmission();
String httpGETRequest(const char* serverName);

const char *ssid            = "Top";
const char *pass            = "9bdymykaqedqz72";
const char *mqtt_server     = "broker.hivemq.com";
String openWeatherMapApiKey = "accf82339c12902aaba436433d79c886";
String city                 = "Surabaya";
String countryCode          = "ID";

#define MODBUSID 1
#define BAUDRATE 115200
#define TFT_RST 14
#define TFT_CS 33
#define TFT_DC 26


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
WiFiClient espClient;
PubSubClient mqttClient(espClient);

mqttcomm mqtt1;
#endif

ModbusMaster node1;
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

int temp, moist, soil, lumen, pres, hum;
int temper;
String wind;
String sTemp, sMoist, sSoil, sLumen;

signed int holdingRegs[] = {0,4};
signed int valueRegsHR[10];
signed int checkValueHR[10];
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;
String jsonBuffer;


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
  Serial2.begin(115200);
  node1.begin(MODBUSID, Serial2);

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

  node1.preTransmission(preTransmission);
  node1.postTransmission(postTransmission);

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

  delay(1000); 


  #if __MODBUS_TCP__
  xTaskCreate(dataSendMB, "Send Modbus IP", 2048, NULL, 1, NULL);
  #endif

  xTaskCreate(dataGetMB, "Get Modbus Master", 2048, NULL, 1, NULL);
  xTaskCreate(displayTask, "Display Data", 4096, NULL, 1, NULL);
  //xTaskCreate(dataSendMQTT, "Send Over MQTT", 1024, NULL, 2, NULL);
}

void loop() {
  // put your main code here, to run repeatedly:
}

void displayTask(void *parameters){
  for(;;){
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
      else {
        Serial.println("WiFi Disconnected");
      }
      tft.flush();
      lastTime = millis();
    }

    vTaskDelay(2000/portTICK_PERIOD_MS);
  }
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


void dataGetMB(void *parameters){
  for(;;){
    //Serial.println("masuk dataGetMB");
    uint32_t i;
    i++;

    node1.setTransmitBuffer(0, lowWord(i));
    node1.setTransmitBuffer(1, highWord(i));

    int _resultHR = node1.readHoldingRegisters(holdingRegs[0], holdingRegs[1]);
    if(_resultHR == node1.ku8MBSuccess){
      for(int j = 0; j < holdingRegs[1]; j++){
        valueRegsHR[j] = node1.getResponseBuffer(j);
      Serial.println("hReg " + String(j) + ":" + String(valueRegsHR[j]));
      }
    }
    CheckDataChanged();
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void CheckDataChanged(){
  bool _dataChanged = false;
  
  for(int j = 0; j < holdingRegs[1]; j++){
    if(valueRegsHR[j] != checkValueHR[j]){
      checkValueHR[j] = valueRegsHR[j];
      
      _dataChanged = true;
    }
  }

  if(_dataChanged == true){
    UpdateValues();
  }
}

void UpdateValues(){
    /*String tmpstr;
    for(int j = 0; j < holdingRegs[1]; j++){
      tmpstr = "Holding Reg " + String(j) + ": " + String(valueRegsHR[j]);
      Serial.println(tmpstr);
    }*/
    
    temp = valueRegsHR[0];
    moist = valueRegsHR[1];
    soil = valueRegsHR[2];
    lumen = valueRegsHR[3];

    sTemp = String(temp);
    sMoist = String(moist);
    sSoil = String(soil);
    sLumen = String(lumen);
 
    Serial.println(moist);
    Serial.println(temp);
    Serial.println(soil);
    Serial.println(lumen);
    
    delay(100);
    
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


void dataSendMQTT(void *Parameters){
  for(;;){
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
}

void preTransmission(){
  Serial2.flush();
}

void postTransmission(){
  delay(500);
}