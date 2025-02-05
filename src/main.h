#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include "button/button.h"
#include "button/buttonGroup.h"
#define PIN_LED_STATUS 2 //built-in led


#define PIN_PUMP1 15
#define PIN_PUMP2 13
#define PIN_HEATER 12

void selftest();
uint8_t read_buttons();
void send_btn_press(uint8_t btn_id);

//init buttons
uint16_t resistor_ladder_out = 0;
// RLadderButton b1(1,&resistor_ladder_out,100,150);
// RLadderButton b2(2,&resistor_ladder_out,350,400);
// RLadderButton b3(3,&resistor_ladder_out,550,600);
ButtonGroup panel({
    RLadderButton(1,&resistor_ladder_out,100,150),
    RLadderButton(2,&resistor_ladder_out,350,400),
    RLadderButton(3,&resistor_ladder_out,550,600)
});


#endif 