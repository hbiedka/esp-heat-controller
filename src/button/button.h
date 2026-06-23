#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

enum BtnState {
    RELEASED = 0,
    WAIT_FOR_PRESS,
    PRESSED,
    HOLD
};

class ButtonInteractive
{
public:
    virtual ~ButtonInteractive() = default;
    virtual void ButtonCallback(uint8_t id, bool hold) = 0;
};

class Button {
    protected:
        uint8_t id = 0;

        unsigned int t_press = 40; //short press time
        unsigned int t_hold = 500; //hold press time
        unsigned int t_hold_decay = 75; //hold press decay time
        unsigned int t_hold_sustain = 50; //minimum hold press time

        unsigned int t_hold_buffer; //hold press buffer

        bool last_pressed = false;
        unsigned long state_ts = 0;

        // Pointer to a void function with one uint8_t argument
        void (*press_cb)(uint8_t) = nullptr;
        void (*hold_cb)(uint8_t) = nullptr;
        
        // Pointer to void fcn with uint8_t and bool arguments
        void (*unified_cb)(uint8_t,bool) = nullptr;

        // Ptr to object which proviedes button callback
        ButtonInteractive* interactiveObject = nullptr;

        void CommitState(bool s);
    public:
        BtnState btn_state = RELEASED;

        uint8_t getId() { return id; };

        Button(uint8_t _id) :
            id(_id)
        {};

        virtual ~Button() = default;

        virtual bool GetState() = 0;
        virtual void Spin(unsigned long ts);

        virtual void assignInteractiveObject(ButtonInteractive* obj);
};


#endif