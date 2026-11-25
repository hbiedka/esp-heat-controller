#include "oledUi.h"

void ActiveTextField::Print(const std::string &s) {
    Clear();
    oled.PrintStr(s,font,x,y,len,offset,border);
}

void ActiveTextField::Clear() {
    oled.ClearStr(font,x,y,len,offset,border);
}