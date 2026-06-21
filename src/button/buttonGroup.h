#ifndef BUTTON_GROUP_H
#define BUTTON_GROUP_H

#include "button.h"
#include <vector>
#include <memory>

class ButtonGroup {
    private:
        std::vector<std::shared_ptr<Button>> buttons;
    public:
        ButtonGroup(std::vector<std::shared_ptr<Button>> _buttons) :
            buttons(_buttons)
        {}
        void Spin(unsigned long ts);
        void setCallbackForId(uint8_t id, void (*cb)(uint8_t));
        void setCallbackForAll(void (*cb)(uint8_t));
        void setHoldCallbackForId(uint8_t id, void (*cb)(uint8_t));
        void setHoldCallbackForAll(void (*cb)(uint8_t));
        void setUnifiedCallbackForId(uint8_t id, void (*cb)(uint8_t,bool));
        void setUnifiedCallbackForAll(void (*cb)(uint8_t,bool));
        void assignInteractiveObject(ButtonInteractive* obj);

        std::optional<std::shared_ptr<Button>> getButton(uint8_t id) {
            for (auto &b : buttons) {
                if (b->getId() == id) {
                    return b;
                }
            }
            return std::nullopt;
        }

        bool isPressed(uint8_t id) {
            auto maybeB = getButton(id);
            if (!maybeB) return false;
            auto b = *maybeB;

            return b->btn_state == PRESSED || b->btn_state == HOLD;
        }
};

#endif