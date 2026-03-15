#ifndef NVMEM_H
#define NVMEM_H

#include <Arduino.h>
#include <EEPROM.h>

#include "objectModel/ObjectModel.h"

#define EEPROM_BLOCK_SIZE 16
#define EEPROM_CRC_SIZE 2
#define EEPROM_BLOCK_LEN (EEPROM_BLOCK_SIZE+EEPROM_CRC_SIZE)

class NVMem {
    private:
        std::vector<uint8_t> memory;
        ObjectModel &om;

        size_t numEeepromBlocks;

        bool loadBlock(size_t num);
        bool saveBlock(size_t num);

    public:
        NVMem(ObjectModel &om, size_t size);
        void init();
        void slowInitAndLoad();
        void load();
        void save();
};

#endif // NVMEM_H