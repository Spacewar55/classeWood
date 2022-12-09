
/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
using namespace std;

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
}

void MyServer::initAllRoutes() { 
    currentTemperature = 3.3f;

    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/home.html", "text/html");
    });

    //Route du de l'image
    this->on("/sac.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/sac.png", "image/png");
    });

    //Route du script JavaScript
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
    });

    this->on("/lireTemperature", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTempFour");
        String lireTemperatureDuFour = String(repString.c_str());
        Serial.println(lireTemperatureDuFour);
        request->send(200, "text/plain", lireTemperatureDuFour );
    });

    this->on("/getBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String woodApi = "http://51.79.68.103:3000/api/woods/getAllWoods";
        http.begin(woodApi);
        http.addHeader("Authorization", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyRW1haWwiOiJBeW1lcmljX0xhbmRyeUBob3RtYWlsLmNvbSIsImlzQWRtaW4iOiJVU0VSIiwiaWF0IjoxNTgzODUxMDAyfQ.ZnwJsk-q2-yePeRURLUk7KbPiSADs4xLmpYJfgyphP4");

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.GET();
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);

        String response;
        if (httpResponseCode>0) {
            response = http.getString();
        }
        http.end();

        DynamicJsonDocument doc(2048);
        deserializeJson(doc, response);
        String tempToSend;
        for(JsonObject elem : doc.as<JsonArray>()){
            String idBois = elem["_id"];
            String nomBois = elem["name"];
            
            tempToSend += idBois + String("&");
            tempToSend += nomBois + String("&");
        }
        request->send(200, "text/plain", tempToSend);
    });

    this->on("/getCaracteristiqueBois", HTTP_GET, [](AsyncWebServerRequest *request){
        HTTPClient http;
        String woodApi = "51.79.68.103:3000/api/woods/getWood/";
        http.begin(woodApi);
        http.addHeader("Authorization", "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VyRW1haWwiOiJBeW1lcmljX0xhbmRyeUBob3RtYWlsLmNvbSIsImlzQWRtaW4iOiJVU0VSIiwiaWF0IjoxNTgzODUxMDAyfQ.ZnwJsk-q2-yePeRURLUk7KbPiSADs4xLmpYJfgyphP4");

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        int httpResponseCode = http.GET();
        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);

        String response;
        if (httpResponseCode>0) {
            response = http.getString();
            Serial.println(response);
        }
        http.end();

        DynamicJsonDocument doc(2048);
        deserializeJson(doc, response);
        String tempToSend;
        for(JsonObject elem : doc.as<JsonArray>()){
            String nomBois = elem["name"];
            String typeBois = elem["type"];
            String origineBois = elem["origin"];
            String tempsSechageBois = elem["dryingTime"];
            String tempsMinBois = elem["temperature"];

            tempToSend += nomBois + String("&");
            tempToSend += typeBois + String("&");
            tempToSend += origineBois + String("&");
            tempToSend += tempsSechageBois + String("&");
            tempToSend += tempsMinBois + String("&");

            // std::string repSring = "";
            // String stringToSend = "caracteristiques";
            // tempToSend += nomBois + String(" ");
            // tempToSend += typeBois + String(" ");
            // tempToSend += origineBois + String(" ");
            // tempToSend += tempsSechageBois + String(" ");
            // tempToSend += tempsMinBois + String(" ");
            Serial.println(tempToSend);

            request->send(200, "text/plain", tempToSend);
        }
    });

   //route si celle tapé n'existe pas
    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
    });

    this->begin();
};
