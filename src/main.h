#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "button/button.h"
#include "button/buttonGroup.h"
#include "util/selftest.h"
#include "input/externalInput.h"
#include "input/analogExternalInput.h"
#include "input/logicInput.h"
#include "heat/heater.h"
#include "display/oled.h"
#include "display/oledUi.h"
#include "adapter/ObjectModelToMenu.h"
#include "network/Wifi.h"
#include "objectModel/ObjectModel.h"
#include "objectModel/ObjectModelWatcher.h"
#include "nvmem/nvmem.h"

#include "ui/ui.h"
#include "ui/mainScreen.h"
#include "ui/menu.h"


#define OLED_ADDR 0x3c

#define PIN_LED_STATUS 2 //built-in led

#define PIN_PUMP1 15
#define PIN_PUMP2 13
#define PIN_HEATER 16

#define PIN_IN1 12
#define PIN_IN2 14

#define PIN_ADC_RESISTOR_LADDER 0

//init buttons
// uint16_t resistor_ladder_out = 0;
AnalogExternalInput resistorLadderButtons(PIN_ADC_RESISTOR_LADDER);

ButtonGroup panel({
    RLadderButton(1,resistorLadderButtons.getStatePtr(),100,150),
    RLadderButton(2,resistorLadderButtons.getStatePtr(),350,400),
    RLadderButton(3,resistorLadderButtons.getStatePtr(),550,600)
});

std::vector<uint8_t> outputs{PIN_PUMP1,PIN_PUMP2,PIN_HEATER};

ExternalInput in1(PIN_IN1);
ExternalInput in2(PIN_IN2);
LogicInput combined(LogicInputOperation::OR,{&in1,&in2});

Heater pump1(PIN_PUMP1,in1.getStatePtr(),"Pump 1");
Heater pump2(PIN_PUMP2,in2.getStatePtr(),"Pump 2");
Heater htr(PIN_HEATER,combined.getStatePtr(),"Heater");

std::vector<ObjectModel*> pumps{{&pump1,&pump2}};
ObjectModelList pump_list{pumps};
ObjectModel omRoot{
    {
        {"pumps", ObjectModelItem{&pump_list}},
        {"heater", ObjectModelItem{&htr}},
    }
};


NVMem eeprom{omRoot, 512};

Selftest selftest(outputs,&panel);

Oled oled(OLED_ADDR);
Ui ui(&oled,&panel);

auto pump1ton = std::make_shared<IntObjectModelIface>(&pump1,"delayToOn",0,100);
auto pump2ton = std::make_shared<IntObjectModelIface>(&pump2,"delayToOn",0,100);
auto htrton = std::make_shared<IntObjectModelIface>(&htr,"delayToOn",0,100);
auto pump1toff = std::make_shared<IntObjectModelIface>(&pump1,"delayToOff",0,100);
auto pump2toff = std::make_shared<IntObjectModelIface>(&pump2,"delayToOff",0,100);
auto htrtoff = std::make_shared<IntObjectModelIface>(&htr,"delayToOff",0,100);

void saveToEeprom();

std::vector<MenuItem> m1Items{
    MenuItem{"P1 delay on", INT, pump1ton, "%d s"},
    MenuItem{"P1 delay off", INT, pump1toff, "%d s"},
    MenuItem{"Back",BACK},
};
Menu m1(ui,m1Items);

std::vector<MenuItem> m2Items{
    MenuItem{"P2 delay on", INT, pump2ton, "%d s"},
    MenuItem{"P2 delay off", INT, pump2toff, "%d s"},
    MenuItem{"Back",BACK},
};
Menu m2(ui, m2Items);

std::vector<MenuItem> m3Items{
    MenuItem{"Htr delay on", INT, htrton, "%d s"},
    MenuItem{"Htr delay off", INT, htrtoff, "%d s"},
    MenuItem{"Back",BACK},
};
Menu m3(ui, m3Items);

std::vector<MenuItem> mainMenuItems{
    MenuItem{"Pump 1", LINK, &m1},
    MenuItem{"Pump 2", LINK, &m2},
    MenuItem{"Heater", LINK, &m3},
    MenuItem{"Back to main",BACK},
};
Menu mainMenu(ui,mainMenuItems, saveToEeprom);

MainScreen mainScreen(ui,&mainMenu,omRoot);

#endif 