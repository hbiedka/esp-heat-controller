#include <stdlib.h>

#include "main.h"

void setup() {

  pinMode(PIN_PUMP1,OUTPUT);
  pinMode(PIN_PUMP2,OUTPUT);
  pinMode(PIN_HEATER,OUTPUT);
  pinMode(PIN_LED_STATUS,OUTPUT);

  digitalWrite(PIN_PUMP1,0);
  digitalWrite(PIN_PUMP2,0);
  digitalWrite(PIN_HEATER,0);
  digitalWrite(PIN_LED_STATUS,0);

  Serial.begin(9600);
}

void loop() {
  unsigned long ts = millis();

  resistor_ladder_out = analogRead(0);
  panel.Spin(ts);

  //self-test mode
  selftest.Spin();

}

