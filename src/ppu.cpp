#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_hints.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <cstddef>
#include <iostream>

#include "ppu.h"

PPU::PPU() 
{
    // initialize SDL 
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: " << SDL_GetError();
        exit(-1);
    }

    // create a window, renderer and texture
    window_ = SDL_CreateWindow("GameBoy 1989", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window_) {
        std::cout << "Failed to create window: " << SDL_GetError();
        exit(-1);
    }

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "nearest");
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        std::cout << "Failed to create renderer: " << SDL_GetError();
        exit(-1);
    }

    texture_ = SDL_CreateTexture(renderer_, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!texture_) {
        std::cout << "Failed to create texture: " << SDL_GetError();
    }
}

PPU::~PPU() {
    // destructor - exit out of SDL, and destroy all allocated resources
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}


void PPU::clear_screen() {
    // set the screen to black
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
}

void PPU::render() {
    // copy the texture to the screen, then update the window
    SDL_RenderCopy(renderer_, texture_, NULL, NULL);
    SDL_RenderPresent(renderer_);
}

uint8_t PPU::read(uint16_t address)
{
    /* Read from registers, VRAM or OAM. TODO: only can read from VRAM and OAM during HBlank and VBlank periods */
    if (address >= 0x8000 && address <= 0x9fff) {
        return vram_[address - 0x8000];
    }
    else if (address == 0xFF44) {
        return ly_;
    }

    return 0xff; // return junk value
}

void PPU::write(uint16_t address, uint8_t value)
{
    if (address >= 0x8000 && address <= 0x9fff) {
        vram_[address - 0x8000] = value;
    }
    else if (address >= 0xfe00 && address <= 0xfe9f) {
        // it is possible to write to the OAM directly, but this is only possible during HBlank and VBlank periods
        oam_[address - 0xfe00] = value;
    }
}

void PPU::cycle()
{
    /* Run through one cycle of the PPU. This processes 1 frame, and switches between the 4 possible PPU modes. Each PPU mode takes a certain number of cycles
    to complete (t-cycles, which are controlled by the master clock also counting the CPU cycles) */

    // check if the mode is "over", and therefore we need to switch
    if (t_cycles_delay_ == 0) {
        ppu_mode_ = (ppu_mode_ + 1) % 4;
    }

    // process what happens in each mode
    switch (ppu_mode_) {
        case 0:
            // HBlank (variable length based on mode 3)
            break;
        case 1:
            // VBlank
            break;
        case 2:
            // OAM scan
            t_cycles_delay_ += 80;
            break;
        case 3:
            // Drawing pixels (variable length)
            break;
    }

    if (t_cycles_delay_ > 0) {
        t_cycles_delay_--; 
    }
}









