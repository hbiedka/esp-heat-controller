#include <stdlib.h>

#include "util/interval.h"
#include "objectModel/ObjectModel.h"
#include "main.h"

const char degC_cstr[] = {127,'C','\0'};
String degC{degC_cstr};

IntervalOperation sendHtrStatus(3000);

void setup() {

  pinMode(PIN_LED_STATUS,OUTPUT);
  digitalWrite(PIN_LED_STATUS,0);

  Serial.begin(9600);

  oled.Init();

  delay(1000);

  m.Show();
}

void loop() {
  unsigned long ts = millis();

  resistor_ladder_out = analogRead(0);
  panel.Spin(ts);

  in1.Spin();
  in2.Spin();

  pump1.Spin(ts);
  pump2.Spin(ts);

  if (sendHtrStatus.trig()) {
    Serial.println(pump2.serialize().c_str());
  }

}

