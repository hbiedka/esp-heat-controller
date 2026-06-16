#include "analogExternalInput.h"

AnalogExternalInput::AnalogExternalInput(uint8_t _pin) : pin(_pin) {
}

void AnalogExternalInput::Spin(unsigned long ts) {
    //we cannot read analog input too often, because it distorts WiFi
    if (ts - lastTs < 50) return;

    lastTs = ts;
    state = analogRead(pin);
}