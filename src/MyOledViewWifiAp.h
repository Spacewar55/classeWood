/**
    Class MyOledViewWifiAp : Gestion d'une VUE pour le OLed
    @file MyButton.h 
    @author Alex De Souza
    @version 1.1 17/11/22 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      17/11/22    Ald        Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
**/


#ifndef MYOLEDVIEWWIFIAP_H
#define MYOLEDVIEWWIFIAP_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"
#include <string>
#include <vector>
using std::vector;

class MyOledViewWifiAp: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);
        void setSsIDDuSysteme(std::string val);
        void setPassDuSysteme(std::string val);

    private:
        std::string nomDuSysteme;
        std::string ssIDDuSysteme;
        std::string passDuSysteme;

        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
      
};
#endif