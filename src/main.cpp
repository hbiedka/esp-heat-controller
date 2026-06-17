#include <stdlib.h>

#include "util/interval.h"
#include "main.h"

DebugObjectModelWatcher serialP1Watcher("p1");
DebugObjectModelWatcher serialP2Watcher("p2");
DebugObjectModelWatcher serialHtrWatcher("htr");
DebugObjectModelWatcher WifiStatusWatcher("wifi");

void saveToEeprom()
{
  eeprom.save();
  Serial.println("Saving to eeprom");
}

void setup() {

  pinMode(PIN_LED_STATUS,OUTPUT);
  digitalWrite(PIN_LED_STATUS,0);

  Serial.begin(9600);

  pump1.AddWatcher("state",&serialP1Watcher);
  pump2.AddWatcher("state",&serialP2Watcher);
  htr.AddWatcher("state",&serialHtrWatcher);
  pump1.AddWatcher("timeToNextState",&serialP1Watcher);
  pump2.AddWatcher("timeToNextState",&serialP2Watcher);
  htr.AddWatcher("timeToNextState",&serialHtrWatcher);
  wifi.AddWatcher("state",&WifiStatusWatcher);

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

