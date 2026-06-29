#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "button/rLadderButton.h"
#include "button/buttonGroup.h"
#include "util/selftest.h"
#include "input/externalInput.h"
#include "input/analogExternalInput.h"
#include "input/logicInput.h"
#include "io/pcf8574.h"
#include "heat/heater.h"
#include "display/oled.h"
#include "display/oledUi.h"
#include "adapter/ObjectModelToMenu.h"
#include "network/Wifi.h"
#include "network/HttpServer.h"
#include "objectModel/ObjectModel.h"
#include "objectModel/ObjectModelWatcher.h"
#include "nvmem/nvmem.h"

#include "ui/ui.h"
#include "ui/mainScreen.h"
#include "ui/menu.h"


#define OLED_ADDR 0x3c
#define PCF_ADDR 0x3f

#define PIN_LED_STATUS 2 //built-in led

#define PIN_PUMP1 13
#define PIN_PUMP2 15
#define PIN_HEATER 16

#define PIN_PCF_INT 12

#define PCF_PIN_IN1 7
#define PCF_PIN_IN2 6
#define PCF_PIN_BTN1 1
#define PCF_PIN_BTN2 0
#define PCF_PIN_BTN3 4

auto pcf_int = std::make_shared<ExternalInput>(PIN_PCF_INT);
PCF8574 pcf(PCF_ADDR,pcf_int);

ButtonGroup panel({
    pcf.RegisterButton(PCF_PIN_BTN1,1),
    pcf.RegisterButton(PCF_PIN_BTN2,2),
    pcf.RegisterButton(PCF_PIN_BTN3,3),
});

std::vector<uint8_t> outputs{PIN_PUMP1,PIN_PUMP2,PIN_HEATER};

auto in1 = pcf.RegisterInput(PCF_PIN_IN1);
auto in2 = pcf.RegisterInput(PCF_PIN_IN2);
auto combined = std::make_shared<LogicInput>(LogicInputOperation::OR,
    std::vector<std::shared_ptr<Input>>{in1,in2});

Heater pump1(PIN_PUMP1,in1,"Pump 1");
Heater pump2(PIN_PUMP2,in2,"Pump 2");
Heater htr(PIN_HEATER,combined,"Heater");
WifiEngine wifi;

std::vector<ObjectModel*> pumps{{&pump1,&pump2}};
ObjectModelList pump_list{pumps};
ObjectModel omRoot{
    {
        {"pumps", ObjectModelItem{&pump_list}},
        {"heater", ObjectModelItem{&htr}},
        {"wifi", ObjectModelItem{&wifi}}
    }
};


NVMem eeprom{omRoot, 512};

HttpServer httpServer(omRoot);

Selftest selftest(outputs,&panel);

Oled oled(OLED_ADDR);
Ui ui(&oled,&panel);

auto pump1ton = std::make_shared<IntObjectModelIface>(&pump1,"delayToOn",0,100);
auto pump2ton = std::make_shared<IntObjectModelIface>(&pump2,"delayToOn",0,100);
auto htrton = std::make_shared<IntObjectModelIface>(&htr,"delayToOn",0,100);
auto pump1toff = std::make_shared<IntObjectModelIface>(&pump1,"delayToOff",0,100);
auto pump2toff = std::make_shared<IntObjectModelIface>(&pump2,"delayToOff",0,100);
auto htrtoff = std::make_shared<IntObjectModelIface>(&htr,"delayToOff",0,100);

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
Menu mainMenu(ui,mainMenuItems);

MainScreen mainScreen(ui,&mainMenu,omRoot);

#endif 