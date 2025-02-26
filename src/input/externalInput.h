#include <Arduino.h>

//TODO add parent class Input
class ExternalInput {
    private:
        uint8_t pin;
        bool state = false;
    public:
        ExternalInput(uint8_t _pin) : pin(_pin) {
            pinMode(pin,INPUT_PULLUP);
        }
        void Spin() {
            digitalWrite(pin,1);
            state = digitalRead(pin) == 0;
        }
        bool *getStatePtr() { return &state; }
};