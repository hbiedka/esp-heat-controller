#ifndef LOGIC_INPUT_H
#define LOGIC_INPUT_H

#include "input.h"
#include <Arduino.h>

enum class LogicInputOperation {
    NOP = 0,
    OR,
    AND
};

class LogicInput : public Input {
    private:
        LogicInputOperation op;
        const std::vector<Input*> inputs;
    public:
        LogicInput(LogicInputOperation _op, const std::vector<Input*> _inputs);

        void Spin() override;
};


#endif