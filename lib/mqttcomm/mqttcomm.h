//#include <PubSubClient.h>
#include <WiFi.h>

#ifndef MQTTCOMM_H
#define MQTTCOMM_H

//String string, topictemp;
//int status;

void callback(char* topic, byte* payload, unsigned int length);
//void publisher(const char *topic1, const char *topic2, const char *topic3, const char *topic4);

class mqttcomm{
    private:    
    const char *mqtt_server = "broker.hivemq.com";
    const char *ssid        = "Top";
    const char *password    = "1234567890";
    const char *topicSubs1  = "mfmelon/ghwates/kontrol/pompa/1";
    const char *topicPubs1  = "mfmelon/ghwates/monitor/temp/1";
    const char *topicPubs2  = "mfmelon/ghwates/monitor/moist/1";
    const char *topicPubs3  = "mfmelon/ghwates/monitor/soil/1";
    const char *topicPubs4  = "mfmelon/ghwates/monitor/lumen/1";

    public:
    void setup_mqtt();
    void loop_mqtt();
    void setup_wifi(const char*, const char*);
    void reconnect(const char*);
    void publisher(const char*, const char*, const char*, const char*);
};

#endif