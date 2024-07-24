#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <cstdint>
#include <string>
#include <vector>

class Cartridge {
    public:
        void load_cartridge_from_file(std::string cartridge_file);
    private:
        std::vector<uint8_t> cartridge_; // store the contents of the cartridge into a vector - since this might be variable length with different MBCs, this may be different sizes
};

#endif