#include "cartridge.h"
#include <cstdint>
#include <fstream>
#include <iostream>

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
}