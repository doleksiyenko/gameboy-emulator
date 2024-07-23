/*
bus.h: header file for bus.cpp

The GameBoy has a 16-bit address bus, used to address the various hardware components of
the system.

Constructor: take pointers to hardware components created in the Gameboy class, and link
            them to the private members. 
*/

#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "bootrom.h"
#include "cpu.h"
#include "ram.h"
#include "ppu.h"

class Bus {
    public:
        Bus(CPU* cpu, RAM* ram, PPU* ppu);
        void write(uint16_t address, uint8_t value);
        uint8_t read(uint16_t address);
    private:
        CPU* cpu_; 
        RAM* ram_;
        PPU* ppu_;
        BootROM* bootrom_;
};

#endif