#include <stdlib.h>

#include "main.h"
#include "button/button.h"

#define PIN_PUMP1 15
#define PIN_PUMP2 13
#define PIN_HEATER 12

#define PIN_LED_STATUS 2 //built-in led

void setup() {

  pinMode(PIN_PUMP1,OUTPUT);
  pinMode(PIN_PUMP2,OUTPUT);
  pinMode(PIN_HEATER,OUTPUT);
  pinMode(PIN_LED_STATUS,OUTPUT);

  digitalWrite(PIN_PUMP1,0);
  digitalWrite(PIN_PUMP2,0);
  digitalWrite(PIN_HEATER,0);
  digitalWrite(PIN_LED_STATUS,0);

  panel.setCallbackForAll(send_btn_press);

  Serial.begin(9600);
}

void loop() {
  selftest();

}


void selftest() {
  std::vector<uint8_t> outputs{PIN_PUMP1,PIN_PUMP2,PIN_HEATER};

  uint8_t b;
  auto outputs_iter = outputs.begin();
  unsigned long ts = 0;

  for(;;) {
    b = read_buttons();
    // Serial.println(resistor_ladder_out);

    if (b != 1 && ts < millis()) {
      ts = millis() + (b == 3 ? 200 : 400);

      digitalWrite(*outputs_iter, b==2);
      outputs_iter++;
      if (outputs_iter == outputs.end()) outputs_iter = outputs.begin();
      digitalWrite(*outputs_iter, b!=2);

    }

    digitalWrite(PIN_LED_STATUS,1);
    delay(10);
    digitalWrite(PIN_LED_STATUS,0);
  }

}

uint8_t read_buttons() {

  resistor_ladder_out = analogRead(0);
  unsigned long ts = millis();
  panel.Spin(ts);

  if (panel.getPanelState(1) == PRESSED) return 1;
  if (panel.getPanelState(2) == PRESSED) return 2;
  if (panel.getPanelState(3) == PRESSED) return 3;
  
  return 0;
}

void send_btn_press(uint8_t btn_id) {
  Serial.printf("Button %d pressed\n", btn_id);
}
