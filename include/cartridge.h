#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "mbc/mbc.h"
#include <_types/_uint8_t.h>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include <ram.h>

class Cartridge {
    public:
        void load_cartridge_from_file(std::string cartridge_file);
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value); // if this cartridge has an MBC, we need to access the external RAM + MBC registers
    private:
        std::vector<uint8_t> cartridge_; // store the contents of the cartridge into a vector - since this might be variable length with different MBCs, this may be different sizes
        uint8_t mbc_header_val_; // MBC (memory bank controller) mode of the cartridge
        std::unique_ptr<MBC> mbc_;

        std::unordered_map<uint8_t, std::string> mbc_code_to_name_ = 
        {
            {0x0, "ROM ONLY"},
            {0x1, "MBC1"},
            {0x2, "MBC1 + RAM"},
            {0x3, "MBC1 + RAM + BATTERY"},
            {0x11, "MBC3"},
            {0x12, "MBC3 + RAM"},
            {0x13, "MBC3 + RAM + BATTERY"}
        };

        std::unordered_map<uint8_t, std::string> rom_code_to_name_ =
        {
            {0x0, "32 KiB"},
            {0x1, "64 KiB"},
            {0x2, "128 KiB"},
            {0x3, "256 KiB"},
            {0x4, "512 KiB"},
            {0x5, "1 MiB"},
            {0x6, "2 MiB"},
            {0x7, "4 MiB"},
            {0x8, "8 MiB"},
            {0x52, "1.1 MiB"},
            {0x53, "1.2 MiB"},
            {0x54, "1.5 MiB"}
        };

        std::unordered_map<uint8_t, std::string> ram_code_to_name_ =
        {
            {0x0, "0 bytes"},
            {0x1, "Unused"},
            {0x2, "8 KiB"},
            {0x3, "32 KiB"},
            {0x4, "128 KiB"},
            {0x5, "64 KiB"},
        };
};

#endif