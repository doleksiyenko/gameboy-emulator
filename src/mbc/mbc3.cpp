#include "./mbc/mbc3.h"
#include <cstdint>
#include <iostream>

uint8_t MBC3::read(uint16_t address)
{
    if (address >= 0x0000 && address <= 0x3fff) {
        // this is the fixed bank area (ROM bank 00)
        return cartridge_data_[address];
    }
    else if (address >= 0x4000 && address <= 0x7fff) {
        // this is the switchable bank area (ROM bank 01 - 7f)
        uint offset = address - 0x4000;
        uint rom_address = 0x4000 * rom_bank_number_;
        return cartridge_data_[offset + rom_address];
    }
    else if (address >= 0xa000 && address <= 0xbfff) {
        // RAM bank 00-03. We can only read RAM values if RAM is enabled
        if (ram_bank_reg_ <= 0x3) {
            if (ram_enable_) {
                // TODO: RAM read
            }
            else {
                // RAM is not enabled so return junk data
                return 0xff;
            }
        }
        else if (ram_bank_reg_ >= 0x8 && ram_bank_reg_ <= 0xc) {
            if (rtc_rw_enable_) {
                switch (ram_bank_reg_) {
                    case 0x8:
                        return rtc_s;
                    case 0x9:
                        return rtc_m;
                    case 0xa:
                        return rtc_h;
                    case 0xb:
                        return rtc_dl;
                    case 0xc:
                        return rtc_dh;
                }
            }
            else {
                return 0xff;
            }
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
        if (lower_bits == 0xa) {
            ram_enable_ = true;
            rtc_rw_enable_= true;
        }
        else {
            ram_enable_ = false;
            rtc_rw_enable_ = false;
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
            case 0x5:
                rom_bank_number_ = value & 0x3f; // 64 banks, use 6 bits
                break;
            case 0x6:
                rom_bank_number_ = value & 0x7f; // 128 banks, use 7 bits
                break;
        }
        if (rom_bank_number_ == 0) {
            rom_bank_number_++;
        }
    }
    else if (address >= 0x4000 && address <= 0x5fff) {
        /* select the correct RAM bank number from 00 - 03 for MBC3 */
        if (value <= 0x3) {
            if (external_ram_size_code == 0x03) {
                // 32 KiB RAM cartridge
                ram_bank_reg_ = value & 0x03;
            }
        }
        else if (value >= 0x08 && value <= 0x0c) {
            ram_bank_reg_ = value;
        }
    }
    else if (address >= 0x6000 && address <= 0x7fff) {
        if (latch_clock_data_ == 0 && value == 1) {
            latch_clock();
        }
        latch_clock_data_ = value;
    }

    else if (address >= 0xa000 && address <= 0xbfff && ram_enable_) {
        // RAM bank 00-03. We can only write to RAM values if RAM is enabled
        if (ram_bank_reg_ <= 0x3) {
            if (ram_enable_) {
                // TODO: ram write
            }
        }
        else if (ram_bank_reg_ >= 0x8 && ram_bank_reg_ <= 0xc) {
            if (rtc_rw_enable_) {
                switch (ram_bank_reg_) {
                    case 0x8:
                        rtc_s = value;
                        break;
                    case 0x9:
                        rtc_m = value;
                        break;
                    case 0xa:
                        rtc_h = value;
                        break;
                    case 0xb:
                        rtc_dl = value;
                        break;
                    case 0xc:
                        rtc_dh = value;
                        break;
                }
            }
        }
    }
}


void MBC3::latch_clock()
{
    // TODO: implement timer feature
}