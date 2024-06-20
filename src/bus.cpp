#include "bus.h"
#include <cstdint>

#include <cpu.h>
#include <ram.h>
#include <ppu.h>

Bus::Bus(CPU* cpu, RAM* ram, PPU* ppu) {
    // link the bus to all of the hardware components created in the GameBoy class
    cpu_ = cpu;
    ram_ = ram;
    ppu_ = ppu;
}

uint8_t Bus::read(uint16_t address)
{
    if (address >= 0xc000 && address <= 0xdfff) {
        // read from work RAM
    }
}

void Bus::write(uint16_t address) 
{
    if (address >= 0xc000 && address <= 0xdfff) {
        // write to work RAM
    }
}