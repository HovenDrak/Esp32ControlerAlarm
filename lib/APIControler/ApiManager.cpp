#include <WiFiClientSecure.h>
#include <Arduino_JSON.h>
#include <ApiManager.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <Const.h>
#include <vector>

WiFiClientSecure httpsClient;
Variables varApi;
HTTPClient http;

ApiControler::ApiControler(){}

String ApiControler::getStatus(String service){
    Serial.println("[HTTP] GET STATUS: " + service);

    http.begin(varApi.hostApi + varApi.pathStatus);
    http.addHeader("Content-Type", "application/json");           
  
    int responseCode = http.sendRequest("GET", "{\"name\":\"" + service + "\"}");
    String responseBody = http.getString();
  
    if (responseCode == 200){
        JSONVar responseJson = JSON.parse(responseBody);
        http.end();
        return JSON.stringify(responseJson["status"]);
    } 
    
    http.end();
    Serial.println("[HTTP]: CODE: " + responseCode);
    Serial.println("[HTTP]: BODY: " + responseBody);
    return "ERROR";
}

void ApiControler::updateStatus(String service, String newState){
    Serial.println("[HTTP] UPDATE STATUS >> Service: " + service + " Novo Status: " + newState);

    http.begin(varApi.hostApi + varApi.pathUpdate);
    http.addHeader("Content-Type", "application/json");           
  
    int responseCode = http.PUT("{\"name\":\"" + service + "\", \"status\":" + newState + "}");
  
    if (responseCode == 200){
        Serial.println("[HTTP] UPDATE STATUS SUCESS!!!");

    } else if (responseCode == 500){
        Serial.println("[ERROR 500], NOVA TENTATIVA!!!");
        updateStatus(service, newState);

    } else {
        String response = http.getString();
        Serial.println("[HTTP] ERROR REQUEST UPDATE STATUS!!!");
        Serial.println("[CODE]: " + responseCode);
        Serial.println("[BODY]: " + response);
    }
    http.end();
}

void ApiControler::createEvent(String description, String user, String type){
    Serial.println("[HTTP] CRIANDO LOG >> Description: " + description + " User: " + user + " TypeEvent: " + type);

    http.begin(varApi.hostApi + varApi.pathCreateLog);
    http.addHeader("Content-Type", "application/json");  

    int responseCode = http.sendRequest("POST", "{\"desc\":\"" + description + "\", \"user\":" + user + ", \"type\":\"" + type + "\"}");
  
    if (responseCode == 200){
        Serial.println("[HTTP] LOG CREATE SUCCESS!!!");

    } else if (responseCode == 500){
        Serial.println("[ERROR 500], NOVA TENTATIVA!!!");
        createEvent(description, user, type);

    } else{
        String response = http.getString();
        Serial.println("[HTTP] ERROR " + String(responseCode) + "REQUEST LOG!!!");
        Serial.println("[BODY]: " + response);
    }
    http.end();
}
