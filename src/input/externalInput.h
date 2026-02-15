#ifndef EXTERNAL_INPUT_H
#define EXTERNAL_INPUT_H

#include "input.h"
#include <Arduino.h>

class ExternalInput : public Input {
    private:
        uint8_t pin;
    public:
        ExternalInput(uint8_t _pin);
        void Spin();
};

#endif