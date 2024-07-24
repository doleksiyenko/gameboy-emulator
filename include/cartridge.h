#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "mbc/mbc.h"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <ram.h>

class Cartridge {
    public:
        void load_cartridge_from_file(std::string cartridge_file);
        uint8_t read(uint16_t address);
    private:
        std::vector<uint8_t> cartridge_; // store the contents of the cartridge into a vector - since this might be variable length with different MBCs, this may be different sizes
        uint8_t mbc_header_val_; // MBC (memory bank controller) mode of the cartridge
        std::unique_ptr<MBC> mbc_;
};

#endif