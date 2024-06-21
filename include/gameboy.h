#ifndef GAMEBOY_H
#define GAMEBOY_H

#include "cpu.h"
#include "ppu.h"
#include "ram.h"
#include "sound.h"
#include "bus.h"

class GameBoy {
    public:
        void run();
    private:
        RAM ram_;        
        PPU ppu_;
        Sound sound_;
        CPU cpu_;
        Bus bus_ {&cpu_, &ram_, &ppu_}; 
};

#endif
