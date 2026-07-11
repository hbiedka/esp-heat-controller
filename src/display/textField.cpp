#include "oledUi.h"

void ActiveTextField::Print(const std::string &s) {
    Clear();
    //TODO: limit length
    oled.PrintStr(s,font,x,y,offset);
}

void ActiveTextField::Clear() {
    oled.ClearStr(x,y,len);
}