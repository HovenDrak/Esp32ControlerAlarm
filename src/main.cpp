#include <Arduino.h>
#include <WiFi.h>
#include "MqttManager.h"
#include "WifiManager.h"
#include "Const.h"
#include "IOManager.h"
#include <HTTPClient.h>

Variables varMain;
WifiManager wifi; 
MqttManager mqtt;
IOManager io;

unsigned long verifySensor = 0;
unsigned long verifyWifi = 0;
boolean consultState = false;

void setup() {
  Serial.begin(115200);
  io.pinSetConfig();

  wifi.wifiCheckConnection();
  mqtt.mqttSetConfigs();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    mqtt.mqttRun();
  }

  if((millis() - verifyWifi) >= 20000){
    verifyWifi = millis();
    wifi.wifiCheckConnection();
  }
  if((millis() - verifySensor) >= 1000){
    verifySensor = millis();
    io.sensorCheckAll();
  }
}