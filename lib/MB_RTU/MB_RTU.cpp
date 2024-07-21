#include <Arduino.h>
#include <ModbusMaster.h>
#include "MB_RTU.h"
#include "mqttcomm.h"

void CheckDataChanged();
void UpdateValues();
void preTransmission();
void postTransmission();

#define MODBUSID 1
#define BAUDRATE 115200

ModbusMaster node1;

signed int holdingRegs[] = {0,4};
signed int valueRegsHR[10];
signed int checkValueHR[10];

int temp, moist, soil, lumen, pres, hum = 0;

void MBSetup(){
  Serial2.begin(115200);
  node1.begin(MODBUSID, Serial2);
  
  node1.preTransmission(preTransmission);
  node1.postTransmission(postTransmission);
}

void MBRun(){
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

void preTransmission(){
  Serial2.flush();
}

void postTransmission(){
  delay(500);
}