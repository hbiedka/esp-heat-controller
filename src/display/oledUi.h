#ifndef OLEDUI_H
#define OLEDUI_H

#include <Arduino.h>
#include "oled.h"


class TextField {
    protected:
        uint8_t x;
        uint8_t y;
        uint8_t len;
        uint8_t font;
        uint8_t offset;
        bool border = false;
        uint8_t padding = 0;

    public:
        TextField(uint8_t _x, uint8_t _y, uint8_t _len, uint8_t _font, uint8_t _offset, bool border=false, uint8_t padding=0) :
            x(_x),
            y(_y),
            len(_len),
            font(_font),
            offset(_offset),
            border(border),
            padding(padding)
        {}

        TextField(const TextField &tf, uint8_t _x, uint8_t _y) :
            x(tf.x+_x),
            y(tf.y+_y),
            len(tf.len),
            font(tf.font),
            offset(tf.offset),
            border(tf.border),
            padding(tf.padding)
        {}
};

class ActiveTextField : public TextField {
    private:
        Oled &oled;
    public:
        ActiveTextField(Oled &_oled, const TextField &_tf, uint8_t _x, uint8_t _y) :
            TextField(_tf,_x,_y),
            oled(_oled)
        {}

        void Print(const std::string &s, bool invert=false);
        void Clear();
};

struct UiTemplateEntry {
    const std::vector<TextField> &block;
    uint8_t x;
    uint8_t y;
};

#endif