#include "analogExternalInput.h"

AnalogExternalInput::AnalogExternalInput(uint8_t _pin) : pin(_pin) {
}

void AnalogExternalInput::Spin() {
    state = analogRead(pin);
}