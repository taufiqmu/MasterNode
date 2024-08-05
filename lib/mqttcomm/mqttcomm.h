//#include <PubSubClient.h>
#include <WiFi.h>

#ifndef MQTTCOMM_H
#define MQTTCOMM_H

//String string, topictemp;
//int status;
extern String sTemp, sMoist, sSoil, sLumen;

void callback(char* topic, byte* payload, unsigned int length);
//void publisher(const char *topic1, const char *topic2, const char *topic3, const char *topic4);

class mqttcomm{
    private:    
    const char *mqtt_server = "broker.hivemq.com";
    const char *ssid        = "Top";
    const char *password    = "9bdymyka";
    const char *topicSubs1  = "mfmelon/ghwates/kontrol/pompa/1";
    const char *topicSubs2  = "mfmelon/ghwates/state/auto/1";
    const char *topicSubs3  = "mfmelon/ghwates/state/manual/1";
    const char *topicSubs4  = "mfmelon/ghwates/setting/jam1/1";
    const char *topicSubs5  = "mfmelon/ghwates/setting/menit1/1";
    const char *topicSubs6  = "mfmelon/ghwates/setting/jam2/1";
    const char *topicSubs7  = "mfmelon/ghwates/setting/menit2/1";
    const char *topicSubs8  = "mfmelon/ghwates/setting/jam3/1";
    const char *topicSubs9  = "mfmelon/ghwates/setting/menit3/1";
    const char *topicSubs10 = "mfmelon/ghwates/setting/total/1";
    const char *topicPubs1  = "mfmelon/ghwates/monitor/temp/1";
    const char *topicPubs2  = "mfmelon/ghwates/monitor/moist/1";
    const char *topicPubs3  = "mfmelon/ghwates/monitor/soil/1";
    const char *topicPubs4  = "mfmelon/ghwates/monitor/lumen/1";

    public:
    void setup_mqtt();
    void loop_mqtt();
    void setup_wifi(const char*, const char*);
    void reconnect(const char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*, const char*);
    void publisher(const char*, const char*, const char*, const char*);
};

#endif