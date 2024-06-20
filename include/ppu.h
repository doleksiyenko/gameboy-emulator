#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <array>

class PPU {
    public:
        std::array<uint8_t, 1024 * 8> vram_;

    public:
        PPU();
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
};

#endif