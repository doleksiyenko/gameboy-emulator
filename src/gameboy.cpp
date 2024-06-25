#include "gameboy.h"
#include <SDL2/SDL.h>
#include <SDL_events.h>

GameBoy::GameBoy() {
    // link hardware components
    cpu_.connect_bus(&bus_);
}

void GameBoy::run() {
    // TODO: load in the cartridge

    while (running_) {

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