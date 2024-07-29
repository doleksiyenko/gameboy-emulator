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
        uint8_t read(uint16_t address); // read a PPU register
        void write(uint16_t address, uint8_t value); // write to the PPU registers
        void clear_screen();
        void render();
    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;

        // -- REGISTERS -- 
        // scrolling
        uint8_t scx_; // background viewport x
        uint8_t scy_; // background viewport y
        uint8_t wx_; // window x position
        uint8_t wy_; // window y position
        // palettes
        uint8_t bgp_; // background colour palette
        uint8_t ogp0_; // object palette 0
        uint8_t ogp1_; // object palette 1
        // lcd status
        uint8_t ly_;
        uint8_t lyc_;
        uint8_t stat_;
        uint8_t lcdc_; // lcd control
};

#endif