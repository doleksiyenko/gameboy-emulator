#include "bootrom.h"
#include <iostream>
#include <cstdint>
#include <fstream>
#include <string>

BootROM::BootROM() 
{
}

void BootROM::load_bootrom_file(std::string bootrom_file)
{
    /* Load the boot ROM into the bootrom array from the location bootrom_file, which is a file path to a .bin file */

    std::ifstream bootrom_reader;
    bootrom_reader.open(bootrom_file, std::ios::binary); // open as a binary file, bootrom given as .bin

    if (!bootrom_reader) {
        // error opening the bootrom file
        std::cout << "Error: could not open the provided bootrom file." << std::endl;
        exit(-1);
    }

    // successfully opened file, read stream of bytes into the bootrom array
    uint8_t array_loc = 0; // the bootrom array is only 256 bytes long, so we can use a uint8_t here
    uint8_t byte;

    while (bootrom_reader.read(reinterpret_cast<char*>(&byte), sizeof(byte))) {
        bootrom_[array_loc] = byte;
        array_loc++;
    }

    bootrom_reader.close();

}

uint8_t BootROM::read(uint16_t address)
{
    return bootrom_[address];
}

uint8_t BootROM::read_bank()
{
    return bank_;
}

void BootROM::write_bank(uint8_t value)
{
    bank_ = value;
}

