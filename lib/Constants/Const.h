#include <Arduino.h>

class Variables{
    public:

        // -------------X CONST WIFI X------------- // 
        const char* WIFI_SSID = "Root Main";
        const char* WIFI_PASS = "1115062002ja";

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
        int PIN_IN[4] = {15, 13, 12, 14};
        int PIN_OUTS[2] = {25, 26};
        int PIN_DESARM = 25;
        int PIN_ARM = 26;
        
        // -------------X CONST API X------------- // 
        String hostApi = "http://api-tcc-oficial.herokuapp.com";
        String pathUpdate = "/alarm/update/status";
        String pathStatus = "/alarm/status";
        String pathCreateLog = "/create/log";
};