#ifndef PPU_H
#define PPU_H

#include <SDL_render.h>
#include <SDL_video.h>
#include <_types/_uint8_t.h>
#include <cstdint>
#include <array>
#include <vector>

#define SCREEN_HEIGHT 144
#define SCREEN_WIDTH 160

class Bus; // forward declaration of class Bus

class PPU {
    private:
        std::array<uint8_t, 1024 * 8> vram_; // starting address of VRAM is 0x8000
        std::array<uint8_t, 160> oam_; // object attributes
    public:
        PPU();
        ~PPU();

        void connect_bus(Bus* bus);

        uint8_t read(uint16_t address); // read a PPU register, VRAM or OAM
        void write(uint16_t address, uint8_t value); // write to the PPU registers
        void clear_screen();
        void render(); // use SDL to draw the current texture to the screen
        void cycle(); // go through one PPU cycle (process 1 frame)

        void display_frame_rate(float frame_rate); // show the frame rate in the title of the window

        // registers
        uint8_t read_ly();

    private:
        SDL_Window* window_;
        SDL_Renderer* renderer_;
        SDL_Texture* texture_;

        Bus* bus_; // hold a reference to the bus

        uint16_t t_cycles_delay_ = 80; // start in mode 2, which lasts 80 "dots"

        void set_mode(uint8_t mode); // set the mode and handle the resulting possible STAT interrupt
        void draw_scanline();
        void draw_bg_window();
        void draw_sprites();
        void set_colour_from_palette(int* r, int* g, int* b, uint8_t colour_ID, uint8_t palette);

        void oam_scan(); // during mode 2, perform the oam_scan, which finds up to 10 sprites to display
        void oam_dma_transfer(uint8_t value); // when requested perform an OAM DMA transfer
        std::vector<int> scanline_sprites_; // up to 10 sprites that can be displayed on a scanline
        
        // -- REGISTERS -- 
        // scrolling
        uint8_t scx_ = 0; // background viewport x
        uint8_t scy_ = 0; // background viewport y
        uint8_t wx_ = 0; // window x position
        uint8_t wy_ = 0; // window y position
        // palettes
        uint8_t bgp_ = 0; // background colour palette
        uint8_t obp0_ = 0; // object palette 0
        uint8_t obp1_ = 0; // object palette 1
        // lcd status
        uint8_t ly_ = 0;
        uint8_t lyc_ = 0;

        uint8_t dma_source_ = 0;

        bool screen_cleared_ = true; // PPU and LCD start as "off". Checks if screen is filled white, so we don't need to process it every frame if already cleared
        // ---- STATUS AND CONTROL REGISTERS ----

        // stat register
        struct stat 
        {
            uint8_t value_ = 2;
            uint8_t lyc_select = 0x0;
            uint8_t mode2_select = 0x0;
            uint8_t mode1_select = 0x0;
            uint8_t mode0_select = 0x0;
            uint8_t lyc_equals = 0x0;
            uint8_t ppu_mode_ = 2; // start in mode 2 by default (OAM scan)

            void set(uint8_t new_lcdc); // set READ/WRITE bits
            bool set_lyc_equals(uint8_t ly, uint8_t lyc); // set READ-ONLY LYC == LY bit
            bool set_mode(uint8_t mode); // set READ-ONLY mode bit
        };

        // lcdc register
        struct lcdc 
        {
            uint8_t value_ = 0x0;
            uint8_t lcdc_enable_ = 0x0;
            uint8_t window_tile_map = 0x0;
            uint8_t window_enable = 0x0;
            uint8_t bg_window_tile_data = 0x0;
            uint8_t bg_tile_map = 0x0;
            uint8_t obj_size = 0x0;
            uint8_t obj_enable = 0x0;
            uint8_t enable_priority = 0x0;

            // set READ/WRITE bits
            void set(uint8_t new_lcdc); 
        };
    
        lcdc lcdc_;
        stat stat_;

    private:
        void test_draw_vram();
};

#endif