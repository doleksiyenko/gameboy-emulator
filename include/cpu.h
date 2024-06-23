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
        uint8_t nop(); uint8_t ldrr(); uint8_t ldrn(); uint8_t ldrhl();
        uint8_t ldr(); uint8_t ldhlr(); uint8_t ldhln(); uint8_t ldabc();
        uint8_t ldade(); uint8_t ldbca(); uint8_t lddea(); uint8_t ldann();
};

#endif