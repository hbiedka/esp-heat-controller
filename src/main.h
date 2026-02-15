#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "button/button.h"
#include "button/buttonGroup.h"
#include "util/selftest.h"
#include "input/externalInput.h"
#include "heat/heater.h"
#include "display/oled.h"
#include "display/oledUi.h"
#include "adapter/ObjectModelToMenu.h"

#include "ui/ui.h"
#include "ui/menu.h"


#define OLED_ADDR 0x3c

#define PIN_LED_STATUS 2 //built-in led

#define PIN_PUMP1 15
#define PIN_PUMP2 13
#define PIN_HEATER 16

#define PIN_IN1 12
#define PIN_IN2 14

//init buttons
uint16_t resistor_ladder_out = 0;

ButtonGroup panel({
    RLadderButton(1,&resistor_ladder_out,100,150),
    RLadderButton(2,&resistor_ladder_out,350,400),
    RLadderButton(3,&resistor_ladder_out,550,600)
});

std::vector<uint8_t> outputs{PIN_PUMP1,PIN_PUMP2,PIN_HEATER};

ExternalInput in1(PIN_IN1);
ExternalInput in2(PIN_IN2);
Heater pump1(PIN_PUMP1,in1.getStatePtr());
Heater pump2(PIN_PUMP2,in2.getStatePtr());

std::vector<ObjectModel*> pumps{{&pump1,&pump2}};
ObjectModelList pump_list{pumps};

Selftest selftest(outputs,&panel);

Oled oled(OLED_ADDR);
Ui ui(&oled);

std::vector<std::string> boolLabels{"Yes","No"};
std::vector<std::string> enumLabels{"Alpha","Bravo","Charlie","Delta"};
std::vector<std::string> dummyLabels;



auto pump1ton = std::make_shared<IntObjectModelIface>(&pump1,"delayToOn",0,100);
auto pump2ton = std::make_shared<IntObjectModelIface>(&pump2,"delayToOn",0,100);
auto pump1toff = std::make_shared<IntObjectModelIface>(&pump1,"delayToOff",0,100);
auto pump2toff = std::make_shared<IntObjectModelIface>(&pump2,"delayToOff",0,100);


std::vector<MenuItem> m1Items{
    MenuItem{"H1 delay on", INT, dummyLabels, pump1ton, "%d s"},
    MenuItem{"H1 delay off", INT, dummyLabels, pump1toff, "%d s"},
    MenuItem{"Back",BACK, dummyLabels, std::make_shared<MenuValueIface>()},
};
Menu m1(m1Items,&panel,ui);

std::vector<MenuItem> m2Items{
    MenuItem{"H2 delay on", INT, dummyLabels, pump2ton, "%d s"},
    MenuItem{"H2 delay off", INT, dummyLabels, pump2toff, "%d s"},
    MenuItem{"Back",BACK, dummyLabels, std::make_shared<MenuValueIface>()},
};
Menu m2(m2Items,&panel,ui);

std::vector<MenuItem> mainMenuItems{
    MenuItem{"Heater 1", LINK, dummyLabels, std::make_shared<LinkMenuValueIface>(&m1)},
    MenuItem{"Heater 2", LINK, dummyLabels, std::make_shared<LinkMenuValueIface>(&m2)},
};
Menu mainMenu(mainMenuItems,&panel,ui);
#endif 