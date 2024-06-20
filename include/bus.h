/*
bus.h: header file for bus.cpp

The GameBoy has a 16-bit address bus, used to address the various hardware components of
the system.
*/

#ifndef BUS_H
#define BUS_H

#include <cstdint>
#include "cpu.h"
#include "ram.h"

class Bus {
    public:
        void write(uint16_t address);
        uint8_t read(uint16_t address);
    private:
        CPU* cpu; 
        RAM* ram;
};

#endif