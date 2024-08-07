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

    // initialize the texture to white
    SDL_SetRenderTarget(renderer_, texture_);
    SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
    SDL_SetRenderTarget(renderer_, NULL);
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
    SDL_SetRenderTarget(renderer_, NULL);
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer_);
}

void PPU::render() {
    // copy the texture to the screen, then update the window
    SDL_SetRenderTarget(renderer_, NULL);
    SDL_RenderCopy(renderer_, texture_, NULL, NULL);
    SDL_RenderPresent(renderer_);
    // get ready for drawing the next frame to the texture
    SDL_SetRenderTarget(renderer_, texture_);
}

uint8_t PPU::read(uint16_t address)
{
    /* Read from registers, VRAM or OAM. TODO: only can read from VRAM and OAM during HBlank and VBlank periods */
    if (address >= 0x8000 && address <= 0x9fff) {
        // vram is not accessible during mode 3, but if the LCD + PPU are off, VRAM and OAM are immediately avaiable
        if ((stat_.ppu_mode_ != 3) || lcdc_.lcdc_enable_ == 0) {
            return vram_[address - 0x8000];
        }
    }
    else if (address >= 0xfe00 && address <= 0xfe9f) {
        // the cpu can only directly read from the OAM during HBlank or VBlank or if the LCD and PPU are off
        if (stat_.ppu_mode_ == 0 || stat_.ppu_mode_ == 1 || lcdc_.lcdc_enable_ == 0) {
            return oam_[address - 0xfe00];
        }
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
        // the cpu can only write to VRAM if the mode is not 3, otherwise ignore write
        if (stat_.ppu_mode_ != 3) {
            vram_[address - 0x8000] = value;
        }
    }
    else if (address >= 0xfe00 && address <= 0xfe9f) {
        // the cpu can only directly write to the OAM during HBlank or VBlank, otherwise ignore write
        if (stat_.ppu_mode_ == 0 || stat_.ppu_mode_ == 1) {
            oam_[address - 0xfe00] = value;
        }
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
}

void PPU::set_mode(uint8_t mode) 
{
        if (stat_.set_mode(mode)) {
            // set mode indicated that the STAT interrupt should be executed; make the request manually
            uint8_t interrupt_flag = bus_->read(0xff0f);
            interrupt_flag |= (1 << 1); // update the LCD / STAT flag in IF
            bus_->write(0xff0f, interrupt_flag); 
        }
}


void PPU::cycle()
{
    /* Run through one cycle of the PPU. This processes 1 dot, and switches between the 4 possible PPU modes. Each PPU mode takes a certain number of cycles
    to complete (t-cycles, which are controlled by the master clock also counting the CPU cycles) */

    /* TODO: implement mode 3 variable timing */

    // check if the mode is "over", and therefore we need to switch
    if (lcdc_.lcdc_enable_) {
        // only process cycles on the PPU if the LCD is enabled
        if (t_cycles_delay_ == 0) {
            switch (stat_.ppu_mode_) {
                case 0:
                    // Switch from HBlank to OAM scan or to VBlank depending on the scanline number
                    {
                        // if we reach this point, we are in mode 0, and have currently finished the scanline.
                        // if we just finished the scanline, and are currently on scanline 143, scanlines 144 - 153 are mode 1 -> the next mode should be VBlank
                        if (ly_ == 143) {
                        set_mode(1); 
                        t_cycles_delay_ += 456; // execute for 1 scanline 
                        }
                        else {
                            set_mode(2);
                            t_cycles_delay_ += 80;
                        }

                        // reaching the end of mode 0 is always the indication of the next scanline
                        ly_++;

                    } 
                    break;
                case 1:
                    //  switch from VBlank to OAM scan if in the last scanline of the frame, otherwise remain in VBlank
                    {
                        if (ly_ == 153) {
                            // we just finished the last scanline, so loop back to the first scanline 
                            set_mode(2);
                            t_cycles_delay_ += 80;
                            ly_ = 0;
                            oam_search();
                        }
                        else {
                            // we remain in mode 1, and progress the scanline
                            set_mode(1);
                            t_cycles_delay_ += 456;
                            ly_++;
                        }
                    }
                    break;
                case 2:
                    // switch from OAM scan to drawing
                    set_mode(3);
                    draw_scanline();
                    t_cycles_delay_ += 172; // MODE 3 has a variable length, for now keep it at the maximum length
                    break;
                case 3:
                    // switch from drawing pixels to HBlank
                    set_mode(0);
                    t_cycles_delay_ += 204; // MODE 0 has a variable length, depending on MODE 3 length (based on (376 - MODE 3 Duration))
                    break;
            }
            
        }

        if (t_cycles_delay_ > 0) {
            t_cycles_delay_--; 
        }
    }
    else {
        // SWITCH OFF LCD: clear the screen to blank white
        if (!screen_cleared_) {
            SDL_SetRenderTarget(renderer_, texture_);
            SDL_SetRenderDrawColor(renderer_, 255, 255, 255, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer_);
        }
    }
}

void PPU::oam_search()
{
    /*  OAM search occurs during mode 2 of the PPU. During this mode, we scan through the OAM (object attribute memory)
        and determine which objects should be displayed on the scanline */

    // cycle through the OAM, only need to check the first byte of every object (4 bytes) to determine Y pos
    for (int byte0 = 0; byte0 < 160; byte0 += 4) {
        uint8_t y_pos = oam_[byte0];
        // check if the current scanline (ly) is within this object's top scanline and its bottom scanline
        if (ly_ >= y_pos && ly_ <= y_pos + (8 * (1 + lcdc_.obj_size))) { // object is either 8 pixels tall or 16 pixels tall depending on LCDC bit
            scanline_sprites_.push_back(byte0); // for now, store the objects position in the OAM array
        }

        if (scanline_sprites_.size() == 10) {
            break;
        }
    }
}

void PPU::draw_scanline()
{
    /*  Draw the scanline during mode 3 of the PPU. In a hardware accurate GameBoy emulator, this should draw one
        pixel per cycle; this method will draw the whole scanline at once at the beginning of mode 3 */
    screen_cleared_ = false;

    // TODO: fetch background, window data, sprite data. Handle priority of sprite data with opacity based on X coordinate
    // or OAM location

    // ----- BACKGROUND AND WINDOW ----- 

    // first, we need to check if the window is enabled, and if it is, then is the current scanline part of that window?
    // NOTE: the window is a fixed rectangle on top of the background layer (i.e. a status bar)
    bool window_enabled = false;

    if (lcdc_.bg_window_enable) {
        if (wy_ <= ly_) { // only the top left coordinate of the window can be specified; this condition means that this scanline contains the window 
            window_enabled = true;        
        }
    }

    // which tile map we use depends on 1) whether we are accessing window or background tile maps, 2) lcdc bits
    uint16_t tile_map_address;
    if (window_enabled) {
        // use window tile map
        if (lcdc_.window_tile_map) {
            tile_map_address = 0x9c00;
        }
        else {
            tile_map_address = 0x9800;
        }
    }
    else {
        // use background tile map 
        if (lcdc_.bg_tile_map) {
            tile_map_address = 0x9c00;
        }
        else {
            tile_map_address = 0x9800;
        }
    }

    // check where we read tiles from (the current addressing mode)
    uint16_t tile_data_address;
    bool signed_addressing = false; // indexing is always done using an 8 bit integer, however depending on the addressing mode this can be signed or unsigned
    if (lcdc_.bg_window_tile_data) {
        tile_data_address = 0x8000;
    }
    else {
        tile_data_address = 0x8800;
        signed_addressing = true;
    }
    

    /*  The next step is to index into the VRAM tile maps. The background of the Game Boy is defined by 32 x 32 tiles. Each of these tiles has 8x8 pixels. Overall, this corresponds to a larger display than our actual display.
        The next step is to figure out which pixels actually need to be drawn to screen */ 
    
    // first, figure out the Y coordinate relative to either the viewport, or the position of the window (if the window is enabled)
    int y_coordinate;
    if (window_enabled) {
        // if the window is enabled, find the Y position relative to the top line of the window (i.e. "window coordinates")
        y_coordinate = ly_ - wy_;
    }
    else {
        // the window is not enabled, so find Y position in "background coordinates". We know that the background begins drawing from where the viewport top left corner is defined
        y_coordinate = scy_ + ly_;
    }

    // Now, we need to find which row of the tile map this y coordinate can be found. First, finding the integer division by 8 finds which row this pixel belongs to (e.g. pixel 6 belongs to row 0, pixel 35 belongs to row 1, etc.). 
    // and then multiplying by 32 gives the index into the row in memory

    uint8_t tile_map_y_index = static_cast<uint8_t>(y_coordinate / 8) * 32;


    // draw each pixel along the scanline
    for (int pixel = 0; pixel < SCREEN_WIDTH; pixel++) {
    }


    // SDL_SetRenderDrawColor(renderer_, 1, 0, 0, SDL_ALPHA_OPAQUE);
    // SDL_RenderDrawPoint(renderer_, 50, 100);
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

    if ((mode != 3) && (value_ & (1 << (mode + 3)))) { // every mode has a select apart from mode 3
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