#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqttcomm.h"


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
        reconnect(topicSubs1);
    }
    client.loop();

    publisher(topicPubs1, topicPubs1, topicPubs3, topicPubs4);
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

void mqttcomm::reconnect(const char* topic){
    while(!client.connected()){
        Serial.print("Attempting MQTT Connection...");
        if(client.connect("ESPClient")){
            Serial.println("connected");
            client.subscribe(topic);
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
        Serial.print(" ");
        status = string.toInt();
        Serial.print("status: ");
        Serial.println(status);
        digitalWrite(27, status);
        delay(15);
    }
}