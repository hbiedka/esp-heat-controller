#ifndef PCF8574_H
#define PCF8574_H

#include <Arduino.h>
#include <Wire.h>
#include <memory>
#include <variant>

#include "input/input.h"
#include "button/button.h"

enum class PCF8574State {
    INIT,
    WAIT_INT_DOWN,
    SEND_RQ,
    WAIT_INT_UP
};

class PCF8574Input : public Input {
    private:
        void SetState(bool s) { state = s; };
    public:
        friend class PCF8574;
        void Spin(unsigned long ts) override {};
};

class PCF8574Button : public Button {
    private:
        bool state;
        void SetState(bool s) { state = s; };
    public:
        friend class PCF8574;
        PCF8574Button(uint8_t id) : Button(id), state(false) {}
        bool GetState() override { return state; };
};

class PCF8574Unused {
    //dummy class
};

using PCF8574IO = std::variant<
    std::shared_ptr<PCF8574Unused>,
    std::shared_ptr<PCF8574Input>,
    std::shared_ptr<PCF8574Button>
>;

class PCF8574 {
    private:
        uint8_t address;
        std::shared_ptr<Input> intPin;

        PCF8574State state = PCF8574State::INIT;
        unsigned long lastTs;

        void UpdateInputs(uint8_t data);

    public:
        std::vector<PCF8574IO> ios;
        PCF8574(uint8_t _address, std::shared_ptr<Input> _intPin);
        void Spin(unsigned long ts);

        std::shared_ptr<PCF8574Button> RegisterButton(size_t ioNum, uint8_t buttonId);
        std::shared_ptr<PCF8574Input>RegisterInput(size_t ioNum);
        void ShowIO();
};

#endif