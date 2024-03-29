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

int STATUS_PIN_IN[4] = {0, 0, 0, 0};
boolean panelVioled = false;

IOManager::IOManager(){}

void IOManager::pinSetConfig(){
    for (int i = 0; i < 2; i++){
        pinMode(varIO.PIN_OUTS[i], OUTPUT);
        digitalWrite(varIO.PIN_OUTS[i], LOW);
    }

    for (int i = 0; i < 3; i++)
        pinMode(varIO.PIN_IN[i], INPUT_PULLUP);
}

void IOManager::sensorCheckAll(){
    if (mqttControl.getClientConnected()){
        for (int i = 0; i < 3 ; i++){
            switch (digitalRead(varIO.PIN_IN[i])){
                case 0:
                    if (STATUS_PIN_IN[i] != 0 || STATUS_PIN_IN[i] == 2){
                        STATUS_PIN_IN[i] = 0;
                        mqttControl.updateStateMqttApi(varIO.TOPIC_PIN_IN[i], "\"fechado\"");
                    }
                    break;
                case 1:
                    if (STATUS_PIN_IN[i] == 2)
                        break;
                    
                    if (mqttControl.getcurrentAlarmState().equals(varIO.CMND_ARM)){
                        STATUS_PIN_IN[i] = 1;
                        panelVioled = true;

                        mqttControl.cmndAlarm(varIO.CMND_VIOLED, "\"Central\"");
                        mqttControl.updateStateMqttApi("alarme", varIO.CMND_VIOLED);
                        mqttControl.updateStateMqttApi(varIO.TOPIC_PIN_IN[i], "\"aberto\"");
                        apiControl.createEvent("em Disparo", "\"Central\"", "setor" + String(i) + "Violed");
                        
                    } else {
                        if (STATUS_PIN_IN[i] != 1){
                            STATUS_PIN_IN[i] = 1;
                            mqttControl.updateStateMqttApi(varIO.TOPIC_PIN_IN[i], "\"aberto\"");
                        }
                    }
                    break;

                default:
                    Serial.println("[ESP32] ERROR AO LER O STATUS DO SENSOR");
                    break;
            }
        }
    } else
        Serial.println("[ESP32] CLIENTE MQTT DESCONECTADO, IMPOSSIVEL LER OS SENSORES");
}

void IOManager::panelArm(){
    digitalWrite(varIO.PIN_ARM, HIGH);
    digitalWrite(varIO.PIN_DESARM, LOW);
}

void IOManager::panelDisarm(){
    panelVioled = false;
    for (int i = 0; i < 3; i++){
        if (STATUS_PIN_IN[i] == 2){
            STATUS_PIN_IN[i] == 1;
            mqttControl.updateStateMqttApi(varIO.TOPIC_PIN_IN[i], "\"aberto\"");
        }
    }
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
    if(panelVioled)
        return false;
    
    for (int i = 0; i < 3 ; i++){
        switch (STATUS_PIN_IN[i]){
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

boolean IOManager::verifyPanelVioled(){
    return panelVioled;
}

void IOManager::setPanelVioled(boolean violed){
    panelVioled = violed;
}

void IOManager::setorBypass(int setor, String user){
    // for (String element : varIO.TOPIC_PIN_IN){
    //     Serial.println(element);
    // }
    STATUS_PIN_IN[setor] = 2;
    mqttControl.updateStateMqttApi(varIO.TOPIC_PIN_IN[setor], "\"bypassed\"");
    apiControl.createEvent("Setor " + varIO.TOPIC_PIN_IN[setor] + " inibido", user, "bypass" + setor);
}

void IOManager::verifySensorsBypass(){
    for(int i = 0; i < 3; i++){
        if(STATUS_PIN_IN[i] == 2){
            STATUS_PIN_IN[i] = 1;
        }
    }
}

