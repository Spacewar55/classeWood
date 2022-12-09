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

#include "MyOledViewWorkingOFF.h"
MyOledViewWorkingOFF *myOledViewWorkingOFF = NULL;

#include "MyOledViewWorkingCOLD.h"
MyOledViewWorkingCOLD *myOledViewWorkingCOLD = NULL;

#include "MyOledViewWorkingHEAT.h"
MyOledViewWorkingHEAT *myOledViewWorkingHEAT = NULL;

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

//Récupérer la température
float temperatureDuFour = 20;
char strTemperatureDuFour[128];

//Récupérer la liste des bois
char strListeBois[128];

string etatFour = "OFF";
int currentTemperatureDisplayed = 0;

int temperatureActuelleDuFour = 24;

std::string CallBackMessageListener(string message) {
  while(replaceAll(message, std::string("  "), std::string(" ")));
  //Décortiquer le message
  string actionToDo = getValue(message, ' ', 0);
  string actionToDo1 = getValue(message, ' ', 1);

  if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
    sprintf(strTemperatureDuFour, "%4.1f", temperatureDuFour);
    Serial.println(strTemperatureDuFour);
    return(strTemperatureDuFour);
  }

  std::string result = "";
  return result;
}

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
  myServer->initAllRoutes();
  myServer->initCallback(&CallBackMessageListener);

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

  //Initiation du senseur de température
  temperatureStub = new TemperatureStub;
  temperatureStub->init(DHTPIN, DHTTYPE);

  char strBoutonAction[128];
  sprintf(strBoutonAction, "%d", sensibilisationButtonAction);

  char strBoutonReset[128];
  sprintf(strBoutonReset, "%d", sensibilisationButtonReset);

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
  delay(2000);
  myOled->clearDisplay();

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

void displayGoodScreen(){
  delay(10);

  sprintf(strTemperatureDuFour, "%g", temperatureDuFour);

  if(isEqualString(etatFour.c_str(), string("HEAT"))) myOled->updateCurrentView(myOledViewWorkingHEAT);

  if(isEqualString(etatFour.c_str(), string("OFF"))) {
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
    digitalWrite(GPIO_PIN_LED_OK_VERT, HIGH);
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, LOW);
    myOledViewWorkingOFF = new MyOledViewWorkingOFF();
    myOledViewWorkingOFF->setParams("nomDuSysteme", nomSystem);
    myOledViewWorkingOFF->setParams("idDuSysteme", idDuSysteme);
    myOledViewWorkingOFF->setParams("temperature", strTemperatureDuFour);
    myOledViewWorkingOFF->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
    myOled->displayView(myOledViewWorkingOFF);
    currentTemperatureDisplayed = temperatureDuFour;
  }

  if(isEqualString(etatFour.c_str(), string("COLD"))) {
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, LOW);
    digitalWrite(GPIO_PIN_LED_OK_VERT, LOW);
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, HIGH);
    myOledViewWorkingCOLD = new MyOledViewWorkingCOLD();
    myOledViewWorkingCOLD->setParams("nomDuSysteme", nomSystem);
    myOledViewWorkingCOLD->setParams("idDuSysteme", idDuSysteme);
    myOledViewWorkingCOLD->setParams("temperature", strTemperatureDuFour);
    myOledViewWorkingCOLD->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
    myOled->displayView(myOledViewWorkingCOLD);
    currentTemperatureDisplayed = temperatureDuFour;

  }

  if(isEqualString(etatFour.c_str(), string("HEAT"))) {
    digitalWrite(GPIO_PIN_LED_LOCK_ROUGE, HIGH);
    digitalWrite(GPIO_PIN_LED_OK_VERT, LOW);
    digitalWrite(GPIO_PIN_LED_HEAT_JAUNE, LOW);
    myOledViewWorkingHEAT = new MyOledViewWorkingHEAT();
    myOledViewWorkingHEAT->setParams("nomDuSysteme", nomSystem);
    myOledViewWorkingHEAT->setParams("idDuSysteme", idDuSysteme);
    myOledViewWorkingHEAT->setParams("temperature", strTemperatureDuFour);
    myOledViewWorkingHEAT->setParams("ipDuSysteme",WiFi.localIP().toString().c_str());
    myOled->displayView(myOledViewWorkingHEAT);
    currentTemperatureDisplayed = temperatureDuFour;
  }
}

void loop() {
  int i;

  temperatureDuFour = temperatureStub->getTemperature();
  Serial.print("Température : ");
  Serial.println(temperatureDuFour);
  delay(2000);

  //Gestion du bouton Démarrer
  // int buttonDemarrer = buttonDemarrer->checkMyButton();
  // if(buttonDemarrer > 2)  {
  //   if(temperatureDuFour < temperatureActuelleDuFour)
  //   {
  //     Serial.println("Le four doit chauffer");
  //     etatFour = "COLD";
  //   }
  //   if(temperatureDuFour >= temperatureActuelleDuFour)
  //   {
  //     Serial.println("Le four est en train de sécher le bois");
  //     etatFour = "HEAT";
  //   }
  //   else{
  //     etatFour = "OFF";
  //   }
  // }
  
  if(temperatureDuFour < temperatureActuelleDuFour){
    etatFour = "COLD";
  } 
  if(temperatureDuFour >= temperatureActuelleDuFour){
    etatFour = "HEAT";
  }
  else{
    etatFour = "OFF";
  }

  displayGoodScreen();
}