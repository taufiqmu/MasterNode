#include <WiFi.h>
#include <HTTPClient.h>
#include "mqttcomm.h"

void sendData2Server(const String &data);

const char *srvAddr= "192.168.126.237";
const uint16_t srvPort = 3000;

unsigned long prevMillDB = 0;
unsigned long intervalDB = 60000;

//String sTemp, sMoist, sSoil, sLumen;

void DBDataCreate(void){
  unsigned long currMillDB = millis();
  if(currMillDB - prevMillDB > intervalDB){
    //String data2Send = "{\"datetime\": \"NOW()\", \"temp\":" + sTemp + ", \"moist\":" + sMoist + ", \"soil\":" + sSoil + ", \"lumen\":" + sLumen + "}";
    String data2Send = "{\"time\": \"NOW()\", \"temp\":" + sTemp + ", \"moist\":" + sMoist + ", \"soil\":" + sSoil + ", \"lumen\":" + sLumen + "}";
    sendData2Server(data2Send);

    prevMillDB = currMillDB;
  }
}


void sendData2Server(const String &data){
  WiFiClient client;
  HTTPClient http;

  Serial.println("Sending data to server...");

  //if(http.begin(client, "http://" + String(srvAddr) + ":" + String(srvPort) + "/node1")){
  if(http.begin(client, "https://qvucelnrpovddidaydir.supabase.co/rest/v1/node_1")){
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(data);

    if(httpResponseCode > 0){
      String response = http.getString();
      Serial.println("Server response: " + response);
    }else{
      Serial.println("Error sending POST request");
    }
    http.end();
  }else{
    Serial.println("Error in HTTP Connection");
  }
}