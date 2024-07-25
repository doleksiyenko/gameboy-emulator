#include "./mbc/mbc1.h"
#include <cstdint>

uint8_t MBC1::read(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x3fff) {
        // this is the fixed bank area (ROM bank X0)
    }
    else if (address >= 0x4000 && address <= 0x7fff) {
        // this is the switchable bank area (ROM bank 01 - 7f)
    }
    else if (address >= 0xa000 && address <= 0xbfff) {
        // RAM bank 00-03
    }

    return cartridge_data_[address]; 
}

void MBC1::write(uint16_t address, uint8_t value)
{
    /* the write function of the MBC changes the MBC control registers */
    if (address >= 0x0000 && address <= 0x1fff) {
        // if the last 4 bits of value contain the value a, enable ram. Any other value disables it
        uint8_t lower_bits = value & 0xf;
        if (lower_bits >> 3 == 0xa || lower_bits >> 2 == 0xa || lower_bits >> 1 == 0xa || (lower_bits & 0x1) == 0xa) {
            ram_enable_ = true;
        }
        else {
            ram_enable_ = false;
        }
    }
    else if (address >= 0x2000 && address <= 0x3fff) {
        // sets the ROM bank number, which selects which ROM bank is exposed to the 4000-7fff region 
        switch (rom_size_code) {
            case 0x0:
                break; // no banking
            case 0x1:
                rom_bank_number_ = value & 0x3; // 4 banks, use 2 bits
                break;
            case 0x2:
                rom_bank_number_ = value & 0x7; // 8 banks, use 3 bits
                break;
            case 0x3:
                rom_bank_number_ = value & 0xf; // 16 banks, use 4 bits
                break;
            case 0x4:
                rom_bank_number_ = value & 0x1f; // 32 banks, use 5 bits
                break;
        }
    }
    else if (address >= 0x4000 && address <= 0x5fff) {
        /* this write instruction only performs an action if this MBC1 cart
        has 32KiB of RAM (set RAM bank) or is 1 MiB ROM or larger (additional rom_bank_number bits) */
        if (external_ram_size_code == 0x03) {
            // 32 KiB RAM cartridge
            ram_bank_number_ = value & 0x03;
        }
        else if (rom_size_code >= 0x05) {
            // 1 MiB ROM cartridge or larger (=> max 8 KiB of external RAM)
            rom_bank_number_ |= ((value & 0x03) << 5); 
        }
    }

    else if (address >= 0x4000 && address <= 0x5fff) {
        /* select MBC1 banking mode */
        if (value == 0x0) {
            banking_mode_ = 1;
        }
        else {
            banking_mode_ = 0;
        }
    }
}