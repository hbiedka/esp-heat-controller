#include <stdlib.h>

#include "util/interval.h"
#include "objectModel/ObjectModel.h"
#include "main.h"

const char degC_cstr[] = {127,'C','\0'};
std::string degC{degC_cstr};

IntervalOperation sendHtrStatus(3000);

void setup() {

  pinMode(PIN_LED_STATUS,OUTPUT);
  digitalWrite(PIN_LED_STATUS,0);

  Serial.begin(9600);

  pump1.setProperty("delayToOn",20);
  pump1.setProperty("delayToOff",120);
  pump2.setProperty("delayToOn",20);
  pump2.setProperty("delayToOff",120);
  htr.setProperty("delayToOn",1);
  htr.setProperty("delayToOff",1);

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

  if (sendHtrStatus.trig()) {
    Serial.println(pump_list.serialize().c_str());
  }

}

