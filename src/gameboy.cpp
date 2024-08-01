#include "gameboy.h"
#include <SDL2/SDL.h>
#include <SDL_events.h>
#include <chrono>
#include <iostream>

GameBoy::GameBoy(std::string bootrom_file, std::string cartridge_file) {
    /* set up hardware components of the Game Boy */

    // link hardware components
    cpu_.connect_bus(&bus_);
    ppu_.connect_bus(&bus_);

    // load in the boot rom
    bootrom_.load_bootrom_file(bootrom_file);

    // load in the cartridge
    cartridge_.load_cartridge_from_file(cartridge_file);
}

void GameBoy::run() {
    /* the main game loop of the Game Boy*/
    
    /*  The Gameboy has a master clock which is 4.194304 MHz, or 4,194,304 cycles per second 
        Furthermore, the PPU has a 154 scanlines, each of which takes 456 cycles, which means that in total, one frame is 70,224 cycles.
        Overall then, in one frame, we process 4,194,304 / 70,224 frames, giving an effect frame rate of 59.7275 frames per second */
    const std::chrono::duration<double> frame_length(70224.0 / 4194304.0);
    unsigned int master_clock_cycles = 0; // keep track of the elapsed cpu cycles

    std::cout << frame_length << '\n';
    
    // initial white screen
    ppu_.clear_screen();
    ppu_.render();

    // the first frame start time
    auto frame_start = std::chrono::high_resolution_clock::now();

    while (running_) {
        // can run a maximum of 70224 cycles in a frame (yields 4.194304 MHz)
        if (master_clock_cycles < 70224) {
            cpu_.cycle();
            ppu_.cycle();
            master_clock_cycles++;
        }
        else if (master_clock_cycles == 70224) {
            // number of cycles to complete frame, so render

            // poll for a quit event (e.g. user exits out of the emulator)
            poll_events();

            // render the texture to the screen
            ppu_.clear_screen();
            ppu_.render();

            std::cout << "Complete time: " << std::chrono::high_resolution_clock::now() - frame_start << '\n';
            master_clock_cycles++;
        }
        else {
            // waste time until frame length is up (TODO: for now we poll for events, to check if quit, but this will probably change when we need joypad input)
            poll_events();
            if (std::chrono::high_resolution_clock::now() - frame_start >= frame_length) {
                // the frame is now official over, can start prossessing again
                std::cout << "Frame Complete: " << std::chrono::high_resolution_clock::now() - frame_start << '\n';
                master_clock_cycles = 0;
                frame_start = std::chrono::high_resolution_clock::now();
            }
        }

        

    }
}

void GameBoy::poll_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                running_ = false;
                break;
            default:
                break;
        }
    }
}

GameBoy::~GameBoy() {
    SDL_Quit();
}