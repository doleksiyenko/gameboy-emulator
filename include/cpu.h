#ifndef CPU_H
#define CPU_H

#include <vector>
#include <cstdint>


class Bus; // forward declaration

class CPU {
    public:
        CPU();
        void cycle();
        void connect_bus(Bus* bus);
    private:
        // 16 bit registers
        uint16_t pc_; // program counter
        uint16_t sp_; // stack pointer
        uint16_t af_; // accumulator + flags
        uint16_t bc_; uint16_t de_; uint16_t hl_; // general-purpose registers (8 bit halves)
        uint8_t ir_; // instruction register
        uint8_t ie_; // interrupt enable

        Bus* bus_; // create a reference to the bus connecting all the hardware components together
        uint8_t read(uint16_t address);
        void write(uint16_t address, uint8_t value); 

        // -- flags -- 
        // the lower 8 bits of the AF register holds flags that can be influenced by an instruction
        // for use in accessing bits in af_
        enum flags {
            Z = (1 << 7), // Zero flag - result of operation is 0
            N = (1 << 6), // Subtraction flag 
            H = (1 << 5), // Half carry flag
            C = (1 << 4) // Carry flag
        };

        void set_flag(flags flag, bool val);

        struct Instruction {
            uint8_t (CPU::*opcode_function)(void) = nullptr; // function pointer to the atomic implementation of the instruction behaviour
            uint8_t t_cycles = 0; // each instruction takes a number of cycles to complete
        };

        std::vector<Instruction> opcode_lookup;
        std::vector<Instruction> cb_opcode_lookup;

        uint8_t t_cycles_delay; // the number of system clock ticks that an instruction requires to complete

        //              ------------------ opcode functions ------------------
        // these are written in the order of instructions (i.e. instruction 0x0, 0x1, 0x2...)
        // helpful interactive reference: https://meganesu.github.io/generate-gb-opcodes/
        // 8 bit instructions
        uint8_t NOP();          uint8_t LD_BC_d16();    uint8_t LD_BC_m_A();    uint8_t INC_BC();
        uint8_t INC_B();        uint8_t DEC_B();        uint8_t LD_B_d8();      uint8_t RLCA();
        uint8_t LD_a16_m_SP();  uint8_t ADD_HL_BC();    uint8_t LD_A_BC_m();    uint8_t DEC_BC();
        uint8_t INC_C();        uint8_t DEC_C();        uint8_t LD_C_d8();      uint8_t RRCA();
        uint8_t STOP_0();       uint8_t LD_DE_d16();    uint8_t LD_DE_m_A();    uint8_t INC_DE();
        uint8_t INC_D();        uint8_t DEC_D();        uint8_t LD_D_d8();      uint8_t RLA();
        uint8_t JR_s8();        uint8_t ADD_HL_DE();    uint8_t LD_A_DE_m();    uint8_t DEC_DE();
        uint8_t INC_E();        uint8_t DEC_E();        uint8_t LD_E_d8();      uint8_t RRA();
        uint8_t JR_NZ_s8();     uint8_t LD_HL_d16();    uint8_t LD_HLp_m_A();   uint8_t INC_HL();
        uint8_t INC_H();        uint8_t DEC_H();        uint8_t LD_H_d8();      uint8_t DAA();
        uint8_t JR_Z_s8();      uint8_t ADD_HL_HL();    uint8_t LD_A_HLp_m();   uint8_t DEC_HL();
        uint8_t INC_L();        uint8_t DEC_L();        uint8_t LD_L_d8();      uint8_t CPL();
        uint8_t JR_NC_s8();     uint8_t LD_SP_d16();    uint8_t LD_HLm_m_A();   uint8_t INC_SP();
        uint8_t INC_HL_m();     uint8_t DEC_HL_m();     uint8_t LD_HL_m_d8();   uint8_t SCF();
        uint8_t JR_C_s8();      uint8_t ADD_HL_SP();    uint8_t LD_A_HLm_m();   uint8_t DEC_SP();
        uint8_t INC_A();        uint8_t DEC_A();        uint8_t LD_A_d8();      uint8_t CCF();

        uint8_t LD_B_B();       uint8_t LD_B_C();       uint8_t LD_B_D();       uint8_t LD_B_E();
        uint8_t LD_B_H();       uint8_t LD_B_L();       uint8_t LD_B_HL_m();    uint8_t LD_B_A();
        uint8_t LD_C_B();       uint8_t LD_C_C();       uint8_t LD_C_D();       uint8_t LD_C_E();
        uint8_t LD_C_H();       uint8_t LD_C_L();       uint8_t LD_C_HL_m();    uint8_t LD_C_A();
        uint8_t LD_D_B();       uint8_t LD_D_C();       uint8_t LD_D_D();       uint8_t LD_D_E();
        uint8_t LD_D_H();       uint8_t LD_D_L();       uint8_t LD_D_HL_m();    uint8_t LD_D_A();
        uint8_t LD_E_B();       uint8_t LD_E_C();       uint8_t LD_E_D();       uint8_t LD_E_E();
        uint8_t LD_E_H();       uint8_t LD_E_L();       uint8_t LD_E_HL_m();    uint8_t LD_E_A();
        uint8_t LD_H_B();       uint8_t LD_H_C();       uint8_t LD_H_D();       uint8_t LD_H_E();
        uint8_t LD_H_H();       uint8_t LD_H_L();       uint8_t LD_H_HL_m();    uint8_t LD_H_A();
        uint8_t LD_L_B();       uint8_t LD_L_C();       uint8_t LD_L_D();       uint8_t LD_L_E();
        uint8_t LD_L_H();       uint8_t LD_L_L();       uint8_t LD_L_HL_m();    uint8_t LD_L_A();
        uint8_t LD_HL_m_B();    uint8_t LD_HL_m_C();    uint8_t LD_HL_m_D();    uint8_t LD_HL_m_E();
        uint8_t LD_HL_m_H();    uint8_t LD_HL_m_L();    uint8_t HALT();         uint8_t LD_HL_m_A();
        uint8_t LD_A_B();       uint8_t LD_A_C();       uint8_t LD_A_D();       uint8_t LD_A_E();
        uint8_t LD_A_H();       uint8_t LD_A_L();       uint8_t LD_A_HL_m();    uint8_t LD_A_A();

        uint8_t ADD_A_B();      uint8_t ADD_A_C();      uint8_t ADD_A_D();      uint8_t ADD_A_E();
        uint8_t ADD_A_H();      uint8_t ADD_A_L();      uint8_t ADD_A_HL_m();   uint8_t ADD_A_A();         
        uint8_t ADC_A_B();      uint8_t ADC_A_C();      uint8_t ADC_A_D();      uint8_t ADC_A_E();
        uint8_t ADC_A_H();      uint8_t ADC_A_L();      uint8_t ADC_A_HL_m();   uint8_t ADC_A_A();         
        uint8_t SUB_B();        uint8_t SUB_C();        uint8_t SUB_D();        uint8_t SUB_E();
        uint8_t SUB_H();        uint8_t SUB_L();        uint8_t SUB_HL_m();     uint8_t SUB_A();         
        uint8_t SBC_A_B();      uint8_t SBC_A_C();      uint8_t SBC_A_D();      uint8_t SBC_A_E();
        uint8_t SBC_A_H();      uint8_t SBC_A_L();      uint8_t SBC_A_HL_m();   uint8_t SBC_A_A();         
        uint8_t AND_B();        uint8_t AND_C();        uint8_t AND_D();        uint8_t AND_E();
        uint8_t AND_H();        uint8_t AND_L();        uint8_t AND_HL_m();     uint8_t AND_A();         
        uint8_t XOR_B();        uint8_t XOR_C();        uint8_t XOR_D();        uint8_t XOR_E();
        uint8_t XOR_H();        uint8_t XOR_L();        uint8_t XOR_HL_m();     uint8_t XOR_A();         
        uint8_t OR_B();         uint8_t OR_C();         uint8_t OR_D();         uint8_t OR_E();
        uint8_t OR_H();         uint8_t OR_L();         uint8_t OR_HL_m();      uint8_t OR_A();         
        uint8_t CP_B();         uint8_t CP_C();         uint8_t CP_D();         uint8_t CP_E();
        uint8_t CP_H();         uint8_t CP_L();         uint8_t CP_HL_m();      uint8_t CP_A();         

        uint8_t RET_NZ();       uint8_t POP_BC();       uint8_t JP_NZ_a16();    uint8_t JP_a16();
        uint8_t CALL_NZ_a16();  uint8_t PUSH_BC();      uint8_t ADD_A_d8();     uint8_t RST_0();
        uint8_t RET_Z();        uint8_t RET();          uint8_t JP_Z_a16();     /* invalid */;
        uint8_t CALL_Z_a16();   uint8_t CALL_a16();     uint8_t ADC_A_d8();     uint8_t RST_1();
        uint8_t RET_NC();       uint8_t POP_DE();       uint8_t JP_NC_a16();    /* invalid */;
        uint8_t CALL_NC_a16();  uint8_t PUSH_DE();      uint8_t SUB_d8();       uint8_t RST_2();
        uint8_t RET_C();        uint8_t RETI();         uint8_t JP_C_a16();     /* invalid */;
        uint8_t CALL_C_a16();   /* invalid */;          uint8_t SBC_A_d8();     uint8_t RST_3();
        uint8_t LD_a8_m_A();    uint8_t POP_HL();       uint8_t LD_C_m_A();     /* invalid */;
        /* invalid */;          uint8_t PUSH_HL();      uint8_t AND_d8();       uint8_t RST_4();
        uint8_t ADD_SP_s8();    uint8_t JP_HL();        uint8_t LD_a16_m_A();   /* invalid */;
        /* invalid */;          /* invalid */;          uint8_t XOR_d8();       uint8_t RST_5();
        uint8_t LD_A_a8_m();    uint8_t POP_AF();       uint8_t LD_A_C_m();     uint8_t DI();
        /* invalid */;          uint8_t PUSH_AF();      uint8_t OR_d8();        uint8_t RST_6();
        uint8_t LD_HL_SP_s8();  uint8_t LD_SP_HL();     uint8_t LD_A_a16_m();   uint8_t EI();
        /* invalid */;          /* invalid */           uint8_t CP_d8();        uint8_t RST_7();


        // 16 bit instructions
        uint8_t RLC_B();        uint8_t RLC_C();        uint8_t RLC_D();        uint8_t RLC_E();
        uint8_t RLC_H();        uint8_t RLC_L();        uint8_t RLC_HL_m();     uint8_t RLC_A();
        uint8_t RRC_B();        uint8_t RRC_C();        uint8_t RRC_D();        uint8_t RRC_E();
        uint8_t RRC_H();        uint8_t RRC_L();        uint8_t RRC_HL_m();     uint8_t RRC_A();
        uint8_t RL_B();         uint8_t RL_C();         uint8_t RL_D();         uint8_t RL_E();
        uint8_t RL_H();         uint8_t RL_L();         uint8_t RL_HL_m();      uint8_t RL_A();
        uint8_t RR_B();         uint8_t RR_C();         uint8_t RR_D();         uint8_t RR_E();
        uint8_t RR_H();         uint8_t RR_L();         uint8_t RR_HL_m();      uint8_t RR_A();
        uint8_t SLA_B();        uint8_t SLA_C();        uint8_t SLA_D();        uint8_t SLA_E();
        uint8_t SLA_H();        uint8_t SLA_L();        uint8_t SLA_HL_m();     uint8_t SLA_A();
        uint8_t SRC_B();        uint8_t SRC_C();        uint8_t SRC_D();        uint8_t SRC_E();
        uint8_t SRC_H();        uint8_t SRC_L();        uint8_t SRC_HL_m();     uint8_t SRC_A();
        uint8_t SWAP_B();       uint8_t SWAP_C();       uint8_t SWAP_D();       uint8_t SWAP_E();
        uint8_t SWAP_H();       uint8_t SWAP_L();       uint8_t SWAP_HL_m();    uint8_t SWAP_A();
        uint8_t SRL_B();        uint8_t SRL_C();        uint8_t SRL_D();        uint8_t SRL_E();
        uint8_t SRL_H();        uint8_t SRL_L();        uint8_t SRL_HL_m();     uint8_t SRL_A();
        uint8_t BIT_0_B();      uint8_t BIT_0_C();      uint8_t BIT_0_D();      uint8_t BIT_0_E();
        uint8_t BIT_0_H();      uint8_t BIT_0_L();      uint8_t BIT_0_HL_m();   uint8_t BIT_0_A();         
        uint8_t BIT_1_B();      uint8_t BIT_1_C();      uint8_t BIT_1_D();      uint8_t BIT_1_E();
        uint8_t BIT_1_H();      uint8_t BIT_1_L();      uint8_t BIT_1_HL_m();   uint8_t BIT_1_A();         
        uint8_t BIT_2_B();      uint8_t BIT_2_C();      uint8_t BIT_2_D();      uint8_t BIT_2_E();
        uint8_t BIT_2_H();      uint8_t BIT_2_L();      uint8_t BIT_2_HL_m();   uint8_t BIT_2_A();         
        uint8_t BIT_3_B();      uint8_t BIT_3_C();      uint8_t BIT_3_D();      uint8_t BIT_3_E();
        uint8_t BIT_3_H();      uint8_t BIT_3_L();      uint8_t BIT_3_HL_m();   uint8_t BIT_3_A();         
        uint8_t BIT_4_B();      uint8_t BIT_4_C();      uint8_t BIT_4_D();      uint8_t BIT_4_E();
        uint8_t BIT_4_H();      uint8_t BIT_4_L();      uint8_t BIT_4_HL_m();   uint8_t BIT_4_A();         
        uint8_t BIT_5_B();      uint8_t BIT_5_C();      uint8_t BIT_5_D();      uint8_t BIT_5_E();
        uint8_t BIT_5_H();      uint8_t BIT_5_L();      uint8_t BIT_5_HL_m();   uint8_t BIT_5_A();         
        uint8_t BIT_6_B();      uint8_t BIT_6_C();      uint8_t BIT_6_D();      uint8_t BIT_6_E();
        uint8_t BIT_6_H();      uint8_t BIT_6_L();      uint8_t BIT_6_HL_m();   uint8_t BIT_6_A();         
        uint8_t BIT_7_B();      uint8_t BIT_7_C();      uint8_t BIT_7_D();      uint8_t BIT_7_E();
        uint8_t BIT_7_H();      uint8_t BIT_7_L();      uint8_t BIT_7_HL_m();   uint8_t BIT_7_A();         
        uint8_t RES_0_B();      uint8_t RES_0_C();      uint8_t RES_0_D();      uint8_t RES_0_E();
        uint8_t RES_0_H();      uint8_t RES_0_L();      uint8_t RES_0_HL_m();   uint8_t RES_0_A();         
        uint8_t RES_1_B();      uint8_t RES_1_C();      uint8_t RES_1_D();      uint8_t RES_1_E();
        uint8_t RES_1_H();      uint8_t RES_1_L();      uint8_t RES_1_HL_m();   uint8_t RES_1_A();         
        uint8_t RES_2_B();      uint8_t RES_2_C();      uint8_t RES_2_D();      uint8_t RES_2_E();
        uint8_t RES_2_H();      uint8_t RES_2_L();      uint8_t RES_2_HL_m();   uint8_t RES_2_A();         
        uint8_t RES_3_B();      uint8_t RES_3_C();      uint8_t RES_3_D();      uint8_t RES_3_E();
        uint8_t RES_3_H();      uint8_t RES_3_L();      uint8_t RES_3_HL_m();   uint8_t RES_3_A();         
        uint8_t RES_4_B();      uint8_t RES_4_C();      uint8_t RES_4_D();      uint8_t RES_4_E();
        uint8_t RES_4_H();      uint8_t RES_4_L();      uint8_t RES_4_HL_m();   uint8_t RES_4_A();         
        uint8_t RES_5_B();      uint8_t RES_5_C();      uint8_t RES_5_D();      uint8_t RES_5_E();
        uint8_t RES_5_H();      uint8_t RES_5_L();      uint8_t RES_5_HL_m();   uint8_t RES_5_A();         
        uint8_t RES_6_B();      uint8_t RES_6_C();      uint8_t RES_6_D();      uint8_t RES_6_E();
        uint8_t RES_6_H();      uint8_t RES_6_L();      uint8_t RES_6_HL_m();   uint8_t RES_6_A();         
        uint8_t RES_7_B();      uint8_t RES_7_C();      uint8_t RES_7_D();      uint8_t RES_7_E();
        uint8_t RES_7_H();      uint8_t RES_7_L();      uint8_t RES_7_HL_m();   uint8_t RES_7_A();         
        uint8_t SET_0_B();      uint8_t SET_0_C();      uint8_t SET_0_D();      uint8_t SET_0_E();
        uint8_t SET_0_H();      uint8_t SET_0_L();      uint8_t SET_0_HL_m();   uint8_t SET_0_A();         
        uint8_t SET_1_B();      uint8_t SET_1_C();      uint8_t SET_1_D();      uint8_t SET_1_E();
        uint8_t SET_1_H();      uint8_t SET_1_L();      uint8_t SET_1_HL_m();   uint8_t SET_1_A();         
        uint8_t SET_2_B();      uint8_t SET_2_C();      uint8_t SET_2_D();      uint8_t SET_2_E();
        uint8_t SET_2_H();      uint8_t SET_2_L();      uint8_t SET_2_HL_m();   uint8_t SET_2_A();         
        uint8_t SET_3_B();      uint8_t SET_3_C();      uint8_t SET_3_D();      uint8_t SET_3_E();
        uint8_t SET_3_H();      uint8_t SET_3_L();      uint8_t SET_3_HL_m();   uint8_t SET_3_A();         
        uint8_t SET_4_B();      uint8_t SET_4_C();      uint8_t SET_4_D();      uint8_t SET_4_E();
        uint8_t SET_4_H();      uint8_t SET_4_L();      uint8_t SET_4_HL_m();   uint8_t SET_4_A();         
        uint8_t SET_5_B();      uint8_t SET_5_C();      uint8_t SET_5_D();      uint8_t SET_5_E();
        uint8_t SET_5_H();      uint8_t SET_5_L();      uint8_t SET_5_HL_m();   uint8_t SET_5_A();         
        uint8_t SET_6_B();      uint8_t SET_6_C();      uint8_t SET_6_D();      uint8_t SET_6_E();
        uint8_t SET_6_H();      uint8_t SET_6_L();      uint8_t SET_6_HL_m();   uint8_t SET_6_A();         
        uint8_t SET_7_B();      uint8_t SET_7_C();      uint8_t SET_7_D();      uint8_t SET_7_E();
        uint8_t SET_7_H();      uint8_t SET_7_L();      uint8_t SET_7_HL_m();   uint8_t SET_7_A();         

        uint8_t INVALID(); // opcode for any undefined behaviour
        uint8_t CB_PREFIX();

};

#endif