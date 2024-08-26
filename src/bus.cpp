#include "bus.h"
#include "bootrom.h"
#include "cartridge.h"
#include "joypad.h"
#include "serial.h"
#include "timers.h"
#include <SDL_events.h>
#include <cstdint>
#include <SDL.h>

#include <cpu.h>
#include <ram.h>
#include <ppu.h>

Bus::Bus(CPU* cpu, RAM* ram, PPU* ppu, BootROM* bootrom, Cartridge* cartridge, Serial* serial, Timers* timers, Joypad* joypad) 
{
    // link the bus to all of the hardware components created in the GameBoy class
    cpu_ = cpu;
    ram_ = ram;
    ppu_ = ppu;
    bootrom_ = bootrom;
    cartridge_ = cartridge;
    serial_ = serial;
    timers_ = timers;
    joypad_ = joypad;
}

uint8_t Bus::read(uint16_t address)
{
    if (address >= 0x0000 && address < 0x0100) {
        // read from the boot ROM (no corresponding write, since this is ROM) 
        if (bootrom_->read_bank()) {
            // the bootrom is unmapped
            return cartridge_->read(address);
        }
        else {
            return bootrom_->read(address);
        }
    }
    else if (address >= 0x0100 && address <= 0x7fff) {
        // access to the 16 KiB fixed ROM bank 00, and 16 KiB ROM bank 01, which is switchable and controlled via the MBC
        return cartridge_->read(address);
    }
    else if ((address >= 0x8000 && address <= 0x9fff) || (address >= 0xff40 && address <= 0xff4b)) {
        // read from VRAM (first range), OR read LCD registers (second range)
        return ppu_->read(address);
    }
    else if (address >= 0xa000 && address <= 0xbfff) {
        // acccess 8 KiB of external RAM contained in the cartridge. This is a switchable bank controller by the MBC (which is in the cartridge class)
        return cartridge_->read(address);
    }
    else if (address >= 0xc000 && address <= 0xdfff) {
        // read from work RAM
        return ram_->read(address);
    }
    else if (address == 0xff00) {
        // joypad input. read from the lower 4 bits of the register depending on the current set upper 4 bits of the register
        switch (joypad_->get_selection()) {
            case 0x10:
                // select buttons
                return joypad_->get_buttons();
                break;
            case 0x20:
                // select dpad
                return joypad_->get_dpad();
                break;
            case 0x30:
                // no selection 
                return 0xf;
                break;
        }
    }
    else if (address == 0xff01) {
        return serial_->read_sb();
    }
    else if (address >= 0xff04 && address <= 0xff07) {
        return timers_->read(address);
    }
    else if (address == 0xff0f) {
        // read the interrupt flag
        return cpu_->read_if();
    }
    else if (address >= 0xff80 && address <= 0xfffe) {
        return cpu_->read_hram(address);
    }
    else if (address == 0xffff) {
        // read the interrupt enable register
        return cpu_->read_ie();
    }

    return 0xff;
}

void Bus::write(uint16_t address, uint8_t value) 
{
    if (address >= 0x0000 && address <= 0x7fff) {
        // access to MBC external RAM + MBC registers 
        cartridge_->write(address, value);
    }
    else if ((address >= 0x8000 && address <= 0x9fff) || (address >= 0xff40 && address <= 0xff4b)) {
        // write to VRAM (first range) OR to LCD registers (second range)
        ppu_->write(address, value);
    }
    else if (address >= 0xc000 && address <= 0xdfff) {
        // write to work RAM
        ram_->write(address, value);
    }
    else if (address == 0xff00) {
        // write to the upper 4 bits of the joypad register 
        
    }
    else if (address == 0xff01) {
        serial_->write_sb(value);
    }
    else if (address == 0xff02) {
        serial_->write_sc(value);
    }
    else if (address >= 0xff04 && address <= 0xff07) {
        timers_->write(address, value);
    }
    else if (address == 0xff0f) {
        // update the interrupt flag register (make a request for an interrupt)
        cpu_->write_if(value);
    }
    else if (address == 0xff50) {
        bootrom_->write_bank(value);
    }
    else if (address >= 0xff80 && address <= 0xfffe) {
        cpu_->write_hram(address, value);
    }
    else if (address == 0xffff) {
        // update the interrupt enable register
        cpu_->write_ie(value);
    }
}