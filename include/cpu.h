#ifndef CPU_H
#define CPU_H

#include <array>
#include <cstdint>
#include <string>

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
        uint16_t bc_; uint16_t de_; uint16_t hl_; // general-purpose registers (8 bit halves)
        uint8_t ir_; // instruction register
        uint8_t ie_; // interrupt enable

        // --- opcodes --- reference sheet: https://www.pastraiser.com/cpu/gameboy/gameboy_opcodes.html
        uint8_t NOP(); uint8_t LD(); uint8_t INC();
        uint8_t DEC(); uint8_t RLCA(); uint8_t ADD();
        uint8_t RRCA(); uint8_t STOP(); uint8_t RLA();
        uint8_t JR(); uint8_t RRA();

        struct INSTRUCTION {
            std::string instruction_name; // name of the instruction
            uint8_t(CPU::*opcode)() = nullptr; // create a function pointer to one of the opcodes;
            uint8_t cycles = 0; // number of cycles that the instruction requires to complete
        };

        std::array<INSTRUCTION, 16 * 16> opcode_lookup;
        std::array<INSTRUCTION, 16 * 16> cb_opcode_lookup;
};

#endif