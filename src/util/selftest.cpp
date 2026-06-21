#include "selftest.h"

Selftest::Selftest(std::vector<uint8_t>& _outputs, ButtonGroup *_panel)
  : outputs(_outputs), panel(_panel)
  {
      outputs_iter = outputs.begin();
  }

void Selftest::Init() {
  if (panel != nullptr) {
    panel->assignInteractiveObject(this);
  }
}

void Selftest::Spin() {
    if (panel == nullptr) return;

    uint8_t b = ReadButtons();

    if (b != 1 && ts < millis()) {
      ts = millis() + (b == 3 ? 200 : 400);

      digitalWrite(*outputs_iter, b==2);
      outputs_iter++;
      if (outputs_iter == outputs.end()) outputs_iter = outputs.begin();
      digitalWrite(*outputs_iter, b!=2);

    }
}

void Selftest::ButtonCallback(uint8_t id, bool hold) {
    Serial.printf("Button %d ",id);
    Serial.println(hold ? "hold" : "press");
}

uint8_t Selftest::ReadButtons() {
    if (panel == nullptr) return 0;

    if (panel->isPressed(1)) return 1;
    if (panel->isPressed(2)) return 2;
    if (panel->isPressed(3)) return 3;
  
  return 0;
}
