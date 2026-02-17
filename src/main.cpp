#include <stdlib.h>

#include "util/interval.h"
#include "main.h"

const char degC_cstr[] = {127,'C','\0'};
std::string degC{degC_cstr};

DebugObjectModelWatcher serialP1Watcher("p1");
DebugObjectModelWatcher serialP2Watcher("p2");
DebugObjectModelWatcher serialHtrWatcher("htr");

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

  pump1.AddWatcher("state",&serialP1Watcher);
  pump2.AddWatcher("state",&serialP2Watcher);
  htr.AddWatcher("state",&serialHtrWatcher);
  pump1.AddWatcher("timeToNextState",&serialP1Watcher);
  pump2.AddWatcher("timeToNextState",&serialP2Watcher);
  htr.AddWatcher("timeToNextState",&serialHtrWatcher);

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

