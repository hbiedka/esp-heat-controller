#ifndef NAVIGATIVE_SCREEN_H
#define NAVIGATIVE_SCREEN_H

#include "ui.h"

class NavigativeScreen {
    private:
        NavigativeScreen *prev = nullptr;
        virtual void SavePrev(NavigativeScreen *_prev)
        {
            //save previous menu
            prev = _prev;
        };
    public:
        virtual ~NavigativeScreen() = default;
        virtual void Show() = 0;

        virtual void Jump(NavigativeScreen *target) {
            //saves calling object as previous and jumps to target

            if (target == nullptr) return; //TODO throw exception
            target->SavePrev(this);
            target->Show();
        };
        virtual void JumpBack() {
            if (prev == nullptr) return;    //TODO throw exception
            prev->Show();
        }
};

#endif