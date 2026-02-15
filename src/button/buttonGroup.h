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
        void setHoldCallbackForId(uint8_t id, void (*cb)(uint8_t));
        void setHoldCallbackForAll(void (*cb)(uint8_t));
        void setUnifiedCallbackForId(uint8_t id, void (*cb)(uint8_t,bool));
        void setUnifiedCallbackForAll(void (*cb)(uint8_t,bool));
        void assignInteractiveObject(ButtonInteractive* obj);

        RLadderButton *getButton(uint8_t id) {
            for (auto &b : buttons) {
                if (b.getId() == id) {
                    return &b;
                }
            }
            return nullptr;
        }

        BtnState getPanelState(uint8_t id) { 
            RLadderButton *b = getButton(id);
            if (b == nullptr) return UNDEFINED;
            return b->btn_state;
        }

        bool isPressed(uint8_t id) {
            RLadderButton *b = getButton(id);
            if (b == nullptr) return false;
            return b->btn_state == PRESSED || b->btn_state == HOLD;
        }
};

#endif