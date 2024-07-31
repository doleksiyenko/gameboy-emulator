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
        uint8_t read(uint16_t address); // read a PPU register, VRAM or OAM
        void write(uint16_t address, uint8_t value); // write to the PPU registers
        void clear_screen();
        void render(); // use SDL to draw the current texture to the screen
        void cycle(); // go through one PPU cycle (process 1 frame)

        // registers
        uint8_t read_ly();

        void lcd_enable();
        void lcd_disable();


    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;

        std::array<uint8_t, 160> oam_; // object attribute memory 
        uint16_t t_cycles_delay_ = 80; // start in mode 2, which lasts 80 "dots"

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

        // stat register
        struct stat_ {
            uint8_t value_ = 2;
            uint8_t lyc_select = 0x0;
            uint8_t mode2_select = 0x0;
            uint8_t mode1_select = 0x0;
            uint8_t mode0_select = 0x0;
            uint8_t lyc_equals = 0x0;
            uint8_t ppu_mode_ = 2; // start in mode 2 by default (OAM scan)

            void set(uint8_t new_lcdc) {
                value_ = new_lcdc;
                lyc_select = new_lcdc & (1 << 6);
                mode2_select = new_lcdc & (1 << 5);
                mode1_select = new_lcdc & (1 << 4);
                mode0_select = new_lcdc & (1 << 3);
                lyc_equals = new_lcdc & (1 << 2);
                ppu_mode_ = new_lcdc & 3;
            }
        };

        // lcdc register
        struct lcdc_ {
            uint8_t value_ = 0x0;
            uint8_t lcdc_enable_ = 0x0;
            uint8_t window_tile_map = 0x0;
            uint8_t window_enable = 0x0;
            uint8_t bg_window_tile_data = 0x0;
            uint8_t bg_tile_map = 0x0;
            uint8_t obj_size = 0x0;
            uint8_t obj_enable = 0x0;
            uint8_t bg_window_enable = 0x0;


            void set(uint8_t new_lcdc) {
                value_ = new_lcdc;
                lcdc_enable_ = new_lcdc & (1 << 7);
                window_tile_map = new_lcdc & (1 << 6);
                window_enable = new_lcdc & (1 << 5);
                bg_window_tile_data = new_lcdc & (1 << 4);
                bg_tile_map = new_lcdc & (1 << 3);
                obj_size = new_lcdc & (1 << 2);
                obj_enable = new_lcdc & (1 << 1);
                bg_window_enable = new_lcdc & (1 << 0);
            }
        };
};

#endif