#include <WiFi.h>
#include <HTTPClient.h>
#include <ESPSupabase.h>
#include "mqttcomm.h"

//void sendData2Server(const String &data);
void postData(String json);

// const char *srvAddr= "192.168.126.237";
// const uint16_t srvPort = 3000;

const char* supabaseURL = "https://edggtblrgdscfjhkznkw.supabase.co";
const char* supabaseApiKey = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6ImVkZ2d0YmxyZ2RzY2ZqaGt6bmt3Iiwicm9sZSI6ImFub24iLCJpYXQiOjE3MjMwMDUwNzIsImV4cCI6MjAzODU4MTA3Mn0.TtYY0AVPuVbQcJBBTXDvdPxEh6ffiUjL81XqIrHHqb4";
String table = "dataNode";
String node = "2";
bool upsert = true;

unsigned long prevMillDB = 0;
unsigned long intervalDB = 60000;

Supabase db;

//String sTemp, sMoist, sSoil, sLumen;


void DBDataCreate(void){
  unsigned long currMillDB = millis();
  if(currMillDB - prevMillDB > intervalDB){
    //String JSON = "{\"time\": \"NOW()\", \"lumen\":" + sLumen + ", \"soil\":" + sSoil + ", \"moist\":" + sMoist + ", \"temp\":" + sTemp + ", \"id_gh\":" + node + "}";
    String JSON = "{\"time\": \"NOW()\", \"lumen\":" + sLumen + ", \"soil\":" + sSoil + ", \"moist\":" + sMoist + ", \"temp\":" + sTemp + ", \"id_gh\":" + node + "}";
    postData(JSON);
    //String data2Send = "{\"datetime\": \"NOW()\", \"lumen\":" + sLumen + ", \"soil\":" + sSoil + ", \"moist\":" + sMoist + ", \"temp\":" + sTemp + ", \"id_gh\":" + 1 + "}";
    // sendData2Server(data2Send);

    prevMillDB = currMillDB;
  }
}

void postData(String json){
  if (WiFi.status() == WL_CONNECTED) {
    db.begin(supabaseURL, supabaseApiKey);

    // Uncomment this line below, if you activate RLS in your Supabase Table
    // db.login_email("email", "password");
    
    // You can also use
    // db.login_phone("phone", "password");

    int code = db.insert(table, json, upsert);
    Serial.println(code);
    db.urlQuery_reset();
  }
}
// void sendData2Server(const String &data){
  //  WiFiClient client;
  //  HTTPClient http;

//   Serial.println("Sending data to server...");

//   if(http.begin(client, "http://" + String(srvAddr) + ":" + String(srvPort) + "/node1")){
//     http.addHeader("Content-Type", "application/json");

//     int httpResponseCode = http.POST(data);

//     if(httpResponseCode > 0){
//       String response = http.getString();
//       Serial.println("Server response: " + response);
//     }else{
//       Serial.println("Error sending POST request");
//     }
//     http.end();
//   }else{
//     Serial.println("Error in HTTP Connection");
//   }
// }