#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqttcomm.h"
#include "oledDisplay.h"
#include "ScheduleWatering.h"

WiFiClient espClient;
PubSubClient client(espClient);

//String topictemp, string;
int status;
String sTemp, sMoist, sSoil, sLumen;

void mqttcomm::setup_mqtt(){
    setup_wifi(ssid, password);
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
}

void mqttcomm::loop_mqtt(){
    
    if(!client.connected()){
        reconnect(topicSubs1, topicSubs2, topicSubs3, topicSubs4, topicSubs5, topicSubs6, topicSubs7, topicSubs8, topicSubs9, topicSubs10);
    }
    client.loop();
    
    client.publish(topicPubs1, sTemp.c_str());
    client.publish(topicPubs2, sMoist.c_str());
    client.publish(topicPubs3, sSoil.c_str());
    client.publish(topicPubs4, sLumen.c_str());
    // publisher(topicPubs1, topicPubs1, topicPubs3, topicPubs4);
}

/*!
*   @brief  setup konfigurasi wifi
*   @param  ssid
*           nama jaringan/wifi/hotspot
*   @param  password
*           password jaringan/wifi/hotspot 
* */
void mqttcomm::setup_wifi(const char* id, const char* pass){
    delay(10);
    Serial.println();
    Serial.print("Connectiong to ");
    Serial.println(id);
    WiFi.begin(id, pass);

    while(WiFi.status() != WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi Connected");
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
}


void mqttcomm::publisher(const char *topic1, const char *topic2, const char *topic3, const char *topic4){ 
    sTemp = "12";
    client.publish(topic1, sTemp.c_str());
    client.publish(topic2, sMoist.c_str());
    client.publish(topic2, sSoil.c_str());
    client.publish(topic2, sLumen.c_str());
}

void mqttcomm::reconnect(const char* topic1, const char* topic2, const char* topic3, const char* topic4, const char* topic5, const char* topic6, const char* topic7, const char* topic8, const char* topic9, const char* topic10){
    while(!client.connected()){
        Serial.print("Attempting MQTT Connection...");
        if(client.connect("ESPClient")){
            Serial.println("connected");
            client.subscribe(topic1);
            client.subscribe(topic2);
            client.subscribe(topic3);
            client.subscribe(topic4);
            client.subscribe(topic5);
            client.subscribe(topic6);
            client.subscribe(topic7);
            client.subscribe(topic8);
            client.subscribe(topic9);
            client.subscribe(topic10);
        }else{
            Serial.print("failed, rc = ");
            Serial.print(client.state());
            Serial.print(" try again in 2 seconds");
            delay(2000);
        }
    }
}


/*!
*   @brief  response saat menerima topik
*   @param  topic
*           topic mqtt
*   @param  payload
*           isi pesan
*   @param  length
*           panjang pesan
*   @param  topic_user
*           isikan topic yang ingin didapat
*   @param  pin
*           pin microcontroller yang ingin dikontrol
* */
void callback(char* topic, byte* payload, unsigned int length){
    String string;
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("]");
    String topictemp = topic;


    for(int i = 0; i < length; i++){
        string+=(char)payload[i];
        //status = string.toInt();
        //Serial.print("status: ");
    }
    Serial.println();
    if(topictemp == "mfmelon/ghwates/kontrol/pompa/1"){
        if(pumpControl == 1){
            Serial.print(" ");
            if(string == "true"){
                string = "1";
            }else if(string == "false"){
                string = "0";
            }
            relayState = string.toInt();
            Serial.print("status: ");
            Serial.println(relayState);
            digitalWrite(27, relayState);
            delay(15);
        }
    }
    
    if(topictemp == "mfmelon/ghwates/state/manual/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("manual: ");
        Serial.println(status);
        pumpControl = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/state/auto/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("auto: ");
        Serial.println(status);
        autoControl = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/setting/jam1/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("jam1: ");
        Serial.println(status);
        waktu[0] = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/setting/menit1/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("menit1: ");
        Serial.println(status);
        waktu[1] = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/setting/jam2/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("jam2: ");
        Serial.println(status);
        waktu[2] = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/setting/menit2/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("menit2: ");
        Serial.println(status);
        waktu[3] = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/setting/jam3/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("jam3: ");
        Serial.println(status);
        waktu[4] = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/setting/menit3/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("menit3: ");
        Serial.println(status);
        waktu[5] = status;
        delay(15);
    }
    
    if(topictemp == "mfmelon/ghwates/setting/total/1"){
        Serial.print(" ");
        status = string.toInt();
        Serial.print("Total Plants: ");
        Serial.println(status);
        totalPlants = status;
        delay(15);
    }
}