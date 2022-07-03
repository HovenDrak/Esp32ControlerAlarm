#include <WifiManager.h>
#include <Arduino.h>
#include <Const.h>
#include <WiFi.h>

Variables varWifi;

WifiManager::WifiManager(){}

void WifiManager::wifiCheckConnection(){
  if (WiFi.status() == WL_CONNECTED){
      return;
  }
  Serial.print("TENTANDO CONEXÃO WIFI...");

  if (WiFi.begin(varWifi.WIFI_SSID, varWifi.WIFI_PASS) == WL_CONNECTED){
      Serial.println("CONECTADO COM SUCESSO NA REDE WIFI!");
      return;
  }
  Serial.println(WiFi.status());
}