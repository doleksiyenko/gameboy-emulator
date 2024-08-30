#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_hints.h>
#include <SDL_pixels.h>
#include <SDL_render.h>
#include <SDL_surface.h>
#include <SDL_video.h>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <type_traits>

#include "ppu.h"
#include "bus.h"

PPU::PPU() 
{
    // initialize SDL 
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "Error: " << SDL_GetError();
        exit(-1);
    }

    int scale = 4;
    // create a window, renderer and texture
    window_ = SDL_CreateWindow("GameBoy 1989", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, scale * SCREEN_WIDTH, scale * SCREEN_HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
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


    // start the OAM with 0s
    oam_.fill(0);
}

PPU::~PPU() {
    // destructor - exit out of SDL, and destroy all allocated resources
    SDL_DestroyTexture(texture_);
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
}

void PPU::display_frame_rate(float frame_rate)
{
    SDL_SetWindowTitle(window_, std::to_string(frame_rate).c_str());
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
                // for CPU-DEBUG
                // return 0x90;
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
                return obp0_;
            case 0xff49:
                return obp1_;
            case 0xff46:
                return dma_source_;
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
            case 0xff42:
                scy_ = value;
                break;
            case 0xff43:
                scx_ = value;
                break;
            case 0xff45:
                lyc_ = value;
                break;
            case 0xff46:
                dma_source_ = value;
                // TODO: the OAM DMA transfer will happen instantaneously. However, in reality,
                // this should take 160 M cycles, and CPU access, PPU access should be restricted
                oam_dma_transfer(value);
                break;
            case 0xff47:
                bgp_ = value;
                break;
            case 0xff48:
                obp0_ = value;
                break;
            case 0xff49:
                obp1_ = value;
                break;
            case 0xff4a:
                wy_ = value;
                break;
            case 0xff4b:
                wx_ = value;
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

            // constantly update the LY == LYC flag, and check if a stat interrupt needs to be requested
            if (stat_.set_lyc_equals(ly_, lyc_)) {
                uint8_t interrupt_flag = bus_->read(0xff0f);
                interrupt_flag |= (1 << 1); // update the LCD / STAT flag in IF
                bus_->write(0xff0f, interrupt_flag); 
            }

            switch (stat_.ppu_mode_) {
                case 0:
                    // Switch from HBlank to OAM scan or to VBlank depending on the scanline number
                    {
                        // if we reach this point, we are in mode 0, and have currently finished the scanline.
                        // if we just finished the scanline, and are currently on scanline 143, scanlines 144 - 153 are mode 1 -> the next mode should be VBlank
                        if (ly_ == 143) {
                            set_mode(1); 
                            t_cycles_delay_ += 456; // execute for 1 scanline 
                            // create a VBlank interrupt request 
                            uint8_t interrupt_flag = bus_->read(0xff0f);
                            interrupt_flag |= (1 << 0); // update the timer flag IF
                            bus_->write(0xff0f, interrupt_flag); 
                        }
                        else {
                            set_mode(2);
                            oam_scan();
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
                            oam_scan();
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
                    // test_draw_vram();
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
            screen_cleared_ = true;
        }
        ly_ = 0;
        set_mode(0);
    }
}


void PPU::set_colour_from_palette(int* r, int* g, int* b, uint8_t colour_ID, uint8_t palette)
{
    /* Given a colour ID, set the r, g, b values by using the PPU's palette */

    // based on the colour ID, take a look at the bits of the palette corresponding to the ID
    uint8_t colour_value;

    switch(colour_ID) {
        case 0:
            // look at bits 0, 1
            colour_value = (palette & 0b00000011) >> 0;
            break;
        case 1:
            // look at bits 2, 3
            colour_value = (palette & 0b00001100) >> 2;
            break;
        case 2:
            // look at bits 4, 5
            colour_value = (palette & 0b00110000) >> 4;
            break;
        case 3:
            // look at bits 6, 7
            colour_value = (palette & 0b11000000) >> 6;
            break;
    }

    switch (colour_value) {
        case 0:
            // set the colour to near-white
            *r = 242;        
            *g = 242;
            *b = 242;
            break;
        case 1:
            // set the colour to light gray 
            *r = 191;        
            *g = 191;
            *b = 191;
            break;
        case 2:
            // set the colour to dark gray 
            *r = 115;        
            *g = 115;
            *b = 115;
            break;
        case 3:
            // set the colour to black 
            *r = 0;        
            *g = 0;
            *b = 0;
            break;
    }
}

void PPU::test_draw_vram()
{
    /* Test function. Draw out the whole tilemap in the first section (for drawing Nintendo logo tiles) */

    uint16_t tile_data_area = 0x8000;
    int r = 0; int g = 0; int b = 0;

    for (int y = 0; y <= SCREEN_HEIGHT - 8; y += 8) {
        for (int x = 0; x <= SCREEN_WIDTH - 8; x += 8) {
            // read the 16 bytes
            // match the colour ID to its actual colour using the palette 
            for (int i = 0; i < 8; i++) {
                uint8_t byte1 = vram_[tile_data_area - 0x8000];
                uint8_t byte2 = vram_[(tile_data_area + 1) - 0x8000];
                // draw the row
                for (int j = 0; j < 8; j++) {
                    set_colour_from_palette(&r, &g, &b, ((byte1 & (1 << (7 - j))) >> (7 - j)) + (((byte2 & (1 << (7 - j))) >> (7 - j)) << 1), bgp_);
                    SDL_SetRenderDrawColor(renderer_, r, g, b, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawPoint(renderer_, x + j, y + i);
                }
                // after drawing the row, fetch the next two bytes 
                tile_data_area += 2;
            }
        }
    }
    
}

void PPU::oam_dma_transfer(uint8_t value)
{
   /*  When requested, perform a transfer from ROM or RAM to OAM. <value> specifies the 
        transfer source address divided by 0x100 */

    uint16_t source_address = value << 8; // multiply by 0x100 to retrieve the original source address
    for (int i = 0; i < 160 ; i++) { // write to 160 possible OAM locations
        oam_[i] = bus_->read(source_address + i);
    }
}

void PPU::oam_scan()
{
    /*  OAM search occurs during mode 2 of the PPU. During this mode, we scan through the OAM (object attribute memory)
        and determine which objects should be displayed on the scanline */

   scanline_sprites_.clear(); // first, clear any objects from previous scanlines 

    // cycle through the OAM, only need to check the first byte of every object (4 bytes) to determine Y pos
    for (int byte0 = 0; byte0 < 160; byte0 += 4) {
        uint8_t y_pos = oam_[byte0] - 16;
        // check if the current scanline (ly) is within this object's top scanline and its bottom scanline
        if (ly_ >= y_pos && ly_ <= y_pos + (8 * (1 + lcdc_.obj_size))) { // object is either 8 pixels tall or 16 pixels tall depending on LCDC bit
            scanline_sprites_.push_back(byte0); // store the objects position in the OAM array
        }
        if (scanline_sprites_.size() == 10) {
            break;
        }
    }
}
void PPU::draw_bg_window()
{
    /*
    This method is called by the draw_scanline method. It deals with drawing the background and window tiles. 
    */

    // ----- BACKGROUND AND WINDOW ----- 

    // first, we need to check if the window is enabled, and if it is, then is the current scanline part of that window?
    // NOTE: the window is a fixed rectangle on top of the background layer (i.e. a status bar)
    bool window_enabled = false;

    if (lcdc_.window_enable) {
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
        tile_data_address = 0x9000; // 0x8800 method uses 0x9000 as base pointer, and uses signed addressing
        signed_addressing = true;
    }
    

    /*  The next step is to index into the VRAM tile maps. The background of the Game Boy is defined by 32 x 32 tiles. Each of these tiles has 8x8 pixels. Overall, this corresponds to a larger display than our actual display.
        The next step is to figure out which pixels actually need to be drawn to screen */ 
    
    // first, figure out the Y coordinate relative to either the viewport, or the position of the window (if the window is enabled)
    int y_coordinate;
    if (window_enabled) {
        // if the window is enabled, find the Y position relative to the top line of the window (i.e. "window coordinates")
        // INVARIANT: y_coordinate >= 0 since window_enabled == true if wy_ <= ly_
        y_coordinate = ly_ - wy_;
    }
    else {
        // the window is not enabled, so find Y position in "background coordinates". We know that the background begins drawing from where the viewport top left corner is defined
        y_coordinate = (scy_ + ly_) % 256;
    }

    // Now, we need to find which row of the tile map this y coordinate can be found. First, finding the integer division by 8 finds which row this pixel belongs to (e.g. pixel 6 belongs to row 0, pixel 35 belongs to row 1, etc.). 
    // and then multiplying by 32 gives the index into the row in memory

    uint16_t tile_map_y_index = static_cast<uint8_t>(y_coordinate / 8) * 32;

    // draw each pixel along the scanline
    int x_coordinate;
    for (int pixel = 0; pixel < SCREEN_WIDTH; pixel++) {
        // again, finding the x coordinate depends on whether or not the window is enabled or not: find the coordinate relative to the window or the viewport
        if (window_enabled) {
            // adjust the x coordinate into window space if it is within the window
            if (pixel >= wx_ - 7) {
                x_coordinate = pixel - (wx_ - 7);
            }
        }
        else {
            // x coordinate relative to the viewport
            x_coordinate = (scx_ + pixel) % 256;
        }


        // similar to finding the Y index, we need to find the x index to find a final uint8_t index into the tilemap
        uint8_t tile_map_x_index = static_cast<uint8_t>(x_coordinate / 8); // divide by 8 to bucket into one of 32 8x8 tiles in the row

        uint16_t tile_index = tile_map_address + (tile_map_y_index + tile_map_x_index); // find the location of the 1 byte index of the tile in the tile data in VRAM

        uint16_t tile_data_location;
        if (!signed_addressing) {
            // if using unsigned addressing, the base pointer is at 0x8000, and use unsigned 8 bit index to index into block 0 or block 1 
            // Each tile is 16 bytes in size. Given the tile index, we can find the location of the first byte of the tile by multiplying the index by 16 
            tile_data_location = tile_data_address + vram_[tile_index - 0x8000] * 16; 
        }
        else {
            // using the 0x8800 method, uses 0x9000 as base pointer
            tile_data_location = tile_data_address + static_cast<int8_t>(vram_[tile_index - 0x8000]) * 16;
        }

        // Now, we have the base pointer for the 16 bytes of tile data. Currently, we are on a specific scanline, and therefore are dealing with a specific 
        // y coordinate. Therefore, we need to find that specific row of tile data that we are currently dealing with

        uint8_t tile_row = y_coordinate % 8; // given the y coordinate, which row of colour data in the an 8x8 tile is this?
        tile_row *= 2; // adjust for the fact that every line of tile data contains 2 bytes (every colour row is 2 * 8 bytes of data, so the first row of colour data is row 0, 1, the second row of colour data is row 2,3, etc.) 

        // read the two bytes corresponding to this row of the tile
        // first byte specifies the least significant bit of the color ID, second byte specifies the most significant bit (for the specific x, y pixel we are currently observing)
        uint8_t byte1 = vram_[(tile_data_location + tile_row + 0) - 0x8000];
        uint8_t byte2 = vram_[(tile_data_location + tile_row + 1) - 0x8000];

        // The final step is to get the colour - first, get the ID from the two bytes - bit 7 represents the leftmost pixel, bit 0 represents the rightmost
        uint8_t bit_number = ((x_coordinate % 8) - 7) * (-1); // which column in the tile data is the x coordinate, and then accounting for bit 7 being the leftmost pixel
        uint8_t msb = ((byte2 & (1 << bit_number)) >> bit_number) << 1;
        uint8_t lsb = (byte1 & (1 << bit_number)) >> bit_number; 

        uint8_t colour_ID = msb + lsb;

        // match the colour ID to its actual colour using the palette 
        int r; int g; int b;
        set_colour_from_palette(&r, &g, &b, colour_ID, bgp_);

        SDL_SetRenderDrawColor(renderer_, r, g, b, SDL_ALPHA_OPAQUE);
        SDL_RenderDrawPoint(renderer_, pixel, ly_);
    }
}

void PPU::draw_sprites()
{
    /*
    This method is called by the draw_scanline method. It deals with drawing the sprites. 
    */

    // TODO : y and x flipping

    for (int sprite_loc : scanline_sprites_) {
        uint8_t y_pos = oam_[sprite_loc + 0] - 16; 
        uint8_t x_pos = oam_[sprite_loc + 1] - 8; 
        uint8_t tile_index = oam_[sprite_loc + 2];
        uint8_t attributes = oam_[sprite_loc + 3];

        uint8_t x_flip = attributes & 0b1000;
        uint8_t y_flip = attributes & 0b10000;
        uint8_t priority = attributes & 0b100000;
        uint8_t palette = (attributes & 0b100) >> 2;
        
        // since we already checked in the OAM scan, we know that this sprite intersects with this
        // scanline. However, we need to find which line of the 8x8 or 8x16 sprite we are rendering

        uint8_t sprite_line = (ly_ - y_pos); 
        sprite_line *= 2; // a lot of this logic follows the bg and window rendering: each tile has 2 bytes of information per row

        uint16_t tile_address = 0x8000 + (tile_index * 16) + sprite_line; // only unsigned addressing for sprites
        // read the two bytes corresponding to this row of the tile
        // first byte specifies the least significant bit of the color ID, second byte specifies the most significant bit (for the specific x, y pixel we are currently observing)
        uint8_t byte1 = vram_[tile_address + 0];
        uint8_t byte2 = vram_[tile_address + 1];

        // draw 8 pixels across
        for (int bit_number = 7; bit_number >= 0; bit_number--) {
            uint8_t msb = ((byte2 & (1 << bit_number)) >> bit_number) << 1;
            uint8_t lsb = (byte1 & (1 << bit_number)) >> bit_number; 
            uint8_t colour_ID = msb + lsb;

            if (colour_ID != 0) {
                // in sprite palettes, ignore the lower 2 bits (transparent)

                // match the colour ID to its actual colour using the palette 
                int r; int g; int b;
                if (palette) {
                    set_colour_from_palette(&r, &g, &b, colour_ID, obp1_);
                }
                else {
                    set_colour_from_palette(&r, &g, &b, colour_ID, obp0_);
                }

                SDL_SetRenderDrawColor(renderer_, r, g, b, SDL_ALPHA_OPAQUE);

                uint8_t x_pixel = x_pos + (7 - bit_number); // 7th bit is leftmost pixel

                if (x_pixel >= 0 || x_pixel <= SCREEN_WIDTH) {
                    SDL_RenderDrawPoint(renderer_, x_pixel, ly_);
                }
            }
        }
    }
}

void PPU::draw_scanline()
{
    /*  Draw the scanline during mode 3 of the PPU. In a hardware accurate GameBoy emulator, this should draw one
        pixel per cycle; this method will draw the whole scanline at once at the beginning of mode 3 */
    screen_cleared_ = false;

    draw_bg_window();
    draw_sprites(); 
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
    lcdc_enable_ = (new_lcdc & (1 << 7)) >> 7;
    window_tile_map = (new_lcdc & (1 << 6)) >> 6;
    window_enable = (new_lcdc & (1 << 5)) >> 5;
    bg_window_tile_data = (new_lcdc & (1 << 4)) >> 4;
    bg_tile_map = (new_lcdc & (1 << 3)) >> 3;
    obj_size = (new_lcdc & (1 << 2)) >> 2;
    obj_enable = (new_lcdc & (1 << 1)) >> 1;
    enable_priority = (new_lcdc & (1 << 0)) >> 0;
}