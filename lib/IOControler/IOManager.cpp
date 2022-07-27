#include <PubSubClient.h>
#include <MqttManager.h>
#include <ApiManager.h>
#include <IOManager.h>
#include <Arduino.h>
#include <Const.h>

extern String currentAlarmState;
ApiControler apiControl;
MqttManager mqttControl;
Variables varIO;

String TOPIC_PIN_IN[4] = {"setor1", "setor2", "setor3", "setor4"};
int STATUS_PIN_IN[4] = {0, 0, 0, 0};

IOManager::IOManager(){}

void IOManager::pinSetConfig(){
    for(int i = 0; i < 2; i++){
        pinMode(varIO.PIN_OUTS[i], OUTPUT);
        digitalWrite(varIO.PIN_OUTS[i], LOW);
    }
    for(int i = 0; i < 4; i++){
        pinMode(varIO.PIN_IN[i], INPUT_PULLUP);
    }
}

void IOManager::sensorCheckAll(){
    if(mqttControl.getClientConnected()){
        for(int i = 0; i < 4 ; i++){
            switch (digitalRead(varIO.PIN_IN[i])){
                case 0:
                    if(STATUS_PIN_IN[i] != 0 || STATUS_PIN_IN[i] == 2){
                        STATUS_PIN_IN[i] = 0;
                        mqttControl.updateStateMqttApi(TOPIC_PIN_IN[i], "\"fechado\"");
                    }
                    break;
                case 1:
                    if(STATUS_PIN_IN[i] == 2){
                        break;
                    }
                    if(mqttControl.getcurrentAlarmState().equals(varIO.CMND_ARM)){
                        STATUS_PIN_IN[i] = 1;
                        mqttControl.cmndAlarm(varIO.CMND_VIOLED, "SYSTEM");
                        mqttControl.updateStateMqttApi(TOPIC_PIN_IN[i], "\"aberto\"");
                    } else{
                        if(STATUS_PIN_IN[i] != 1){
                            STATUS_PIN_IN[i] = 1;
                            mqttControl.updateStateMqttApi(TOPIC_PIN_IN[i], "\"aberto\"");
                        }
                    }
                    break;

                default:
                    Serial.println("[ESP32] ERROR AO LER O STATUS DO SENSOR");
                    break;
            }
        }
    } else{
        Serial.println("[ESP32] CLIENTE MQTT DESCONECTADO, IMPOSSIVEL LER OS SENSORES");
    }
    if(mqttControl.getcurrentAlarmState().equals(varIO.CMND_VIOLED)){
        panelViolated();
        Serial.println("[ESP32] PAINEL VIOLADO!!!");
    }
}

void IOManager::panelArm(){
    digitalWrite(varIO.PIN_ARM, HIGH);
    digitalWrite(varIO.PIN_DESARM, LOW);
}

void IOManager::panelDisarm(){
    digitalWrite(varIO.PIN_ARM, LOW);
    digitalWrite(varIO.PIN_DESARM, HIGH);
}

void IOManager::panelViolated(){
    digitalWrite(varIO.PIN_ARM, LOW);
    delay(500);
    digitalWrite(varIO.PIN_ARM, HIGH);
    delay(500);
    digitalWrite(varIO.PIN_ARM, LOW);
    delay(500);
    digitalWrite(varIO.PIN_ARM, HIGH);
    delay(500);
}

boolean IOManager::verifyCanArm(){
    boolean b = true;
    for(int i = 0; i < 4 ; i++){
        switch(STATUS_PIN_IN[i]){
            case 0:
                b = true;
                break;
            case 1:
                b = false;
                break;
        }
    }
    return b;
}

void IOManager::setorBypass(int setor){
    STATUS_PIN_IN[setor] = 2;
}


