#include "gameboy.h"
#include <iostream>

int main(int argc, char* argv[]) 
{
    if (argc != 2) {
        std::cout << "Please provide path to BOOTROM .bin file, and to the ROM file." << std::endl;
        exit(-1);
    }
    else if (argc != 3) {
        std::cout << "Please provide path to ROM file." << std::endl;
        exit(-1);
    }

    GameBoy gameboy{argv[1], argv[2]};
    gameboy.run();
    return 0;
}