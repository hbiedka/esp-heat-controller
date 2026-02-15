#include "externalInput.h"

ExternalInput::ExternalInput(uint8_t _pin) : pin(_pin) {
    pinMode(pin,INPUT_PULLUP);
}

void ExternalInput::Spin() {
    digitalWrite(pin,1);
    state = digitalRead(pin) == 0;
}