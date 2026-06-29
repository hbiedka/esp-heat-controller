#include <stdlib.h>

#include "util/interval.h"
#include "main.h"

DebugObjectModelWatcher debugWatcher;

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

  pcf_int->Spin(ts);
  pcf.Spin(ts);
  panel.Spin(ts);

  combined->Spin(ts);

  pump1.Spin(ts);
  pump2.Spin(ts);
  htr.Spin(ts);

  eeprom.Spin(ts);

  wifi.Spin(ts);
  httpServer.Spin(ts);

}

