#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Wire.h>

#define OLED_WID 128
#define OLED_HEI 8  //8 pages (=64px)

class Oled {

    private:
        uint8_t address = 0x3C;

        void cmd(uint8_t c);
    public:
        uint8_t width = OLED_WID;
        uint8_t height = OLED_HEI;
        Oled(uint8_t a) : address(a) {};
        void Init();
        void Clear();

        int8_t PrintChar(char c, size_t font_id, uint8_t col, uint8_t page,uint8_t offset);
        void PrintStr(const String &s, size_t font_id, uint8_t col, uint8_t page,uint8_t offset);
        void ClearStr(uint8_t col,uint8_t page, uint8_t wid);
};

#endif