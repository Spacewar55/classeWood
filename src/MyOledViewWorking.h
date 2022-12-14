/**
    Class MyOledViewWorking : Vue utilisée par MyOled (Affiche la page montrant le système en opération)
    @file MyOledViewWorking.h 
    @author Alain Dubé
    @version 1.1 21/09/20 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      21/09/20   Ald         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
    Exemple d'utilisation 
        #include "MyOledViewWorking.h"
        MyOledViewWorking *myOledViewWorking = NULL;
        myOledViewWorking = new MyOledViewWorking();
        myOledViewWorking->setNomDuSysteme(nomDuSysteme.c_str());
        myOledViewWorking->setIpDuSysteme(WiFi.localIP().toString().c_str());
        myOledViewWorking->setStatusDuSysteme("System OK");
        myOled->displayView(myOledViewWorking);
**/
#ifndef MYOLEDVIEWWORKING_H
#define MYOLEDVIEWWORKING_H
#include "MyOledView.h"
#include <string>

const unsigned char Fire24x24_1 [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x00, 0x00, 0xE0, 0x00,
0x81, 0xF0, 0x01, 0xC3, 0xF0, 0x01, 0xC7, 0xF8, 0x03, 0xFF, 0xF8, 0x03, 0xFF, 0xF8, 0xE7, 0xFF,
0xF8, 0xE7, 0xFF, 0xFF, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x9F, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFE, 0x1F,
0xFF, 0xF8, 0x1F, 0xFF, 0xE0, 0x1F, 0xFF, 0xE0, 


};
const unsigned char Fire24x24_2 [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x80, 0x00, 0x03, 0xC0, 0x01, 0x8F, 0xE0, 0x03, 0x9F, 0xE0, 0x07, 0xFF, 0xF0, 0x07, 0xFF,
0xF3, 0x87, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xDF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x9F, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFE, 0x1F,
0xFF, 0xFC, 0x1F, 0xFF, 0xE0, 0x1F, 0xFF, 0xE0, 

};
const unsigned char Fire24x24_3 [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x02, 0x04, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x8F, 0x00, 0x01, 0x9F, 0xC0, 0x03, 0xFF,
0xE2, 0x83, 0xFF, 0xFF, 0x87, 0xFF, 0xFF, 0xC7, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x9F, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFE, 0x1F,
0xFF, 0xF8, 0x1F, 0xFF, 0xE0, 0x1F, 0xFF, 0xE0, 
};
const unsigned char Fire24x24_4 [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x9F, 0x80, 0x01, 0xDF,
0xC0, 0x83, 0xFF, 0xF3, 0xE3, 0xFF, 0xFF, 0xE7, 0xFF, 0xFF, 0xEF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0x9F, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFE, 0x1F,
0xFF, 0xF8, 0x1F, 0xFF, 0xE0, 0x1F, 0xFF, 0xE0, 
};
const unsigned char Fire24x24_5 [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x07, 0x80, 0x00, 0x0F, 0xC0, 0x01, 0x8F,
0xE0, 0x83, 0xDF, 0xF0, 0x83, 0xFF, 0xFB, 0xE7, 0xFF, 0xFF, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE7, 0xFF, 0xFF, 0xE7, 0xFF, 0xFF, 0xE3, 0xFF, 0xFF, 0xE0,
0xFF, 0xFF, 0xE0, 0x1F, 0xFF, 0x00, 0x1F, 0xFF, 
};
const unsigned char Fire24x24_6 [] PROGMEM = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x00, 0x01, 0xE0, 0x00, 0x83,
0xE0, 0x00, 0xCF, 0xF0, 0x81, 0xDF, 0xF8, 0xC3, 0xFF, 0xF8, 0xE7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
0xFF, 0xFF, 0xFF, 0x9F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFF, 0x1F, 0xFF, 0xFE, 0x1F,
0xFF, 0xFC, 0x1F, 0xFF, 0xF0, 0x1F, 0xFF, 0xF0, 

};

class MyOledViewWorking: public MyOledView {
    public:
        void init(std::string _id);
    
    protected:
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        unsigned char *Fire24x24Pointers[6];

        int indexFire = 1;
        int indexFireDelay = 1;

        void displayGifFire(Adafruit_SSD1306 *adafruitint, int positionX, int positionY);
        void displayGifFireAnimated(Adafruit_SSD1306 *adafruit, int positionX, int positionY);       
};
#endif