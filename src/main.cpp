#include <stdlib.h>

#include "util/interval.h"
#include "objectModel/ObjectModel.h"
#include "main.h"

const char degC_cstr[] = {127,'C','\0'};
std::string degC{degC_cstr};

void setup() {

  pinMode(PIN_LED_STATUS,OUTPUT);
  digitalWrite(PIN_LED_STATUS,0);

  Serial.begin(9600);

  pump1.setProperty("delayToOn",10);
  pump1.setProperty("delayToOff",10);
  pump2.setProperty("delayToOn",10);
  pump2.setProperty("delayToOff",10);
  htr.setProperty("delayToOn",3);
  htr.setProperty("delayToOff",3);

  watcher.Watch(&pump1,"state","pump1/on");
  watcher.Watch(&pump2,"state","pump2/on");
  watcher.Watch(&htr,"state","heater/on");
  watcher.Watch(&pump1,"timeToNextState","pump1/timeToNextState");
  watcher.Watch(&pump2,"timeToNextState","pump2/timeToNextState");
  watcher.Watch(&htr,"timeToNextState","heater/timeToNextState");

  oled.Init();
  mainScreen.Show();
}

void loop() {
  unsigned long ts = millis();

  resistor_ladder_out = analogRead(0);
  panel.Spin(ts);

  in1.Spin();
  in2.Spin();
  combined.Spin();

  pump1.Spin(ts);
  pump2.Spin(ts);
  htr.Spin(ts);

}

