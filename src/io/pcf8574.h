#ifndef PCF8574_H
#define PCF8574_H

#include <Arduino.h>
#include <Wire.h>
#include <memory>

#include "input/input.h"

enum class PCF8574State {
    INIT,
    WAIT_INT_DOWN,
    SEND_RQ,
    WAIT_INT_UP
};

class PCF8574Input : public Input {
    public:
        friend class PCF8574;
        void Spin(unsigned long ts) override {};
};

class PCF8574 {
    private:
        uint8_t address;
        std::shared_ptr<Input> intPin;


        PCF8574State state = PCF8574State::INIT;
        unsigned long lastTs;

        void updateInputs(uint8_t data);

    public:
        std::vector<PCF8574Input> inputs;
        PCF8574(uint8_t _address, std::shared_ptr<Input> _intPin);
        void Spin(unsigned long ts);
};

#endif