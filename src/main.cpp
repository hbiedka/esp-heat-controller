#include <stdlib.h>

#include "util/interval.h"
#include "main.h"

DebugObjectModelWatcher debugWatcher;

void saveToEeprom()
{
  eeprom.save();
  Serial.println("Saving to eeprom");
}

void setup() {

  pinMode(PIN_LED_STATUS,OUTPUT);
  digitalWrite(PIN_LED_STATUS,0);

  Serial.begin(9600);

  pump1.AddWatcher("state","pump1/state",&debugWatcher);
  pump2.AddWatcher("state","pump2/state",&debugWatcher);
  htr.AddWatcher("state","htr/state",&debugWatcher);
  pump1.AddWatcher("timeToNextState","pump1/timeToNextState",&debugWatcher);
  pump2.AddWatcher("timeToNextState","pump2/timeToNextState",&debugWatcher);
  htr.AddWatcher("timeToNextState","htr/timeToNextState",&debugWatcher);
  wifi.AddWatcher("state","wifi/state",&debugWatcher);

  oled.Init();
  mainScreen.Show();

  eeprom.slowInitAndLoad();
}

void loop() {
  unsigned long ts = millis();

  resistorLadderButtons.Spin(ts);
  panel.Spin(ts);

  in1.Spin(ts);
  in2.Spin(ts);
  combined.Spin(ts);

  pump1.Spin(ts);
  pump2.Spin(ts);
  htr.Spin(ts);

  wifi.Spin(ts);
  httpServer.Spin(ts);

}

