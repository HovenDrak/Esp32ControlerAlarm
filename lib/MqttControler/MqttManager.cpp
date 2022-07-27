#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <MqttManager.h>
#include <ApiManager.h>
#include <IOManager.h>
#include <Arduino.h>
#include <iostream>
#include <Const.h>
#include <JSON.h>

WiFiClientSecure espClient;
PubSubClient client(espClient);
ApiControler controlApi;
MqttManager controlMqtt;
IOManager controlIO;
Variables varMqtt;

static const char *ROOT_CA PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";

boolean clientConnected = false;
String currentAlarmState;
String result = "ERROR";
String canArm;

MqttManager::MqttManager(){}

void callback(char* topic, byte* payload, unsigned int length) {

  String msgMqtt = "";
  String topico = String(topic);
  
  for (int i = 0; i < length; i++){
    msgMqtt += (char)payload[i];
  }

  JSONVar msgMqttJson = JSON.parse(msgMqtt);
  Serial.println("[MQTT] MENSAGEM RECEBIDA >>> Topico: " + String(topic) + " Mensagem: " + JSON.stringify(msgMqttJson));

  if(topico.equals(varMqtt.TOPIC_CMND_ALARM)){ 
    String cmnd = JSON.stringify(msgMqttJson[0]["newState"]);
    String user = JSON.stringify(msgMqttJson[1]["user"]);
    controlMqtt.cmndAlarm(cmnd, user);
  } else if (topico.equals(varMqtt.TOPIC_CMND_ALARM_BYPASS)){
      for(int i = 0; i < msgMqttJson["setor_bypass"].length(); i++){
        int setor = msgMqttJson["setor_bypass"][i]["setor"];
        controlIO.setorBypass(setor);
      }
  } else {
    Serial.println("[MQTT] TOPICO NÃO IDENTIFICADO!!!");
  }
}

void MqttManager::reconnect() {
  while (!client.connected()) {
    Serial.print("[MQTT] TENTANDO SE CONECTAR AO BROKER MQTT, ");

    varMqtt.CLIENT_ID += String(random(0xffff), HEX);

    if (client.connect(varMqtt.CLIENT_ID.c_str(), varMqtt.MQTT_USER, varMqtt.MQTT_PASSWORD)) {
      Serial.println("CONECTADO!!!");
      client.subscribe(varMqtt.TOPIC_CMND_ALARM);
      client.subscribe(varMqtt.TOPIC_SENSOR);
      client.subscribe(varMqtt.TOPIC_CMND_ALARM_BYPASS);
      clientConnected = true;
      canArm = "y";
      consultAllState();
      
    } else if(client.state() == -1) {
      clientConnected = false;
      Serial.print("[MQTT] FALHOU, STATE = ");
      Serial.print(client.state());
      Serial.println("PROXIMA TENTATIVA EM 3s...");
      delay(3000);

    } else if(client.state() == -2){
      Serial.println("[MQTT] FALHOU, STATE = -2");
      Serial.println("[ESP32] Reiniciando ESP...");
      clientConnected = false;
      ESP.restart();
    }
    
    else {
      clientConnected = false;
      Serial.print("[MQTT] FALHOU, STATE = ");
      Serial.print(client.state());
      Serial.println("PROXIMA TENTATIVA EM 5s...");
      delay(5000);
    }
  }
}

void MqttManager::mqttSetConfigs(){
  espClient.setCACert(ROOT_CA);
  client.setServer(varMqtt.MQTT_SERVER, varMqtt.MQTT_PORT);
  client.setCallback(callback);
}

void MqttManager::mqttRun(){
  if (!client.connected()) {
      reconnect();
  } 
  client.loop();
}

void MqttManager::updateStateMqttApi(String service, String state){
  String topic = "status/" + service;

  Serial.println("[MQTT] ATUALIZANDO STATUS MQTT >>> Service: " + service + " Status: " + state);
  client.publish(topic.c_str(), state.c_str());
  delay(500);
  controlApi.updateStatus(service, state);
  delay(500);
}

void MqttManager::updateStateMqtt(String topic, String msg){
  client.publish(topic.c_str(), msg.c_str());
}

void MqttManager::consultAllState(){
  while(result.equals("ERROR")){
    result = controlApi.getStatus("alarme");
  }
  cmndAlarm(result, "SYSTEM");
  delay(200);
}

void MqttManager::cmndAlarm(String cmnd, String user){
  Serial.println("[ESP32] COMANDO DE ALARME FOI REQUISITADO PELO USUÁRIO: " + user);
  if(cmnd.equals(varMqtt.CMND_ARM)){
    if(controlIO.verifyCanArm()){
      controlIO.panelArm();
      Serial.println("[ESP32] PAINEL ARMADO.");
      if(user.equals("alexa") || user.equals("mobile")){
        controlApi.createLog("Casa Armado", user);
      }
    } else{
      Serial.println("[ESP32] NÃO FOI POSSIVEL EFETUAR O ARME, EXISTE SETOR ABERTO.");
      controlMqtt.updateStateMqtt("status/error/alarme", "Existe setor aberto");
      return;
    }
  } 
  else if (cmnd.equals(varMqtt.CMND_DESARM)){ 
    controlIO.panelDisarm();
    Serial.println("[ESP32] PAINEL DESARMADO.");
    if(user.equals("alexa") || user.equals("mobile")){
      controlApi.createLog("Casa Desarmado", user);
    }
  } 
  else if (cmnd.equals(varMqtt.CMND_VIOLED)){ 
    controlIO.panelViolated();
    controlApi.createLog("Casa em Disparo", "esp32");
  } 
  else { Serial.println("[ESP32] COMANDO NAO RECONHECIDO: " + cmnd); }

  currentAlarmState = cmnd;
  Serial.println("[ESP32] STATUS DO ALARME INTERNO ATUALIZANDO PARA: " + currentAlarmState);
  controlMqtt.updateStateMqttApi("alarme", cmnd);
}

String MqttManager::getcurrentAlarmState(){
  return currentAlarmState;
}

boolean MqttManager::getClientConnected(){
  return clientConnected;
}

void MqttManager::setCanArm(String validation){
  canArm = validation;
}

String MqttManager::getCanArm(){
  return canArm;
}



