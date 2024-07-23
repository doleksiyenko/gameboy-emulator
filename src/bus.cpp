#include "bus.h"
#include "bootrom.h"
#include <cstdint>

#include <cpu.h>
#include <ram.h>
#include <ppu.h>

Bus::Bus(CPU* cpu, RAM* ram, PPU* ppu, BootROM* bootrom) 
{
    // link the bus to all of the hardware components created in the GameBoy class
    cpu_ = cpu;
    ram_ = ram;
    ppu_ = ppu;
    bootrom_ = bootrom;
}

uint8_t Bus::read(uint16_t address)
{
    if (address >= 0x0000 && address < 0x0100) {
        // read from the boot ROM (no corresponding write, since this is ROM) 
        return bootrom_->read(address);
    }
    else if (address >= 0x8000 && address <= 0x9fff) {
        // read from VRAM
        return ppu_->read(address);
    }
    else if (address >= 0xc000 && address <= 0xdfff) {
        // read from work RAM
        return ram_->read(address);
    }
    else if (address == 0xffff) {
        return cpu_->read_ie();
    }

    return 0x0;
}

void Bus::write(uint16_t address, uint8_t value) 
{
    if (address >= 0x8000 && address <= 0x9fff) {
        // write to VRAM
        ppu_->write(address, value);
    }
    else if (address >= 0xc000 && address <= 0xdfff) {
        // write to work RAM
        ram_->write(address, value);
    }
    else if (address == 0xffff) {
        // update the interrupt enable register
        cpu_->write_ie(value);
    }
}