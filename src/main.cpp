#include <stdlib.h>

#include "main.h"

const char degC_cstr[] = {127,'C','\0'};
String degC{degC_cstr};

void setup() {

  pinMode(PIN_LED_STATUS,OUTPUT);
  digitalWrite(PIN_LED_STATUS,0);

  Serial.begin(9600);
  pump1.setLogCb([](String msg) { Serial.println(msg); });

  oled.Init();

  delay(1000);

  oled.Clear();

  menu[0].tfList[0].Print("Pump1");
  menu[0].tfList[1].Print("?");
}

void loop() {
  unsigned long ts = millis();

  resistor_ladder_out = analogRead(0);
  panel.Spin(ts);

  in1.Spin();
  in2.Spin();

  pump1.Spin(ts);
  pump2.Spin(ts);

  for (size_t i = 0; i < pumps.size(); i++) {
    if (pumps[i]->watch(ts)) {
      String s;
      if (pumps[i]->h_data.state == DELAY_TO_OFF || pumps[i]->h_data.state == DELAY_TO_ON) {
        s+= String{pumps[i]->h_data.timeTo};
        s+= "->";
      }
      s+= pumps[i]->h_data.state == ON || pumps[i]->h_data.state == DELAY_TO_ON ? "ON" : "OFF";

      menu[0].tfList[1].Print(s);


    }
  }

}

