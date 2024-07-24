#include "gameboy.h"
#include <iostream>
#include <ostream>

int main(int argc, char* argv[]) 
{
    if (argc < 2) {
        std::cout << "Please provide path to BOOTROM .bin file, and path to the ROM file." << std::endl;
        exit(-1);
    }
    else if (argc != 3) {
        std::cout << "Please provide path to ROM file." << std::endl;
        exit(-1);
    }
    else if (argc == 3) {
        std::cout << "Running game: " << argv[2] << std::endl;
    }
    else {
        std::cout << "Invalid number of arguments. Only specify BOOTROM .bin file, and path to the ROM file." << std::endl;
    }

    GameBoy gameboy{argv[1], argv[2]};
    gameboy.run();
    return 0;
}