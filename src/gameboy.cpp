#include "gameboy.h"
#include <SDL2/SDL.h>
#include <SDL_events.h>

GameBoy::GameBoy(std::string bootrom_file, std::string cartridge_file) {
    /* set up hardware components of the Game Boy */

    // link hardware components
    cpu_.connect_bus(&bus_);

    // load in the boot rom
    bootrom_.load_bootrom_file(bootrom_file);

    // load in the cartridge
    cartridge_.load_cartridge_from_file(cartridge_file);
}

void GameBoy::run() {
    /* the main game loop of the Game Boy*/

    while (running_) {
        cpu_.cycle();

        // poll for a quit event (e.g. user exits out of the emulator)
        poll_events();
        
        // update the screen
        ppu_.clear_screen();
        ppu_.render();

        // use SDL Delay for now, later will use more precise clock timing
        SDL_Delay(16);
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