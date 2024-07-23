#ifndef BOOTROM_H
#define BOOTROM_H

#include <array>
#include <string>
#include <cstdint>

class BootROM {
    public:
        BootROM();
        void load_bootrom_file(std::string bootrom_file);
        uint8_t read(uint16_t address); // when the bus needs to read data from the bootrom
    private:
        std::array<uint8_t, 256> bootrom_; // array containing all the data from the bootrom
};

#endif