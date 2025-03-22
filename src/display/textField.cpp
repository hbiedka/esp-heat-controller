#include "oledUi.h"

// TextField TextField::&operator=(const TextField &tf) {
//     x = tf.x;
//     y = tf.y;
//     font = tf.font;
//     offset = tf.offset;
//     Serial.println("TextField::operator=");
//     return *this;
// }


void TextField::Print(const String &s) {
    if (oled == nullptr) return;        //TODO: throw exception
    oled->PrintStr(s,font,x,y,offset);
}