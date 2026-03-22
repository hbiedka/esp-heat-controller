#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>

enum BtnState {
    RELEASED = 0,
    WAIT_FOR_PRESS,
    PRESSED,
    HOLD,
    UNDEFINED   //used by getPanelState to indicate that button with given id was not found
};

class ButtonInteractive
{
public:
    virtual ~ButtonInteractive() = default;
    virtual void ButtonCallback(uint8_t id, bool hold) = 0;
};

class RLadderButton {
    private:
        uint8_t id = 0;
        unsigned int *state = nullptr;
        unsigned int thr_low = 0;
        unsigned int  thr_high = 0;

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

    protected:
        void CommitState(bool s);
    public:
        BtnState btn_state = RELEASED;

        uint8_t getId() { return id; };

        RLadderButton(uint8_t _id,unsigned int *_state, unsigned int _thr_low, unsigned int _thr_high) : 
            id(_id),
            state(_state),
            thr_low(_thr_low),
            thr_high(_thr_high)
        {};

        //TODO destructor / copy constructor /asignment op?

        bool GetState();
        void Spin(unsigned long ts);

        void setPressCb(void (*cb)(uint8_t));
        void setHoldCb(void (*cb)(uint8_t));
        void setCb(void (*cb)(uint8_t,bool));
        void assignInteractiveObject(ButtonInteractive* obj);
};

#endif