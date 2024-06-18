#ifndef PPU_H
#define PPU_H

#include <cstdint>
#include <array>

class PPU {
    public:
        std::array<uint8_t, 1024 * 8> vram;
};

#endif