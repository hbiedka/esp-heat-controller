#ifndef SELFTEST_H
#define SELFTEST_H

#include <Arduino.h>
#include <vector>
#include "button/buttonGroup.h"

class Selftest {
    private:
        std::vector<uint8_t> &outputs;
        std::vector<uint8_t>::iterator outputs_iter;
        ButtonGroup *panel = nullptr;

        unsigned long ts = 0;

    public:
        Selftest(std::vector<uint8_t>& _outputs, ButtonGroup *_panel);
        void Spin();
        uint8_t ReadButtons();

};

void sendBtnPress(uint8_t btn_id);
void sendBtnHold(uint8_t btn_id);

#endif  // SELFTEST_H