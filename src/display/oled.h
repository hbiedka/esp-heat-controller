#ifndef OLED_H
#define OLED_H

#include <Arduino.h>
#include <Wire.h>
#include <string>

#define OLED_WID 128
#define OLED_HEI 8  //8 pages (=64px)

enum class OledWakeUpMode {
    OFF,    //do not sleep
    MANUAL, //trigger manually using WakeUpPulse()
    AUTO    //trigger on each display update
};

class Oled {

    private:
        uint8_t address = 0x3C;
        unsigned long lastWakeUpTs = 0;
        bool isAwake = false;
        OledWakeUpMode wakeUpMode = OledWakeUpMode::OFF;

        void cmd(uint8_t c);

        void WakeUp();
    public:
        uint8_t width = OLED_WID;
        uint8_t height = OLED_HEI;
        Oled(uint8_t a) : address(a) {};
        void Init();
        void Clear();

        void SetWakeUpMode(OledWakeUpMode mode);
        bool WakeUpPulse();

        int8_t PrintChar(char c, size_t font_id, uint8_t col, uint8_t page,uint8_t offset);
        void PrintStr(const std::string &s, size_t font_id, uint8_t col, uint8_t page,uint8_t offset);
        void ClearStr(uint8_t col,uint8_t page, uint8_t wid);

        void Spin(unsigned long ts);
};

#endif