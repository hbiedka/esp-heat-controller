#include "logicInput.h"

LogicInput::LogicInput(LogicInputOperation _op, const std::vector<Input*> _inputs):
    op(_op),
    inputs(_inputs)
{}

void LogicInput::Spin(unsigned long ts)
{
    if (inputs.empty()) return;

    switch (op)
    {
        case LogicInputOperation::OR:
            state = false;
            for (auto* in : inputs) {
                state |= in->getState();
                if (state) break;
            }
            break;

        case LogicInputOperation::AND:
            state = true;
            for (auto* in : inputs) {
                state &= in->getState();
                if (!state) break;
            }
            break;

        default:
            break;
    }
}
