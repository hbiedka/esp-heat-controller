#ifndef OLEDUI_H
#define OLEDUI_H

#include <Arduino.h>
#include "oled.h"


class TextField {
    private:
        uint8_t x;
        uint8_t y;
        uint8_t len;
        uint8_t font;
        uint8_t offset;

        Oled *oled = nullptr;

    public:
        TextField(uint8_t _x, uint8_t _y, uint8_t _len, uint8_t _font, uint8_t _offset) :
            x(_x),
            y(_y),
            len(_len),
            font(_font),
            offset(_offset)
        {}

        TextField(const TextField &tf, uint8_t _x, uint8_t _y) :
            x(tf.x+_x),
            y(tf.y+_y),
            len(tf.len),
            font(tf.font),
            offset(tf.offset)
        {}

        void SetOled(Oled *_oled) { oled = _oled; }

        void Print(const std::string &s);
        void Clear();

};

struct UiTemplateEntry {
    const std::vector<TextField> &block;
    uint8_t x;
    uint8_t y;
};

#endif