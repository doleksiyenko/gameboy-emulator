#include <cpu.h>

CPU::CPU() {
    // create opcode_lookup through an initializer list, with function pointers pointing to each of the
    // instruction implementations

    opcode_lookup = {
        &CPU::NOP,          &CPU::LD_BC_d16,    &CPU::LD_BC_m_A,    &CPU::INC_BC,
        &CPU::INC_B,        &CPU::DEC_B,        &CPU::LD_B_d8,      &CPU::RLCA,
        &CPU::LD_a16_m_SP,  &CPU::ADD_HL_BC,    &CPU::LD_A_BC_m,    &CPU::DEC_BC,
        &CPU::INC_C,        &CPU::DEC_C,        &CPU::LD_C_d8,      &CPU::RRCA,
        &CPU::STOP_0,       &CPU::LD_DE_d16,    &CPU::LD_DE_m_A,    &CPU::INC_DE,
        &CPU::INC_D,        &CPU::DEC_D,        &CPU::LD_D_d8,      &CPU::RLA,
        &CPU::JR_s8,        &CPU::ADD_HL_DE,    &CPU::LD_A_DE_m,    &CPU::DEC_DE,
        &CPU::INC_E,        &CPU::DEC_E,        &CPU::LD_E_d8,      &CPU::RRA,
        &CPU::JR_NZ_s8,     &CPU::LD_HL_d16,    &CPU::LD_HLp_m_A,   &CPU::INC_HL,
        &CPU::INC_H,        &CPU::DEC_H,        &CPU::LD_H_d8,      &CPU::DAA,
        &CPU::JR_Z_s8,      &CPU::ADD_HL_HL,    &CPU::LD_A_HLp_m,   &CPU::DEC_HL,
        &CPU::INC_L,        &CPU::DEC_L,        &CPU::LD_L_d8,      &CPU::CPL,
        &CPU::JR_NC_s8,     &CPU::LD_SP_d16,    &CPU::LD_HLm_m_A,   &CPU::INC_SP,
        &CPU::INC_HL_m,     &CPU::DEC_HL_m,     &CPU::LD_HL_m_d8,   &CPU::SCF,
        &CPU::JR_C_s8,      &CPU::ADD_HL_SP,    &CPU::LD_A_HLm_m,   &CPU::DEC_SP,
        &CPU::INC_A,        &CPU::DEC_A,        &CPU::LD_A_d8,      &CPU::CCF,
        &CPU::LD_B_B,       &CPU::LD_B_C,       &CPU::LD_B_D,       &CPU::LD_B_E,
        &CPU::LD_B_H,       &CPU::LD_B_L,       &CPU::LD_B_HL_m,    &CPU::LD_B_A,
        &CPU::LD_C_B,       &CPU::LD_C_C,       &CPU::LD_C_D,       &CPU::LD_C_E,
        &CPU::LD_C_H,       &CPU::LD_C_L,       &CPU::LD_C_HL_m,    &CPU::LD_C_A,
        &CPU::LD_D_B,       &CPU::LD_D_C,       &CPU::LD_D_D,       &CPU::LD_D_E,
        &CPU::LD_D_H,       &CPU::LD_D_L,       &CPU::LD_D_HL_m,    &CPU::LD_D_A,
        &CPU::LD_E_B,       &CPU::LD_E_C,       &CPU::LD_E_D,       &CPU::LD_E_E,
        &CPU::LD_E_H,       &CPU::LD_E_L,       &CPU::LD_E_HL_m,    &CPU::LD_E_A,
        &CPU::LD_H_B,       &CPU::LD_H_C,       &CPU::LD_H_D,       &CPU::LD_H_E,
        &CPU::LD_H_H,       &CPU::LD_H_L,       &CPU::LD_H_HL_m,    &CPU::LD_H_A,
        &CPU::LD_L_B,       &CPU::LD_L_C,       &CPU::LD_L_D,       &CPU::LD_L_E,
        &CPU::LD_L_H,       &CPU::LD_L_L,       &CPU::LD_L_HL_m,    &CPU::LD_L_A,
        &CPU::LD_HL_m_B,    &CPU::LD_HL_m_C,    &CPU::LD_HL_m_D,    &CPU::LD_HL_m_E,
        &CPU::LD_HL_m_H,    &CPU::LD_HL_m_L,    &CPU::HALT,         &CPU::LD_HL_m_A,
        &CPU::LD_A_B,       &CPU::LD_A_C,       &CPU::LD_A_D,       &CPU::LD_A_E,
        &CPU::LD_A_H,       &CPU::LD_A_L,       &CPU::LD_A_HL_m,    &CPU::LD_A_A,
        &CPU::ADD_A_B,      &CPU::ADD_A_C,      &CPU::ADD_A_D,      &CPU::ADD_A_E,
        &CPU::ADD_A_H,      &CPU::ADD_A_L,      &CPU::ADD_A_HL_m,   &CPU::ADD_A_A,         
        &CPU::ADC_A_B,      &CPU::ADC_A_C,      &CPU::ADC_A_D,      &CPU::ADC_A_E,
        &CPU::ADC_A_H,      &CPU::ADC_A_L,      &CPU::ADC_A_HL_m,   &CPU::ADC_A_A,         
        &CPU::SUB_B,        &CPU::SUB_C,        &CPU::SUB_D,        &CPU::SUB_E,
        &CPU::SUB_H,        &CPU::SUB_L,        &CPU::SUB_HL_m,     &CPU::SUB_A,         
        &CPU::SBC_A_B,      &CPU::SBC_A_C,      &CPU::SBC_A_D,      &CPU::SBC_A_E,
        &CPU::SBC_A_H,      &CPU::SBC_A_L,      &CPU::SBC_A_HL_m,   &CPU::SBC_A_A,         
        &CPU::AND_B,        &CPU::AND_C,        &CPU::AND_D,        &CPU::AND_E,
        &CPU::AND_H,        &CPU::AND_L,        &CPU::AND_HL_m,     &CPU::AND_A,         
        &CPU::XOR_B,        &CPU::XOR_C,        &CPU::XOR_D,        &CPU::XOR_E,
        &CPU::XOR_H,        &CPU::XOR_L,        &CPU::XOR_HL_m,     &CPU::XOR_A,         
        &CPU::OR_B,         &CPU::OR_C,         &CPU::OR_D,         &CPU::OR_E,
        &CPU::OR_H,         &CPU::OR_L,         &CPU::OR_HL_m,      &CPU::OR_A,         
        &CPU::CP_B,         &CPU::CP_C,         &CPU::CP_D,         &CPU::CP_E,
        &CPU::CP_H,         &CPU::CP_L,         &CPU::CP_HL_m,      &CPU::CP_A,         
        &CPU::RET_NZ,       &CPU::POP_BC,       &CPU::JP_NZ_a16,    &CPU::JP_a16,
        &CPU::CALL_NZ_a16,  &CPU::PUSH_BC,      &CPU::ADD_A_d8,     &CPU::RST_0,
        &CPU::RET_Z,        &CPU::RET,          &CPU::JP_Z_a16,     &CPU::INVALID,
        &CPU::CALL_Z_a16,   &CPU::CALL_a16,     &CPU::ADC_A_d8,     &CPU::RST_1,
        &CPU::RET_NC,       &CPU::POP_DE,       &CPU::JP_NC_a16,    &CPU::INVALID,
        &CPU::CALL_NC_a16,  &CPU::PUSH_DE,      &CPU::SUB_d8,       &CPU::RST_2,
        &CPU::RET_C,        &CPU::RETI,         &CPU::JP_C_a16,     &CPU::INVALID,
        &CPU::CALL_C_a16,   &CPU::INVALID,      &CPU::SBC_A_d8,     &CPU::RST_3,
        &CPU::LD_a8_m_A,    &CPU::POP_HL,       &CPU::LD_C_m_A,     &CPU::INVALID,
        &CPU::INVALID,      &CPU::PUSH_HL,      &CPU::AND_d8,       &CPU::RST_4,
        &CPU::ADD_SP_s8,    &CPU::JP_HL,        &CPU::LD_a16_m_A,   &CPU::INVALID,
        &CPU::INVALID,      &CPU::INVALID,      &CPU::XOR_d8,       &CPU::RST_5,
        &CPU::LD_A_a8_m,    &CPU::POP_AF,       &CPU::LD_A_C_m,     &CPU::DI,
        &CPU::INVALID,      &CPU::PUSH_AF,      &CPU::OR_d8,        &CPU::RST_6,
        &CPU::LD_HL_SP_s8,  &CPU::LD_SP_HL,     &CPU::LD_A_a16_m,   &CPU::EI,
        &CPU::INVALID,      &CPU::INVALID,      &CPU::CP_d8,        &CPU::RST_7
    };

    // create a lookup for all of the implementations of 16-bit instructions (function pointers)




}