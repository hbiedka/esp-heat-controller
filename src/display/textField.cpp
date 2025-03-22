#include "oledUi.h"


void TextField::Print(const String &s) {
    if (oled == nullptr) return;        //TODO: throw exception

    Clear();
    //TODO: limit length
    oled->PrintStr(s,font,x,y,offset);
}

void TextField::Clear() {
    oled->ClearStr(x,y,len);
}