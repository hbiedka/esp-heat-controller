#ifndef BUTTON_GROUP_H
#define BUTTON_GROUP_H

#include "button.h"
#include <vector>

class ButtonGroup {
    private:
        std::vector<RLadderButton> buttons;
    public:
        ButtonGroup(std::vector<RLadderButton> _buttons) : buttons(_buttons) {}
        void Spin(unsigned long ts);
        void setCallbackForId(uint8_t id, void (*cb)(uint8_t));
        void setCallbackForAll(void (*cb)(uint8_t));

        BtnState getPanelState(uint8_t id) {
            for (auto &b : buttons) {
                if (b.getId() == id) {
                    return b.btn_state;
                }
            }
            return UNDEFINED;
        }
};

#endif