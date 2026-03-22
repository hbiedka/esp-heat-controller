#ifndef ANALOG_INPUT_H
#define ANALOG_INPUT_H

#include "input.h"
#include <Arduino.h>

class AnalogExternalInput : public AnalogInput {
    private:
        uint8_t pin;
    public:
        AnalogExternalInput(uint8_t _pin);
        void Spin();
};

#endif