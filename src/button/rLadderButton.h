#ifndef R_LADDER_BUTTON_H
#define R_LADDER_BUTTON_H

#include <memory>
#include "button.h"
#include "input/input.h"

class RLadderButton : public Button
{
    private:
        std::weak_ptr<AnalogInput> input;
        unsigned int thr_low = 0;
        unsigned int  thr_high = 0;

    public:
        RLadderButton(uint8_t _id, std::weak_ptr<AnalogInput>_input, unsigned int _thr_low, unsigned int _thr_high) :
            Button(_id),
            input(_input),
            thr_low(_thr_low),
            thr_high(_thr_high)
        {};

        bool GetState() override;

};

#endif