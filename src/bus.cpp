#include "bus.h"
#include <cstdint>

#include <cpu.h>
#include <ram.h>
#include <ppu.h>

Bus::Bus(CPU* cpu, RAM* ram, PPU* ppu) 
{
    // link the bus to all of the hardware components created in the GameBoy class
    cpu_ = cpu;
    ram_ = ram;
    ppu_ = ppu;



}

uint8_t Bus::read(uint16_t address)
{
    if (address >= 0x8000 && address <= 0x9fff) {
        // read from VRAM
        return ppu_->read(address);
    }
    else if (address >= 0xc000 && address <= 0xdfff) {
        // read from work RAM
        return ram_->read(address);
    }
}

void Bus::write(uint16_t address, uint8_t value) 
{
    if (address >= 0x8000 && address <= 0x9fff) {
        // write to VRAM
        ppu_->write(address, value);
    }
    if (address >= 0xc000 && address <= 0xdfff) {
        // write to work RAM
        ram_->write(address, value);
    }
}