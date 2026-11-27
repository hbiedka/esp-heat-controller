#include "oledUi.h"

void ActiveTextField::Print(const std::string &s,bool invert) {
    oled.PrintStr(s,font,x,y,len,offset,border,padding,invert);
}

void ActiveTextField::Clear() {
    oled.ClearStr(font,x,y,len,offset,border);
}