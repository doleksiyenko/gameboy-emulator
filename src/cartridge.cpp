#include "cartridge.h"
#include "mbc/mbc1.h"
#include <cstdint>
#include <fstream>
#include <iostream>
#include <memory>

void Cartridge::load_cartridge_from_file(std::string cartridge_file)
{
    /* Read the Cartridge from the specified .gb file,  */
    std::ifstream cartridge_reader;
    cartridge_reader.open(cartridge_file, std::ios::binary); // open as a binary file, cartridge given as .bin

    if (!cartridge_reader) {
        // error opening the cartridge file
        std::cout << "Error: could not open the provided cartridge file." << std::endl;
        exit(-1);
    }

    // successfully opened file, read stream of bytes into the cartridge array
    uint8_t byte;

    // read the header of the cartridge and save the information
    while (cartridge_reader.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        cartridge_.push_back(byte);
    }

    cartridge_reader.close();

    // to be a valid cartridge, the number of bytes must be divisible by the bank size, 16KB
    if (cartridge_.size() % (16 * 1024) != 0) {
        std::cout << "Error: invalid cartridge size." << std::endl;
        exit(-1);
    }

    // read any required information from the cartridge header
    mbc_header_val_ = cartridge_[0x0147]; // mbc mode found in catridge header. 0x0147 contains the cartridge type, and indicates how the Cartridge is organized

    // create the appropriate MBC chip for the cartridge
    switch (mbc_header_val_) {
        case 0x0:
            // no mbc chip
            break;
        case 0x1:
            // mbc1 chip
            mbc_ = std::make_unique<MBC1>(cartridge_);
            break;
        default:
            break;
    }

    std::cout << "MBC type: " << (int)mbc_header_val_ << '\n'; 
    std::cout << "Cartridge size (bytes): " << cartridge_.size() << std::endl;
}

uint8_t Cartridge::read(uint16_t address)
{
    // if there is no MBC, then there is no swapping of banks. We can read directly from the 32 KiB ROM
    if (mbc_header_val_ == 0x0) {
        return cartridge_[address];
    }
    else {
        // we have an mbc, read from it
        mbc_->read(address);
    }
}