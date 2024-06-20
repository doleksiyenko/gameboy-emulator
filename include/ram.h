#ifndef RAM_H
#define RAM_H

#include <array>
#include <cstdint>

class RAM {
    public:
        std::array<uint8_t, 1024 * 8> ram_;
    public:
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
};

#endif