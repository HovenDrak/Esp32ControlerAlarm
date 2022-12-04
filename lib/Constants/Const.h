#include <Arduino.h>

class Variables{
    public:
        // -------------X CONST WIFI X------------- // 
        const char* WIFI_SSID = "Leia_WIFI";
        const char* WIFI_PASS = "Leia1301";

        // -------------X CONST MQTT X------------- // 
        const char* MQTT_SERVER = "bbfb08f6f1b84ffebf8b0c4fbbcd0e90.s1.eu.hivemq.cloud";
        const int MQTT_PORT = 8883;
        const char* TOPIC_CMND_ALARM_BYPASS = "cmnd/alarme/bypass";
        const char* TOPIC_CMND_ALARM = "cmnd/alarme";
        const char* TOPIC_SENSOR = "cmnd/sensor";
        const char* MQTT_PASSWORD = "67Esp3267";
        const char* MQTT_USER = "userESP";
        String CLIENT_ID = "ESP32Client-";
        String CMND_DESARM = "\"desarmado\"";
        String CMND_VIOLED = "\"disparado\"";
        String CMND_ARM = "\"armado\"";
        
        // -------------X CONST PINS X------------- // 
        String TOPIC_PIN_IN[3] = {"setor1", "setor2", "setor3"};

        int PIN_IN[3] = {15, 14, 12};
        int PIN_OUTS[2] = {25, 26};
        int PIN_DESARM = 25;
        int PIN_ARM = 26;
        
        // -------------X CONST API X------------- // 
        String hostApi = "http://54.174.25.158";
        String pathUpdate = "/alarm/update/status";
        String pathCreateLog = "/create/log";
        String pathStatus = "/alarm/status";
};