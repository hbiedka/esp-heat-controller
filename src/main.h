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

#include "ui/menu.h"


#define OLED_ADDR 0x3c

#define PIN_LED_STATUS 2 //built-in led

#define PIN_PUMP1 15
#define PIN_PUMP2 13
#define PIN_HEATER 12

#define PIN_IN1 16
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

std::vector<Heater*> pumps{&pump1,&pump2};

Selftest selftest(outputs,&panel);

Oled oled(OLED_ADDR);

String boolLabels[2] = {"Yes","No"};

bool v1 = false;
bool v2 = true;
bool v3 = true;

std::vector<MenuItem> mItems = {
    {"Label 1",BOOL,boolLabels,{&v1},0,0},
    {"Label 2",BOOL,boolLabels,{&v2},0,0},
    {"Label 3",BOOL,boolLabels,{&v3},0,0},
    {"Label 4",BOOL,boolLabels,{&v1},0,0},
    {"Label 5",BOOL,boolLabels,{&v2},0,0},
    {"Label 6",BOOL,boolLabels,{&v3},0,0},

};

Menu m(mItems,&panel,&oled);

#endif 