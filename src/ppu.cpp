#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_hints.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <cstddef>
#include <iostream>

#include "ppu.h"
#include "bus.h"

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

void PPU::connect_bus(Bus* bus) 
{
    bus_ = bus;
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
    else {
        // reading a register
        switch (address) {
            case 0xff40:
                return lcdc_.value_;
            case 0xff41:
                return stat_.value_;
            case 0xff44:
                return ly_;
            case 0xff45:
                return lyc_;
            case 0xff42:
                return scy_;
            case 0xff43:
                return scx_;
            case 0xff4a:
                return wy_;
            case 0xff4b:
                return wx_;
            case 0xff47:
                return bgp_;
            case 0xff48:
                return ogp0_;
            case 0xff49:
                return ogp1_;
        }
    }

    return 0xff; // return junk value
}

void PPU::write(uint16_t address, uint8_t value)
{
    if (address >= 0x8000 && address <= 0x9fff) {
        vram_[address - 0x8000] = value;
    }
    else {
        // writing to a register
        switch (address) {
            case 0xff40:
                lcdc_.set(value);
                break;
            case 0xff41:
                stat_.set(value);
                break;
            case 0xff45:
                lyc_ = value;
                break;
            case 0xff42:
                scy_ = value;
                break;
            case 0xff43:
                scx_ = value;
                break;
            case 0xff4a:
                wy_ = value;
                break;
            case 0xff4b:
                wx_ = value;
                break;
            case 0xff47:
                bgp_ = value;
                break;
            case 0xff48:
                ogp0_ = value;
                break;
            case 0xff49:
                ogp1_ = value;
                break;
        }
    }

    // else if (address >= 0xfe00 && address <= 0xfe9f) {
    //     // it is possible to write to the OAM directly, but this is only possible during HBlank and VBlank periods
    //     oam_[address - 0xfe00] = value;
    // }
}

bool PPU::cycle()
{
    /* Run through one cycle of the PPU. This processes 1 frame, and switches between the 4 possible PPU modes. Each PPU mode takes a certain number of cycles
    to complete (t-cycles, which are controlled by the master clock also counting the CPU cycles) */
    // check if the mode is "over", and therefore we need to switch
    if (t_cycles_delay_ == 0) {
        if (stat_.set_mode((stat_.ppu_mode_ + 1) % 4))  {
            // set mode indicated that the STAT interrupt should be executed; make the request manually
            uint8_t interrupt_flag = bus_->read(0xff0f);
            interrupt_flag |= (1 << 1); // update the LCD / STAT flag in IF
            bus_->write(0xff0f, interrupt_flag); 
        }
    }

    // process what happens in each mode
    switch (stat_.ppu_mode_) {
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

    return false;
}



// -- STAT REGISTER methods -- 
void PPU::stat::set(uint8_t new_lcdc)
{
    /* set the read / write bits of the STAT register */
    value_ &= 7; // clear all but read only bits
    value_ |= new_lcdc & 0b01111000; // set the read/write bits
    lyc_select = new_lcdc & (1 << 6);
    mode2_select = new_lcdc & (1 << 5);
    mode1_select = new_lcdc & (1 << 4);
    mode0_select = new_lcdc & (1 << 3);
}

bool PPU::stat::set_lyc_equals(uint8_t ly, uint8_t lyc) 
{
    if (ly == lyc) {
        lyc_equals = 1;
        value_ |= (1 << 2); // set the LYC == LC bit
        if (lyc_select) { // check the condition for the STAT interrupt
            return true;        
        } 
    }
    else {
        lyc_equals = 0; 
        value_ &= 0b01111011; // clear the LYC == LC bit
    }
    
    return false;
}


bool PPU::stat::set_mode(uint8_t mode) 
{
    ppu_mode_ = mode;
    value_ &= 0b01111100; // clear previous mode bits
    value_ |= mode;       // set mode bits

    if ((mode != 3) &&
        (value_ &
        (1 << (mode +
                3)))) { // every mode has a select apart from mode 3
    // trigger a STAT interrupt
    return true;
    }

    return false;
}

// -- LCDC REGISTER methods --
void PPU::lcdc::set(uint8_t new_lcdc) 
{
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