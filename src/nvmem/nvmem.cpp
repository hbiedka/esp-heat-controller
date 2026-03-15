#include "nvmem.h"

static uint16_t crc16(uint8_t *buf, size_t len)
{
    uint16_t crc = 0;
	for (size_t i = 0; i < len; i++)
	{
		crc ^= buf[i]; 
		for (int j = 8; j != 0; j--) {
			if ((crc & 0x0001) != 0) {
				crc >>= 1;
				crc ^= 0xA001;
			}
			else
				crc >>= 1;
		}
	}

	return crc;
}

static size_t toBlocks(size_t bytes)
{
    int blocks = bytes / EEPROM_BLOCK_SIZE;
    if (bytes % EEPROM_BLOCK_SIZE) blocks++;
    return blocks;
}

static void show() {
    for(size_t i = 0; i < 64; i++)
        Serial.printf("%d: 0x%x\n",i,EEPROM.read(i));
}

NVMem::NVMem(ObjectModel &om, size_t size) : om(om) {
    memory.resize(size);

    numEeepromBlocks = toBlocks(size);
}

void NVMem::init() {
    EEPROM.begin(numEeepromBlocks * EEPROM_BLOCK_LEN);
}

void NVMem::slowInitAndLoad() {
    init();
    delay(10);
    load();
}

bool NVMem::loadBlock(size_t num) {
            
    uint8_t data[EEPROM_BLOCK_SIZE];
    uint8_t crc[EEPROM_CRC_SIZE];
    uint16_t read_crc;
    
    size_t offset = num * EEPROM_BLOCK_LEN;
    for (size_t i = 0; i < EEPROM_BLOCK_SIZE; i++)
        data[i] = EEPROM.read(offset+i);
    
    offset += EEPROM_BLOCK_SIZE;

    for (size_t i = 0; i < EEPROM_CRC_SIZE; i++)
        crc[i] = EEPROM.read(offset+i);

    //calculate and check crc16
    uint16_t calculated_crc = crc16(data,EEPROM_BLOCK_SIZE);
    memcpy(&read_crc,crc,EEPROM_CRC_SIZE);

    //TODO throw error if crc does not match
    if (read_crc != calculated_crc) {
        Serial.printf("CRC ERROR bl %d CRC read 0x%x calc 0x%x\n",num,read_crc,calculated_crc);
        return false;
    }

    auto it = memory.begin() + (num*EEPROM_BLOCK_SIZE);
    for (size_t i = 0; i < EEPROM_BLOCK_SIZE; i++) {
        *(it++) = data[i];
        if (it == memory.end()) break;
    }

    return true;
}

bool NVMem::saveBlock(size_t num) {

    uint8_t data[EEPROM_BLOCK_SIZE];
    uint8_t crc[EEPROM_CRC_SIZE];

    auto it = memory.begin() + (num*EEPROM_BLOCK_SIZE);
    for (size_t i = 0; i < EEPROM_BLOCK_SIZE; i++) {
        if ( it < memory.end()) {
            data[i] = *(it++);
        } else {
            //Fill residual bytes in block
            data[i] = 0;
        }
    }

    //calculate CRC16
    uint16_t calculated_crc = crc16(data,EEPROM_BLOCK_SIZE);
    memcpy(crc,&calculated_crc,EEPROM_CRC_SIZE);

    size_t offset = num * EEPROM_BLOCK_LEN;
    for (size_t i = 0; i < EEPROM_BLOCK_SIZE; i++)
        EEPROM.write(offset+i,data[i]);
    
    offset += EEPROM_BLOCK_SIZE;

    for (size_t i = 0; i < EEPROM_CRC_SIZE; i++)
        EEPROM.write(offset+i,crc[i]);

    return EEPROM.commit();
}


void NVMem::load() {

    // do a dirty dump to calculate number of blocks required
    std::vector<uint8_t> tempMemory;
    tempMemory.reserve(memory.size());

    auto tempIt = om.NVMDump(tempMemory.begin(),tempMemory.end());

    size_t blocksToLoad = toBlocks(std::distance(tempMemory.begin(),tempIt));

    bool ret;
    for (size_t block = 0; block < blocksToLoad; block++) {
        ret = loadBlock(block);
        if (!ret) {
            Serial.printf("Failed to load block %d\n", block);
            return;
        }
    }

    auto it = om.NVMLoad(memory.begin(), memory.end());
    Serial.printf("%d bytes loaded from EEPROM\n",std::distance(memory.begin(),it));
}

void NVMem::save() {
    auto it = om.NVMDump(memory.begin(), memory.end());
    Serial.printf("%d bytes ready to save to EEPROM\n",std::distance(memory.begin(),it));

    size_t blocksToSave = toBlocks(std::distance(memory.begin(),it));  

    for (size_t block = 0; block < blocksToSave; block++) {
        bool saved = saveBlock(block);
        delay(2);
        Serial.printf("block %d of %d %s\n",
            block+1,blocksToSave,
            saved ? "saved" : "skipped"
        );                
    }
}