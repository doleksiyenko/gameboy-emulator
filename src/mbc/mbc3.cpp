#include "./mbc/mbc3.h"
#include <cstdint>

uint8_t MBC3::read(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x3fff) {
        // this is the fixed bank area (ROM bank X0)
        if (banking_mode_ == 0) {
            return cartridge_data_[address];
        }
        else {
           return cartridge_data_[(static_cast<uint32_t>(ram_bank_upper_bit_reg) << 19) + address];
        }
    }
    else if (address >= 0x4000 && address <= 0x7fff) {
        // this is the switchable bank area (ROM bank 01 - 7f)
        return cartridge_data_[(static_cast<uint32_t>(ram_bank_upper_bit_reg) << 19) + (static_cast<uint32_t>(rom_bank_number_) << 14) + address];
    }
    else if (address >= 0xa000 && address <= 0xbfff) {
        // RAM bank 00-03. We can only read RAM values if RAM is enabled
        if (ram_enable_) {
            if (banking_mode_ == 0) {
                return cartridge_data_[address];
            }
            else {
                return cartridge_data_[(static_cast<uint16_t>(ram_bank_upper_bit_reg) << 13) + address];
            }
            // after accessing external RAM, disable it
            ram_enable_ = false;
        }
        else {
            // RAM is not enabled so return junk data
            return 0xff;
        }
    }

    return cartridge_data_[address]; 
}

void MBC3::write(uint16_t address, uint8_t value)
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
        /* this selects the correct RAM bank number for MBC3 */
        if (external_ram_size_code == 0x03 || rom_size_code >= 0x05) {
            // 32 KiB RAM cartridge
            ram_bank_upper_bit_reg = value & 0x03;
        }
    }
    else if (address >= 0x6000 && address <= 0x7fff) {
    }

    else if (address >= 0xa000 && address <= 0xbfff && ram_enable_) {
        // RAM bank 00-03. We can only write to RAM values if RAM is enabled
        if (ram_enable_) {
            if (banking_mode_ == 0) {
                cartridge_data_[address] = value;
            }
            else {
                cartridge_data_[(static_cast<uint16_t>(ram_bank_upper_bit_reg) << 13) + address] = value;
            }
            // after accessing external RAM, disable it
            ram_enable_ = false;
        }
    }
}