#include <Arduino.h>
#include <ArduinoJson.h>
#include <iostream>
#include <Wire.h>
#include <String>
#include "myFunctions.cpp" //Fonctions utilitaires
#include <HTTPClient.h>

using namespace std;

#include "MyOled.h"
MyOled *myOled = NULL;

#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H

#include "MyServer.h" 
MyServer *myServer = NULL;

#include "MyOledViewWifiAp.h"
MyOledViewWifiAp *myOledViewWifiAp = NULL;

#include "MyOledViewInitialisation.h"
MyOledViewInitialisation *myOledViewInitialisation = NULL;

//Variable pour la connection Wifi
const char * SSID = "SAC_";
const char * PASSWORD = "sac_";
String ssIDRandom;

int temps = 0;

// Définition des LEDs
#define GPIO_PIN_LED_LOCK_ROUGE 12 // Led Rouge GPIO12
#define GPIO_PIN_LED_OK_VERT 14 // Led Verte GPIO14
#define GPIO_PIN_LED_HEAT_JAUNE 27 // Led Jaune GPIO16

#define nomSystem "SAC System"
string idDuSysteme = "98262";
 
// Définition des boutons
#include "MyButton.h"
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;
 
#include "TemperatureStub.h"
#define DHTPIN  15  // Pin utilisée par le DHT22
#define DHTTYPE DHT22  //Le type de senseur utilisé
TemperatureStub *temperatureStub = NULL;

// std::string CallBackMessageListener(string message) {
//   while(replaceAll(message, std::string("  "), std::string(" ")));
//   //Décortiquer le message
//   string actionToDo = getValue(message, ' ', 0);
//   string arg1 = getValue(message, ' ', 1);
//   string arg2 = getValue(message, ' ', 2);
//   string arg3 = getValue(message, ' ', 3);
//   string arg4 = getValue(message, ' ', 4);
//   string arg5 = getValue(message, ' ', 5);
//   string arg6 = getValue(message, ' ', 6);
//   string arg7 = getValue(message, ' ', 7);
//   string arg8 = getValue(message, ' ', 8);
//   string arg9 = getValue(message, ' ', 9);
//   string arg10 = getValue(message, ' ', 10);

//   if (string(actionToDo.c_str()).compare(string("button")) == 0) {
//     if(string(arg1.c_str()).compare(string("getTemp")) == 0) {
//       temps = atoi(arg2.c_str());
//       return(String("Ok").c_str());
//     }
//   }
//   std::string result = "";
//   return result;
// }

void setup() {
  Serial.begin(9600);
  
  //Connection au WifiManager
  String ssIDRandom, PASSRandom;
  String stringRandom;
  stringRandom = get_random_string(4).c_str();
  ssIDRandom = SSID;
  ssIDRandom = ssIDRandom + stringRandom;
  stringRandom = get_random_string(4).c_str();
  PASSRandom = PASSWORD;
  PASSRandom = PASSRandom + stringRandom;

  char strToPrint[128];
  sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom, PASSRandom);
  Serial.println(strToPrint);

  if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())) {
    Serial.println("Erreur de connexion.");

  } else {
    Serial.println("Connexion Établie.");
  }

  // ----------- Routes du serveur ----------------
  myServer = new MyServer(80);
  myServer -> initAllRoutes();

  // Initialisation des LEDs
  pinMode(GPIO_PIN_LED_LOCK_ROUGE, OUTPUT);
  pinMode(GPIO_PIN_LED_OK_VERT, OUTPUT);
  pinMode(GPIO_PIN_LED_HEAT_JAUNE, OUTPUT);
 
  //Initialisation des boutons
  myButtonAction = new MyButton();
  myButtonAction->init(T8);
  int sensibilisationButtonAction = myButtonAction->autoSensibilisation();
 
  myButtonReset = new MyButton();
  myButtonReset->init(T9);
  int sensibilisationButtonReset = myButtonReset->autoSensibilisation();

  char strBoutonAction[128];
  sprintf(strBoutonAction, "%d", sensibilisationButtonAction);

  char strBoutonReset[128];
  sprintf(strBoutonReset, "%d", sensibilisationButtonReset);

  //Initiation du senseur de température
  temperatureStub = new TemperatureStub;
  temperatureStub->init(DHTPIN, DHTTYPE);

  myOled = new MyOled(&Wire, -1, 64, 128);
  myOled->init();

  myOledViewInitialisation = new MyOledViewInitialisation();
  myOledViewInitialisation->setIdDuSysteme(idDuSysteme.c_str());
  myOledViewInitialisation->setNomDuSysteme(nomSystem);
  myOledViewInitialisation->setSensibiliteBoutonAction(strBoutonAction);
  myOledViewInitialisation->setSensibiliteBoutonReset(strBoutonReset);
  myOled->displayView(myOledViewInitialisation);
  delay(2000);
  myOled->clearDisplay();

  myOledViewWifiAp = new MyOledViewWifiAp();
  myOledViewWifiAp->setNomDuSysteme(nomSystem);
  myOledViewWifiAp->setSsIDDuSysteme(ssIDRandom.c_str());
  myOledViewWifiAp->setPassDuSysteme(PASSRandom.c_str());
  myOled->displayView(myOledViewWifiAp);

  int i = 0;
  for (i = 0; i < 2; i++){
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, HIGH);
    digitalWrite(GPIO_PIN_LED_OK_VERT, HIGH);
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, HIGH);
    delay(500);
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
    digitalWrite(GPIO_PIN_LED_OK_VERT, LOW);
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, LOW);
    delay(500);
  }
  
}
 
void loop() {
  float t = temperatureStub->getTemperature();
  int i;

  Serial.print("Température : ");
  Serial.println(t);
  delay(2000);
 
  //Gestion du bouton Action
  // int buttonAction = myButtonAction->checkMyButton();
  // if(buttonAction > 2)  {
  //   if(t > 25)
  //   {
  //     Serial.println("Vous avez appuyé sur le bouton d'action ");
  //     for (i = 0; i < 5; i++)
  //     {
  //       digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, HIGH);
  //       delay(500);
  //       digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
  //       delay(500);
  //     }
  //   }
  //   else{
  //     digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, HIGH);
  //     delay(3000);
  //     digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, LOW);
  //   }
  // }
}

