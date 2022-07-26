#include <Arduino.h>
#include <PubSubClient.h>

class MqttManager{
    public:
        MqttManager();
        void updateStateMqttApi(String service, String state);
        void updateStateMqtt(String topic, String msg);
        void cmndLight(String cmnd, int light);
        void setCanArm(String validation);
        String getcurrentLight1State();
        String getcurrentAlarmState();
        boolean getClientConnected();
        void cmndAlarm(String cmnd, String user);
        void consultAllState();
        void mqttSetConfigs();
        String getCanArm();
        void mqttRun();
        
    private:
        void reconnect();
        
};

