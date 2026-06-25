#include "io/pcf8574.h"
#include <stdexcept>

PCF8574::PCF8574(uint8_t _address, std::shared_ptr<Input> _intPin) :
    address(_address),
    intPin(_intPin),
    ios(8,std::make_shared<PCF8574Unused>())
{}

std::shared_ptr<PCF8574Button>
PCF8574::RegisterButton(size_t ioNum, uint8_t buttonId)
{
    if (ioNum >= ios.size())
        throw std::out_of_range("RegisterButton: index must be from 0 to 7");

    auto btn = std::make_shared<PCF8574Button>(buttonId);
    ios[ioNum] = btn;
    return btn;
}

std::shared_ptr<PCF8574Input>
PCF8574::RegisterInput(size_t ioNum)
{
    if (ioNum >= ios.size())
        throw std::out_of_range("RegisterButton: index must be from 0 to 7");

    auto in = std::make_shared<PCF8574Input>();
    ios[ioNum] = in;
    return in;
}

void PCF8574::UpdateInputs(uint8_t data)
{
    for (size_t i = 0; i<8; i++) {
        auto& io = ios[i];
        bool st = !(data & 1);
        if (std::holds_alternative<std::shared_ptr<PCF8574Input>>(io)) {
            std::get<std::shared_ptr<PCF8574Input>>(io)->SetState(st);
        } else if (std::holds_alternative<std::shared_ptr<PCF8574Button>>(io)) {
            std::get<std::shared_ptr<PCF8574Button>>(io)->SetState(st);
        }
        data >>= 1;
    }
}

void PCF8574::ShowIO()
{
    for (size_t i = 0; i< 8; i++) {
        auto& io = ios[i];
        if (std::holds_alternative<std::shared_ptr<PCF8574Input>>(io)) {
            Serial.printf("index %d holds in\n",i);
        } else if (std::holds_alternative<std::shared_ptr<PCF8574Button>>(io)) {
            Serial.printf("index %d holds btn id\n",i);
        } else if (std::holds_alternative<std::shared_ptr<PCF8574Unused>>(io)) {
            Serial.printf("index %d is unused\n",i);
        }
    }
}

void PCF8574::Spin(unsigned long ts)
{
    //spin children
    for (auto& io : ios) {
        if (std::holds_alternative<std::shared_ptr<PCF8574Input>>(io)) {
            std::get<std::shared_ptr<PCF8574Input>>(io)->Spin(ts);
        }
    }

    if (state == PCF8574State::INIT) {
        Wire.setClock(100'000);
        Wire.beginTransmission(address);
        Wire.write(0xFF);
        Wire.endTransmission();
        state = PCF8574State::WAIT_INT_DOWN;
    } else if (state == PCF8574State::WAIT_INT_DOWN) {
        if (intPin->getState()) {
            //interrupt
            state = PCF8574State::SEND_RQ;
        }
    } else if (state == PCF8574State::SEND_RQ) {
        Wire.setClock(100'000);
        int bytesRecvd = Wire.requestFrom(address,(uint8_t)1);
        if (bytesRecvd > 0) {
            uint8_t data = Wire.read();
            UpdateInputs(data);
        } else {
            Serial.println("Error, no data received");
        }

        state = PCF8574State::WAIT_INT_UP;
        lastTs = ts;    //to trigger timeout

    } else if (state == PCF8574State::WAIT_INT_UP) {
        

        if (!intPin->getState()) {
            state = PCF8574State::WAIT_INT_DOWN;
        } else {
            // sometimes int pin does not go back to high state,
            // so the best is to repeat read after some time
            if (lastTs - ts > 100) {
                Serial.println("PCF: Timeout waiting for int pin up, updating");
                state = PCF8574State::SEND_RQ;
            }

        }
    }
}