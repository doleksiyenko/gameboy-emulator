#include "gameboy.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Please provide path to ROM file." << std::endl;
        exit(-1);
    }

    GameBoy gameboy;
    gameboy.run();
    return 0;
}