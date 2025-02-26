#ifndef HEATER_H
#define HEATER_H

#include <Arduino.h>

enum HeaterState {
    OFF = 0,
    DELAY_TO_ON,
    ON,
    DELAY_TO_OFF
};

// String HeaterStateStrs[] = {"OFF","->ON","ON","->OFF"};

struct HeaterData {
    HeaterState state;
    unsigned int timeTo;
};

class Heater {
    private:
        bool firstRun;
        uint8_t pin;
        HeaterState state = OFF;

        unsigned long state_ts = 0;
        unsigned long delay_to_on = 5000;
        unsigned long delay_to_off = 5000;
        bool *input = nullptr;
        bool last_input = false;

        void (*log_cb)(String) = nullptr;
    public:
        HeaterData h_data = {OFF,0};

        Heater(uint8_t _pin, bool *input);
        void setInput(bool *_input) { input = _input; };
        void setLogCb(void (*cb)(String)) { log_cb = cb; };
        void Spin(unsigned long ts);

        void log(String msg) { if (log_cb != nullptr) log_cb(msg); }

        unsigned int getTimeToOn(unsigned long ts);
        unsigned int getTimeToOff(unsigned long ts);
        HeaterState getState() { return state; };
        bool watch(unsigned long ts);
};


#endif