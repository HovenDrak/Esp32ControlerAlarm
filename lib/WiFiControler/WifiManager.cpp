#include <WifiManager.h>
#include <Arduino.h>
#include <Const.h>
#include <WiFi.h>

Variables varWifi;

WifiManager::WifiManager(){}

void WifiManager::wifiCheckConnection(){
	if (WiFi.status() == WL_CONNECTED)
    	return;
  	else
    	Serial.println("WIFI DESCONECTADO!!!");

  	Serial.printf("[WIFI] CONECTANDO AO WIFI: %s \n", varWifi.WIFI_SSID);

	WiFi.mode(WIFI_STA);
  	WiFi.begin(varWifi.WIFI_SSID, varWifi.WIFI_PASS);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	randomSeed(micros());

	Serial.println("[WIFI] WIFI CONECTADO!!!");
  	Serial.print("[WIFI] IP ADDRESS: ");
  	Serial.println(WiFi.localIP());
}