#ifndef LOGIC_INPUT_H
#define LOGIC_INPUT_H

#include "input.h"
#include <Arduino.h>
#include <memory>

enum class LogicInputOperation {
    NOP = 0,
    OR,
    AND
};

class LogicInput : public Input {
    private:
        LogicInputOperation op;
        const std::vector<std::shared_ptr<Input>> inputs;
    public:
        LogicInput(
            LogicInputOperation _op,
            const std::vector<std::shared_ptr<Input>> _inputs);

        void Spin(unsigned long ts) override;
};


#endif