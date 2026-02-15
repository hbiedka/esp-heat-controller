#ifndef UI_H
#define UI_H

#include <Arduino.h>
#include "display/oled.h"
#include "display/oledUi.h"

class Ui {
    private:
        std::vector<UiBlock> blocks;
        Oled *oled = nullptr;

    public:
        Ui(Oled *_oled, const std::vector<UiTemplateEntry> &entries);
        Ui(Oled *_oled);
        Ui(const std::vector<UiTemplateEntry> &entries);
        Ui();
        void loadTemplate(const std::vector<UiTemplateEntry> &entries);
        void Clear();
        UiBlock &operator[](size_t i) { return blocks[i]; };
        unsigned int getBlocksNum() { return blocks.size(); };
};

#endif