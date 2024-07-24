#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "bootrom.h"
#include "cartridge.h"
#include "cpu.h"
#include "ppu.h"
#include "ram.h"
#include "sound.h"
#include "bus.h"

class GameBoy {
    public:
        GameBoy(std::string bootrom_file, std::string cartridge_file);
        void run();
        ~GameBoy();
    private:
        void poll_events();
    private:
        // state
        bool running_ = true; // start the system as automatically running
        // hardware components
        RAM ram_;        
        PPU ppu_;
        Sound sound_;
        CPU cpu_;
        BootROM bootrom_; // fixed size ROM responsible for booting the console
        Cartridge cartridge_; // game cartridge
        Bus bus_ {&cpu_, &ram_, &ppu_, &bootrom_}; 
};

#endif
