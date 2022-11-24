/**
    Class MyOledViewInitialisation : Gestion d'une VUE pour le OLed
    @file MyOledViewInitialisation.h 
    @author Alex De Souza
    @version 1.1 20/11/22 
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      20/11/22    Ald        Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
**/

#ifndef MYOLEDVIEWINITIALISATION_H
#define MYOLEDVIEWINITIALISATION_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"
#include <string>
#include <vector>
using std::vector;

class MyOledViewInitialisation: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);
        void setIdDuSysteme(std::string val);
        void setSensibiliteBoutonAction(std::string val);
        void setSensibiliteBoutonReset(std::string val);

    private:
        std::string nomDuSysteme;
        std::string idDuSysteme;
        std::string SensibiliteBoutonAction;
        std::string SensibiliteBoutonReset;

        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
};
#endif