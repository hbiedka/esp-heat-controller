#ifndef HEATER_H
#define HEATER_H

#include <Arduino.h>
#include <objectModel/ObjectModel.h>

enum HeaterState {
    OFF = 0,
    DELAY_TO_ON,
    ON,
    DELAY_TO_OFF
};

struct HeaterData {
    HeaterState state;
    unsigned int timeTo;
};

class Heater;

class HeaterDelaySetter : public ObjectModelSetter {
    private:
        Heater &htr;
    public:
        HeaterDelaySetter(Heater &_htr) :
            htr(_htr)
        {};
        ObjectModelSetterReturn operator()(const std::string &label, const ObjectModelItemValue &value) override;
};


class Heater : public ObjectModel {
    private:
        bool firstRun;
        uint8_t pin;
        HeaterState state = OFF;

        unsigned long state_ts = 0;
        unsigned long delay_to_on = 5;
        unsigned long delay_to_off = 5;
        bool *input = nullptr;
        bool last_input = false;

        void (*log_cb)(String) = nullptr;
    public:
        friend class HeaterDelaySetter;
    
        HeaterData h_data = {OFF,0};

        Heater(uint8_t _pin, bool *input, std::string name);
        void setInput(bool *_input) { input = _input; };
        void setLogCb(void (*cb)(String)) { log_cb = cb; };
        void Spin(unsigned long ts);

        void log(String msg) { if (log_cb != nullptr) log_cb(msg); }

        HeaterState getState() { return state; };

        HeaterDelaySetter delaySetterFunctor;
};


#endif