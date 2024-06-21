#ifndef PPU_H
#define PPU_H

#include <SDL_render.h>
#include <SDL_video.h>
#include <cstdint>
#include <array>

#define SCREEN_HEIGHT 144
#define SCREEN_WIDTH 160

class PPU {
    public:
        std::array<uint8_t, 1024 * 8> vram_; // starting address of VRAM is 0x8000
    public:
        PPU();
        ~PPU();
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value);
        void clear_screen();
        void render();
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;
};

#endif