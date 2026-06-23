#include "io/pcf8574.h"

PCF8574::PCF8574(uint8_t _address, std::shared_ptr<Input> _intPin) :
    address(_address),
    intPin(_intPin),
    inputs(8)
{
}

void PCF8574::updateInputs(uint8_t data)
{
    for (size_t i = 0; i<8; i++) {
        inputs[i].state = data & 1;
        data >>= 1;
    }
}

void PCF8574::Spin(unsigned long ts)
{
    if (state == PCF8574State::INIT) {
        Wire.setClock(100'000);
        Wire.beginTransmission(address);
        Wire.write(0xFF);
        Wire.endTransmission();
        state = PCF8574State::WAIT_INT_DOWN;
    } else if (state == PCF8574State::WAIT_INT_DOWN) {
        if (intPin->getState()) {
            //interrupt
            Serial.println("int from pcf");
            state = PCF8574State::SEND_RQ;
        }
    } else if (state == PCF8574State::SEND_RQ) {
        Wire.setClock(100'000);
        int bytesRecvd = Wire.requestFrom(address,(uint8_t)1);
        if (bytesRecvd > 0) {
            uint8_t data = Wire.read();
            Serial.printf("got data 0x%02x\n",data);
        } else {
            Serial.println("Error, no data received");
        }

        state = PCF8574State::WAIT_INT_UP;

    } else if (state == PCF8574State::WAIT_INT_UP) {
        if (!intPin->getState()) {
            Serial.println("end int from pcf");
            state = PCF8574State::WAIT_INT_DOWN;
        }
    }
}