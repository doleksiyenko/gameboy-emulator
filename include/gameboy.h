#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"
#include "ppu.h"
#include "ram.h"
#include "sound.h"
#include "bus.h"

class GameBoy {
    public:
        GameBoy();
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
        Bus bus_ {&cpu_, &ram_, &ppu_}; 
};

#endif
