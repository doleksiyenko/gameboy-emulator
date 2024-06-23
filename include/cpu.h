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
        uint8_t ldnna(); uint8_t ldhac(); uint8_t ldhca(); uint8_t ldhan();
        uint8_t ldhna(); uint8_t ldahl_m(); uint8_t ldhla_m(); uint8_t ldahl_p();
        uint8_t ldhla_p(); uint8_t ldrrnn(); uint8_t ldnnsp(); uint8_t ldsphl();
        uint8_t pushrr(); uint8_t poprr(); uint8_t ldhlsp_e(); 
};

#endif