#include "bus.h"
#include "bootrom.h"
#include "cartridge.h"
#include <cstdint>

#include <cpu.h>
#include <ram.h>
#include <ppu.h>

Bus::Bus(CPU* cpu, RAM* ram, PPU* ppu, BootROM* bootrom, Cartridge* cartridge) 
{
    // link the bus to all of the hardware components created in the GameBoy class
    cpu_ = cpu;
    ram_ = ram;
    ppu_ = ppu;
    bootrom_ = bootrom;
    cartridge_ = cartridge;
}

uint8_t Bus::read(uint16_t address)
{
    if (address >= 0x0000 && address < 0x0100) {
        // read from the boot ROM (no corresponding write, since this is ROM) 
        return bootrom_->read(address);
    }
    else if (address >= 0x0100 && address <= 0x7fff) {
        // access to the 16 KiB fixed ROM bank 00, and 16 KiB ROM bank 01, which is switchable and controlled via the MBC
        return cartridge_->read(address);
    }
    else if ((address >= 0x8000 && address <= 0x9fff) || (address >= 0xff40 && address <= 0xff4b)) {
        // read from VRAM (first range), OR read LCD registers (second range)
        return ppu_->read(address);
    }
    else if (address >= 0xA000 && address <= 0xbfff) {
        // acccess 8 KiB of external RAM contained in the cartridge. This is a switchable bank controller by the MBC (which is in the cartridge class)
        return cartridge_->read(address);
    }
    else if (address >= 0xc000 && address <= 0xdfff) {
        // read from work RAM
        return ram_->read(address);
    }
    else if (address == 0xff0f) {
        // read the interrupt flag
        return cpu_->read_if();
    }
    else if (address == 0xffff) {
        // read the interrupt enable register
        return cpu_->read_ie();
    }

    return 0x0;
}

void Bus::write(uint16_t address, uint8_t value) 
{
    if ((address >= 0x8000 && address <= 0x9fff) || (address >= 0xff40 && address <= 0xff4b)) {
        // write to VRAM (first range) OR to LCD registers (second range)
        ppu_->write(address, value);
    }
    else if (address >= 0xc000 && address <= 0xdfff) {
        // write to work RAM
        ram_->write(address, value);
    }
    else if (address == 0xff0f) {
        // update the interrupt flag register (make a request for an interrupt)
        cpu_->write_if(value);
    }
    else if (address == 0xffff) {
        // update the interrupt enable register
        cpu_->write_ie(value);
    }
}