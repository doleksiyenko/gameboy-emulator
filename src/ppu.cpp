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
    return vram_[address - 0x8000];
}

void PPU::write(uint16_t address, uint8_t value)
{
    vram_[address - 0x8000] = value;
}