#ifndef RAM_H
#define RAM_H

#include <array>
#include <cstdint>

class RAM {
    public:
        std::array<uint8_t, 1024 * 8> ram;
};

#endif