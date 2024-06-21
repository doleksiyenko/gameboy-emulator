#ifndef CPU_H
#define CPU_H

#include <cstdint>

class CPU {
    public:
        CPU();
        void fetch();
        void execute();

    private:
        // 16 bit registers
        uint16_t pc_; // program counter
        uint16_t sp_; // stack pointer
        uint16_t af_; // accumulator + flags
        uint16_t bc_; uint16_t de_; uint16_t hl_;

        // opcodes
};

#endif