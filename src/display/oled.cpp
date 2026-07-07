#include "oled.h"
#include "fonts.h"


void Oled::cmd(uint8_t c) {
    Wire.write(0x80);
    Wire.write(c);
}

void Oled::Init() {
    // init display
  Wire.begin();
  Wire.setClock(400'000);   // enable high-speed mode

  Wire.beginTransmission(address);
  cmd(0xAE);      //AE Display off (11)
  cmd(0xA6);   //Normal display (8)
  cmd(0xD5);  //D5 Set display clock div (15)
  cmd(0x80);
  cmd(0xA1);      // Segremap (6)
  cmd(0xC8);    // Comscandec (13)
  cmd(0x81);   //Set contrast (5)
  cmd(0xCF);
  cmd(0xD9);  //D9 Set precharge (16)
  cmd(0xF1);
  cmd(0xDB); //DB Set VComDetect (18)
  cmd(0x40);
  cmd(0xAF);   //AF Display on
  Wire.endTransmission();

  Wire.beginTransmission(address);
  cmd(0x02);  //column lower 2
  cmd(0x10);  //column higher 0

  cmd(0x40);  //line 0

  cmd(0xB0);  //page 0
  Wire.endTransmission();

  isAwake = true;
}

void Oled::Clear() {
    Wire.setClock(400'000);
    for (uint8_t i = 0; i < 0xF ;i++) {
        Wire.beginTransmission(address);
        cmd(0xB0 | i);  //page i
        cmd(0x02);         //column lower 2
        cmd(0x10);          //column higher 0
        Wire.endTransmission();
        
        //we need to split 128 bytes to 2 frames, each 1+64 bit long
        for (uint8_t j = 0; j < 2; j++) {
            Wire.beginTransmission(address);
            Wire.write(0x40);
            for(uint8_t k = 0; k <64; k++)
                Wire.write(0x00);
            
            Wire.endTransmission();
        }
    }
}

int8_t Oled::PrintChar(char c, size_t font_id, uint8_t col, uint8_t page,uint8_t offset) {
    if (wakeUpMode == OledWakeUpMode::AUTO)
        WakeUp();

    // TODO limit font_id

    Font *F = fonts[font_id];

    if (c < F->first_ch || c > F->last_ch) return -1;
    if (page > 0x0F) return -1;

    uint8_t pos = c-F->first_ch;

    //read address and len
    uint16_t reg = pgm_read_word(F->offsets+pos);
    uint16_t next_reg = pgm_read_word(F->offsets+pos+1);
    uint8_t wid = next_reg-reg; //TODO casting

    uint16_t data;

    Wire.setClock(400'000);
    for (uint8_t i = 0; i < 2; i++) {
        Wire.beginTransmission(address);

        cmd(0xB0 | (page+i));      //set page
        cmd(col & 0x0F);      //set column lower
        cmd(0x10 | ((col >> 4) & 0x0F));  //set column higher
        Wire.write(0x40);

        for (uint8_t j = 0; j < wid; j++) {
            data = pgm_read_word(F->data+reg+j);
            Wire.write(((data << offset) >> (8*i)) & 0xFF );
        }
        Wire.endTransmission();
    }

    return col+wid;

}

void Oled::PrintStr(const std::string &s, size_t font_id, uint8_t col, uint8_t page,uint8_t offset) {
    uint8_t c = col;
    int8_t newc;
    for(unsigned int i = 0; i < s.length(); i++) {
        if (c > 128) break;

        newc = PrintChar(s[i],font_id,c,page,offset);
        if (newc < 0) return;
        c = newc;
    }
}

void Oled::ClearStr(uint8_t col,uint8_t page, uint8_t wid) {
    if (wakeUpMode == OledWakeUpMode::AUTO)
        WakeUp();

    Wire.setClock(400'000);
    for (uint8_t i = 0; i < 2; i++) {
        Wire.beginTransmission(address);

        cmd(0xB0 | (page+i));      //set page
        cmd(col & 0x0F);      //set column lower
        cmd(0x10 | ((col >> 4) & 0x0F));  //set column higher
        Wire.write(0x40);

        for (uint8_t j = 0; j < wid; j++) {
            Wire.write(0x00);
        }
        Wire.endTransmission();
    }
}

void Oled::Spin(unsigned long ts) {
    if (wakeUpMode != OledWakeUpMode::OFF && ts - lastWakeUpTs > 30000) {
        // sleep display until next wakeup
        Wire.setClock(400'000);   // enable high-speed mode
        Wire.beginTransmission(address);
        cmd(0xAE);      //AE Display off (11)
        Wire.endTransmission();
        isAwake = false;
    }
}

/* Wake up trigger - updates the timer, wakes up if not awake */
void Oled::WakeUp() {
    lastWakeUpTs = millis();
    if (!isAwake) {
        Wire.setClock(400'000);   // enable high-speed mode
        Wire.beginTransmission(address);
        cmd(0xAF);   //AF Display on
        Wire.endTransmission();

        isAwake = true;
    }
}

/* Wake-up mode setter */
void Oled::SetWakeUpMode(OledWakeUpMode mode)
{
    wakeUpMode = mode;
}

/*
    Wake up only if not awake,
    Returns true if just woken up, false if woken up yet
*/
bool Oled::WakeUpPulse()
{
    if (isAwake) return false;
    WakeUp();
    return true;
}
