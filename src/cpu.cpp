#include <cpu.h>

CPU::CPU() {
  // create opcode_lookup through an initializer list, with function pointers
  // pointing to each of the instruction implementations

  opcode_lookup = {
      &CPU::NOP,         &CPU::LD_BC_d16, &CPU::LD_BC_m_A,  &CPU::INC_BC,
      &CPU::INC_B,       &CPU::DEC_B,     &CPU::LD_B_d8,    &CPU::RLCA,
      &CPU::LD_a16_m_SP, &CPU::ADD_HL_BC, &CPU::LD_A_BC_m,  &CPU::DEC_BC,
      &CPU::INC_C,       &CPU::DEC_C,     &CPU::LD_C_d8,    &CPU::RRCA,
      &CPU::STOP_0,      &CPU::LD_DE_d16, &CPU::LD_DE_m_A,  &CPU::INC_DE,
      &CPU::INC_D,       &CPU::DEC_D,     &CPU::LD_D_d8,    &CPU::RLA,
      &CPU::JR_s8,       &CPU::ADD_HL_DE, &CPU::LD_A_DE_m,  &CPU::DEC_DE,
      &CPU::INC_E,       &CPU::DEC_E,     &CPU::LD_E_d8,    &CPU::RRA,
      &CPU::JR_NZ_s8,    &CPU::LD_HL_d16, &CPU::LD_HLp_m_A, &CPU::INC_HL,
      &CPU::INC_H,       &CPU::DEC_H,     &CPU::LD_H_d8,    &CPU::DAA,
      &CPU::JR_Z_s8,     &CPU::ADD_HL_HL, &CPU::LD_A_HLp_m, &CPU::DEC_HL,
      &CPU::INC_L,       &CPU::DEC_L,     &CPU::LD_L_d8,    &CPU::CPL,
      &CPU::JR_NC_s8,    &CPU::LD_SP_d16, &CPU::LD_HLm_m_A, &CPU::INC_SP,
      &CPU::INC_HL_m,    &CPU::DEC_HL_m,  &CPU::LD_HL_m_d8, &CPU::SCF,
      &CPU::JR_C_s8,     &CPU::ADD_HL_SP, &CPU::LD_A_HLm_m, &CPU::DEC_SP,
      &CPU::INC_A,       &CPU::DEC_A,     &CPU::LD_A_d8,    &CPU::CCF,
      &CPU::LD_B_B,      &CPU::LD_B_C,    &CPU::LD_B_D,     &CPU::LD_B_E,
      &CPU::LD_B_H,      &CPU::LD_B_L,    &CPU::LD_B_HL_m,  &CPU::LD_B_A,
      &CPU::LD_C_B,      &CPU::LD_C_C,    &CPU::LD_C_D,     &CPU::LD_C_E,
      &CPU::LD_C_H,      &CPU::LD_C_L,    &CPU::LD_C_HL_m,  &CPU::LD_C_A,
      &CPU::LD_D_B,      &CPU::LD_D_C,    &CPU::LD_D_D,     &CPU::LD_D_E,
      &CPU::LD_D_H,      &CPU::LD_D_L,    &CPU::LD_D_HL_m,  &CPU::LD_D_A,
      &CPU::LD_E_B,      &CPU::LD_E_C,    &CPU::LD_E_D,     &CPU::LD_E_E,
      &CPU::LD_E_H,      &CPU::LD_E_L,    &CPU::LD_E_HL_m,  &CPU::LD_E_A,
      &CPU::LD_H_B,      &CPU::LD_H_C,    &CPU::LD_H_D,     &CPU::LD_H_E,
      &CPU::LD_H_H,      &CPU::LD_H_L,    &CPU::LD_H_HL_m,  &CPU::LD_H_A,
      &CPU::LD_L_B,      &CPU::LD_L_C,    &CPU::LD_L_D,     &CPU::LD_L_E,
      &CPU::LD_L_H,      &CPU::LD_L_L,    &CPU::LD_L_HL_m,  &CPU::LD_L_A,
      &CPU::LD_HL_m_B,   &CPU::LD_HL_m_C, &CPU::LD_HL_m_D,  &CPU::LD_HL_m_E,
      &CPU::LD_HL_m_H,   &CPU::LD_HL_m_L, &CPU::HALT,       &CPU::LD_HL_m_A,
      &CPU::LD_A_B,      &CPU::LD_A_C,    &CPU::LD_A_D,     &CPU::LD_A_E,
      &CPU::LD_A_H,      &CPU::LD_A_L,    &CPU::LD_A_HL_m,  &CPU::LD_A_A,
      &CPU::ADD_A_B,     &CPU::ADD_A_C,   &CPU::ADD_A_D,    &CPU::ADD_A_E,
      &CPU::ADD_A_H,     &CPU::ADD_A_L,   &CPU::ADD_A_HL_m, &CPU::ADD_A_A,
      &CPU::ADC_A_B,     &CPU::ADC_A_C,   &CPU::ADC_A_D,    &CPU::ADC_A_E,
      &CPU::ADC_A_H,     &CPU::ADC_A_L,   &CPU::ADC_A_HL_m, &CPU::ADC_A_A,
      &CPU::SUB_B,       &CPU::SUB_C,     &CPU::SUB_D,      &CPU::SUB_E,
      &CPU::SUB_H,       &CPU::SUB_L,     &CPU::SUB_HL_m,   &CPU::SUB_A,
      &CPU::SBC_A_B,     &CPU::SBC_A_C,   &CPU::SBC_A_D,    &CPU::SBC_A_E,
      &CPU::SBC_A_H,     &CPU::SBC_A_L,   &CPU::SBC_A_HL_m, &CPU::SBC_A_A,
      &CPU::AND_B,       &CPU::AND_C,     &CPU::AND_D,      &CPU::AND_E,
      &CPU::AND_H,       &CPU::AND_L,     &CPU::AND_HL_m,   &CPU::AND_A,
      &CPU::XOR_B,       &CPU::XOR_C,     &CPU::XOR_D,      &CPU::XOR_E,
      &CPU::XOR_H,       &CPU::XOR_L,     &CPU::XOR_HL_m,   &CPU::XOR_A,
      &CPU::OR_B,        &CPU::OR_C,      &CPU::OR_D,       &CPU::OR_E,
      &CPU::OR_H,        &CPU::OR_L,      &CPU::OR_HL_m,    &CPU::OR_A,
      &CPU::CP_B,        &CPU::CP_C,      &CPU::CP_D,       &CPU::CP_E,
      &CPU::CP_H,        &CPU::CP_L,      &CPU::CP_HL_m,    &CPU::CP_A,
      &CPU::RET_NZ,      &CPU::POP_BC,    &CPU::JP_NZ_a16,  &CPU::JP_a16,
      &CPU::CALL_NZ_a16, &CPU::PUSH_BC,   &CPU::ADD_A_d8,   &CPU::RST_0,
      &CPU::RET_Z,       &CPU::RET,       &CPU::JP_Z_a16,   &CPU::INVALID,
      &CPU::CALL_Z_a16,  &CPU::CALL_a16,  &CPU::ADC_A_d8,   &CPU::RST_1,
      &CPU::RET_NC,      &CPU::POP_DE,    &CPU::JP_NC_a16,  &CPU::INVALID,
      &CPU::CALL_NC_a16, &CPU::PUSH_DE,   &CPU::SUB_d8,     &CPU::RST_2,
      &CPU::RET_C,       &CPU::RETI,      &CPU::JP_C_a16,   &CPU::INVALID,
      &CPU::CALL_C_a16,  &CPU::INVALID,   &CPU::SBC_A_d8,   &CPU::RST_3,
      &CPU::LD_a8_m_A,   &CPU::POP_HL,    &CPU::LD_C_m_A,   &CPU::INVALID,
      &CPU::INVALID,     &CPU::PUSH_HL,   &CPU::AND_d8,     &CPU::RST_4,
      &CPU::ADD_SP_s8,   &CPU::JP_HL,     &CPU::LD_a16_m_A, &CPU::INVALID,
      &CPU::INVALID,     &CPU::INVALID,   &CPU::XOR_d8,     &CPU::RST_5,
      &CPU::LD_A_a8_m,   &CPU::POP_AF,    &CPU::LD_A_C_m,   &CPU::DI,
      &CPU::INVALID,     &CPU::PUSH_AF,   &CPU::OR_d8,      &CPU::RST_6,
      &CPU::LD_HL_SP_s8, &CPU::LD_SP_HL,  &CPU::LD_A_a16_m, &CPU::EI,
      &CPU::INVALID,     &CPU::INVALID,   &CPU::CP_d8,      &CPU::RST_7};

  // create a lookup for all of the implementations of 16-bit instructions
  // (function pointers)
  cb_opcode_lookup = {
      &CPU::RLC_B,   &CPU::RLC_C,   &CPU::RLC_D,      &CPU::RLC_E,
      &CPU::RLC_H,   &CPU::RLC_L,   &CPU::RLC_HL_m,   &CPU::RLC_A,
      &CPU::RRC_B,   &CPU::RRC_C,   &CPU::RRC_D,      &CPU::RRC_E,
      &CPU::RRC_H,   &CPU::RRC_L,   &CPU::RRC_HL_m,   &CPU::RRC_A,
      &CPU::RL_B,    &CPU::RL_C,    &CPU::RL_D,       &CPU::RL_E,
      &CPU::RL_H,    &CPU::RL_L,    &CPU::RL_HL_m,    &CPU::RL_A,
      &CPU::RR_B,    &CPU::RR_C,    &CPU::RR_D,       &CPU::RR_E,
      &CPU::RR_H,    &CPU::RR_L,    &CPU::RR_HL_m,    &CPU::RR_A,
      &CPU::SLA_B,   &CPU::SLA_C,   &CPU::SLA_D,      &CPU::SLA_E,
      &CPU::SLA_H,   &CPU::SLA_L,   &CPU::SLA_HL_m,   &CPU::SLA_A,
      &CPU::SRC_B,   &CPU::SRC_C,   &CPU::SRC_D,      &CPU::SRC_E,
      &CPU::SRC_H,   &CPU::SRC_L,   &CPU::SRC_HL_m,   &CPU::SRC_A,
      &CPU::SWAP_B,  &CPU::SWAP_C,  &CPU::SWAP_D,     &CPU::SWAP_E,
      &CPU::SWAP_H,  &CPU::SWAP_L,  &CPU::SWAP_HL_m,  &CPU::SWAP_A,
      &CPU::SRL_B,   &CPU::SRL_C,   &CPU::SRL_D,      &CPU::SRL_E,
      &CPU::SRL_H,   &CPU::SRL_L,   &CPU::SRL_HL_m,   &CPU::SRL_A,
      &CPU::BIT_0_B, &CPU::BIT_0_C, &CPU::BIT_0_D,    &CPU::BIT_0_E,
      &CPU::BIT_0_H, &CPU::BIT_0_L, &CPU::BIT_0_HL_m, &CPU::BIT_0_A,
      &CPU::BIT_1_B, &CPU::BIT_1_C, &CPU::BIT_1_D,    &CPU::BIT_1_E,
      &CPU::BIT_1_H, &CPU::BIT_1_L, &CPU::BIT_1_HL_m, &CPU::BIT_1_A,
      &CPU::BIT_2_B, &CPU::BIT_2_C, &CPU::BIT_2_D,    &CPU::BIT_2_E,
      &CPU::BIT_2_H, &CPU::BIT_2_L, &CPU::BIT_2_HL_m, &CPU::BIT_2_A,
      &CPU::BIT_3_B, &CPU::BIT_3_C, &CPU::BIT_3_D,    &CPU::BIT_3_E,
      &CPU::BIT_3_H, &CPU::BIT_3_L, &CPU::BIT_3_HL_m, &CPU::BIT_3_A,
      &CPU::BIT_4_B, &CPU::BIT_4_C, &CPU::BIT_4_D,    &CPU::BIT_4_E,
      &CPU::BIT_4_H, &CPU::BIT_4_L, &CPU::BIT_4_HL_m, &CPU::BIT_4_A,
      &CPU::BIT_5_B, &CPU::BIT_5_C, &CPU::BIT_5_D,    &CPU::BIT_5_E,
      &CPU::BIT_5_H, &CPU::BIT_5_L, &CPU::BIT_5_HL_m, &CPU::BIT_5_A,
      &CPU::BIT_6_B, &CPU::BIT_6_C, &CPU::BIT_6_D,    &CPU::BIT_6_E,
      &CPU::BIT_6_H, &CPU::BIT_6_L, &CPU::BIT_6_HL_m, &CPU::BIT_6_A,
      &CPU::BIT_7_B, &CPU::BIT_7_C, &CPU::BIT_7_D,    &CPU::BIT_7_E,
      &CPU::BIT_7_H, &CPU::BIT_7_L, &CPU::BIT_7_HL_m, &CPU::BIT_7_A,
      &CPU::RES_0_B, &CPU::RES_0_C, &CPU::RES_0_D,    &CPU::RES_0_E,
      &CPU::RES_0_H, &CPU::RES_0_L, &CPU::RES_0_HL_m, &CPU::RES_0_A,
      &CPU::RES_1_B, &CPU::RES_1_C, &CPU::RES_1_D,    &CPU::RES_1_E,
      &CPU::RES_1_H, &CPU::RES_1_L, &CPU::RES_1_HL_m, &CPU::RES_1_A,
      &CPU::RES_2_B, &CPU::RES_2_C, &CPU::RES_2_D,    &CPU::RES_2_E,
      &CPU::RES_2_H, &CPU::RES_2_L, &CPU::RES_2_HL_m, &CPU::RES_2_A,
      &CPU::RES_3_B, &CPU::RES_3_C, &CPU::RES_3_D,    &CPU::RES_3_E,
      &CPU::RES_3_H, &CPU::RES_3_L, &CPU::RES_3_HL_m, &CPU::RES_3_A,
      &CPU::RES_4_B, &CPU::RES_4_C, &CPU::RES_4_D,    &CPU::RES_4_E,
      &CPU::RES_4_H, &CPU::RES_4_L, &CPU::RES_4_HL_m, &CPU::RES_4_A,
      &CPU::RES_5_B, &CPU::RES_5_C, &CPU::RES_5_D,    &CPU::RES_5_E,
      &CPU::RES_5_H, &CPU::RES_5_L, &CPU::RES_5_HL_m, &CPU::RES_5_A,
      &CPU::RES_6_B, &CPU::RES_6_C, &CPU::RES_6_D,    &CPU::RES_6_E,
      &CPU::RES_6_H, &CPU::RES_6_L, &CPU::RES_6_HL_m, &CPU::RES_6_A,
      &CPU::RES_7_B, &CPU::RES_7_C, &CPU::RES_7_D,    &CPU::RES_7_E,
      &CPU::RES_7_H, &CPU::RES_7_L, &CPU::RES_7_HL_m, &CPU::RES_7_A,
      &CPU::SET_0_B, &CPU::SET_0_C, &CPU::SET_0_D,    &CPU::SET_0_E,
      &CPU::SET_0_H, &CPU::SET_0_L, &CPU::SET_0_HL_m, &CPU::SET_0_A,
      &CPU::SET_1_B, &CPU::SET_1_C, &CPU::SET_1_D,    &CPU::SET_1_E,
      &CPU::SET_1_H, &CPU::SET_1_L, &CPU::SET_1_HL_m, &CPU::SET_1_A,
      &CPU::SET_2_B, &CPU::SET_2_C, &CPU::SET_2_D,    &CPU::SET_2_E,
      &CPU::SET_2_H, &CPU::SET_2_L, &CPU::SET_2_HL_m, &CPU::SET_2_A,
      &CPU::SET_3_B, &CPU::SET_3_C, &CPU::SET_3_D,    &CPU::SET_3_E,
      &CPU::SET_3_H, &CPU::SET_3_L, &CPU::SET_3_HL_m, &CPU::SET_3_A,
      &CPU::SET_4_B, &CPU::SET_4_C, &CPU::SET_4_D,    &CPU::SET_4_E,
      &CPU::SET_4_H, &CPU::SET_4_L, &CPU::SET_4_HL_m, &CPU::SET_4_A,
      &CPU::SET_5_B, &CPU::SET_5_C, &CPU::SET_5_D,    &CPU::SET_5_E,
      &CPU::SET_5_H, &CPU::SET_5_L, &CPU::SET_5_HL_m, &CPU::SET_5_A,
      &CPU::SET_6_B, &CPU::SET_6_C, &CPU::SET_6_D,    &CPU::SET_6_E,
      &CPU::SET_6_H, &CPU::SET_6_L, &CPU::SET_6_HL_m, &CPU::SET_6_A,
      &CPU::SET_7_B, &CPU::SET_7_C, &CPU::SET_7_D,    &CPU::SET_7_E,
      &CPU::SET_7_H, &CPU::SET_7_L, &CPU::SET_7_HL_m, &CPU::SET_7_A};
}

// instruction implementations
uint8_t CPU::NOP() {}
uint8_t CPU::LD_BC_d16() {}
uint8_t CPU::LD_BC_m_A() {}
uint8_t CPU::INC_BC() {}
uint8_t CPU::INC_B() {}
uint8_t CPU::DEC_B() {}
uint8_t CPU::LD_B_d8() {}
uint8_t CPU::RLCA() {}
uint8_t CPU::LD_a16_m_SP() {}
uint8_t CPU::ADD_HL_BC() {}
uint8_t CPU::LD_A_BC_m() {}
uint8_t CPU::DEC_BC() {}
uint8_t CPU::INC_C() {}
uint8_t CPU::DEC_C() {}
uint8_t CPU::LD_C_d8() {}
uint8_t CPU::RRCA() {}
uint8_t CPU::STOP_0() {}
uint8_t CPU::LD_DE_d16() {}
uint8_t CPU::LD_DE_m_A() {}
uint8_t CPU::INC_DE() {}
uint8_t CPU::INC_D() {}
uint8_t CPU::DEC_D() {}
uint8_t CPU::LD_D_d8() {}
uint8_t CPU::RLA() {}
uint8_t CPU::JR_s8() {}
uint8_t CPU::ADD_HL_DE() {}
uint8_t CPU::LD_A_DE_m() {}
uint8_t CPU::DEC_DE() {}
uint8_t CPU::INC_E() {}
uint8_t CPU::DEC_E() {}
uint8_t CPU::LD_E_d8() {}
uint8_t CPU::RRA() {}
uint8_t CPU::JR_NZ_s8() {}
uint8_t CPU::LD_HL_d16() {}
uint8_t CPU::LD_HLp_m_A() {}
uint8_t CPU::INC_HL() {}
uint8_t CPU::INC_H() {}
uint8_t CPU::DEC_H() {}
uint8_t CPU::LD_H_d8() {}
uint8_t CPU::DAA() {}
uint8_t CPU::JR_Z_s8() {}
uint8_t CPU::ADD_HL_HL() {}
uint8_t CPU::LD_A_HLp_m() {}
uint8_t CPU::DEC_HL() {}
uint8_t CPU::INC_L() {}
uint8_t CPU::DEC_L() {}
uint8_t CPU::LD_L_d8() {}
uint8_t CPU::CPL() {}
uint8_t CPU::JR_NC_s8() {}
uint8_t CPU::LD_SP_d16() {}
uint8_t CPU::LD_HLm_m_A() {}
uint8_t CPU::INC_SP() {}
uint8_t CPU::INC_HL_m() {}
uint8_t CPU::DEC_HL_m() {}
uint8_t CPU::LD_HL_m_d8() {}
uint8_t CPU::SCF() {}
uint8_t CPU::JR_C_s8() {}
uint8_t CPU::ADD_HL_SP() {}
uint8_t CPU::LD_A_HLm_m() {}
uint8_t CPU::DEC_SP() {}
uint8_t CPU::INC_A() {}
uint8_t CPU::DEC_A() {}
uint8_t CPU::LD_A_d8() {}
uint8_t CPU::CCF() {}
uint8_t CPU::LD_B_B() {}
uint8_t CPU::LD_B_C() {}
uint8_t CPU::LD_B_D() {}
uint8_t CPU::LD_B_E() {}
uint8_t CPU::LD_B_H() {}
uint8_t CPU::LD_B_L() {}
uint8_t CPU::LD_B_HL_m() {}
uint8_t CPU::LD_B_A() {}
uint8_t CPU::LD_C_B() {}
uint8_t CPU::LD_C_C() {}
uint8_t CPU::LD_C_D() {}
uint8_t CPU::LD_C_E() {}
uint8_t CPU::LD_C_H() {}
uint8_t CPU::LD_C_L() {}
uint8_t CPU::LD_C_HL_m() {}
uint8_t CPU::LD_C_A() {}
uint8_t CPU::LD_D_B() {}
uint8_t CPU::LD_D_C() {}
uint8_t CPU::LD_D_D() {}
uint8_t CPU::LD_D_E() {}
uint8_t CPU::LD_D_H() {}
uint8_t CPU::LD_D_L() {}
uint8_t CPU::LD_D_HL_m() {}
uint8_t CPU::LD_D_A() {}
uint8_t CPU::LD_E_B() {}
uint8_t CPU::LD_E_C() {}
uint8_t CPU::LD_E_D() {}
uint8_t CPU::LD_E_E() {}
uint8_t CPU::LD_E_H() {}
uint8_t CPU::LD_E_L() {}
uint8_t CPU::LD_E_HL_m() {}
uint8_t CPU::LD_E_A() {}
uint8_t CPU::LD_H_B() {}
uint8_t CPU::LD_H_C() {}
uint8_t CPU::LD_H_D() {}
uint8_t CPU::LD_H_E() {}
uint8_t CPU::LD_H_H() {}
uint8_t CPU::LD_H_L() {}
uint8_t CPU::LD_H_HL_m() {}
uint8_t CPU::LD_H_A() {}
uint8_t CPU::LD_L_B() {}
uint8_t CPU::LD_L_C() {}
uint8_t CPU::LD_L_D() {}
uint8_t CPU::LD_L_E() {}
uint8_t CPU::LD_L_H() {}
uint8_t CPU::LD_L_L() {}
uint8_t CPU::LD_L_HL_m() {}
uint8_t CPU::LD_L_A() {}
uint8_t CPU::LD_HL_m_B() {}
uint8_t CPU::LD_HL_m_C() {}
uint8_t CPU::LD_HL_m_D() {}
uint8_t CPU::LD_HL_m_E() {}
uint8_t CPU::LD_HL_m_H() {}
uint8_t CPU::LD_HL_m_L() {}
uint8_t CPU::HALT() {}
uint8_t CPU::LD_HL_m_A() {}
uint8_t CPU::LD_A_B() {}
uint8_t CPU::LD_A_C() {}
uint8_t CPU::LD_A_D() {}
uint8_t CPU::LD_A_E() {}
uint8_t CPU::LD_A_H() {}
uint8_t CPU::LD_A_L() {}
uint8_t CPU::LD_A_HL_m() {}
uint8_t CPU::LD_A_A() {}
uint8_t CPU::ADD_A_B() {}
uint8_t CPU::ADD_A_C() {}
uint8_t CPU::ADD_A_D() {}
uint8_t CPU::ADD_A_E() {}
uint8_t CPU::ADD_A_H() {}
uint8_t CPU::ADD_A_L() {}
uint8_t CPU::ADD_A_HL_m() {}
uint8_t CPU::ADD_A_A() {}
uint8_t CPU::ADC_A_B() {}
uint8_t CPU::ADC_A_C() {}
uint8_t CPU::ADC_A_D() {}
uint8_t CPU::ADC_A_E() {}
uint8_t CPU::ADC_A_H() {}
uint8_t CPU::ADC_A_L() {}
uint8_t CPU::ADC_A_HL_m() {}
uint8_t CPU::ADC_A_A() {}
uint8_t CPU::SUB_B() {}
uint8_t CPU::SUB_C() {}
uint8_t CPU::SUB_D() {}
uint8_t CPU::SUB_E() {}
uint8_t CPU::SUB_H() {}
uint8_t CPU::SUB_L() {}
uint8_t CPU::SUB_HL_m() {}
uint8_t CPU::SUB_A() {}
uint8_t CPU::SBC_A_B() {}
uint8_t CPU::SBC_A_C() {}
uint8_t CPU::SBC_A_D() {}
uint8_t CPU::SBC_A_E() {}
uint8_t CPU::SBC_A_H() {}
uint8_t CPU::SBC_A_L() {}
uint8_t CPU::SBC_A_HL_m() {}
uint8_t CPU::SBC_A_A() {}
uint8_t CPU::AND_B() {}
uint8_t CPU::AND_C() {}
uint8_t CPU::AND_D() {}
uint8_t CPU::AND_E() {}
uint8_t CPU::AND_H() {}
uint8_t CPU::AND_L() {}
uint8_t CPU::AND_HL_m() {}
uint8_t CPU::AND_A() {}
uint8_t CPU::XOR_B() {}
uint8_t CPU::XOR_C() {}
uint8_t CPU::XOR_D() {}
uint8_t CPU::XOR_E() {}
uint8_t CPU::XOR_H() {}
uint8_t CPU::XOR_L() {}
uint8_t CPU::XOR_HL_m() {}
uint8_t CPU::XOR_A() {}
uint8_t CPU::OR_B() {}
uint8_t CPU::OR_C() {}
uint8_t CPU::OR_D() {}
uint8_t CPU::OR_E() {}
uint8_t CPU::OR_H() {}
uint8_t CPU::OR_L() {}
uint8_t CPU::OR_HL_m() {}
uint8_t CPU::OR_A() {}
uint8_t CPU::CP_B() {}
uint8_t CPU::CP_C() {}
uint8_t CPU::CP_D() {}
uint8_t CPU::CP_E() {}
uint8_t CPU::CP_H() {}
uint8_t CPU::CP_L() {}
uint8_t CPU::CP_HL_m() {}
uint8_t CPU::CP_A() {}
uint8_t CPU::RET_NZ() {}
uint8_t CPU::POP_BC() {}
uint8_t CPU::JP_NZ_a16() {}
uint8_t CPU::JP_a16() {}
uint8_t CPU::CALL_NZ_a16() {}
uint8_t CPU::PUSH_BC() {}
uint8_t CPU::ADD_A_d8() {}
uint8_t CPU::RST_0() {}
uint8_t CPU::RET_Z() {}
uint8_t CPU::RET() {}
uint8_t CPU::JP_Z_a16() {}
uint8_t CPU::CALL_Z_a16() {}
uint8_t CPU::CALL_a16() {}
uint8_t CPU::ADC_A_d8() {}
uint8_t CPU::RST_1() {}
uint8_t CPU::RET_NC() {}
uint8_t CPU::POP_DE() {}
uint8_t CPU::JP_NC_a16() {}
uint8_t CPU::CALL_NC_a16() {}
uint8_t CPU::PUSH_DE() {}
uint8_t CPU::SUB_d8() {}
uint8_t CPU::RST_2() {}
uint8_t CPU::RET_C() {}
uint8_t CPU::RETI() {}
uint8_t CPU::JP_C_a16() {}
uint8_t CPU::CALL_C_a16() {}
uint8_t CPU::SBC_A_d8() {}
uint8_t CPU::RST_3() {}
uint8_t CPU::LD_a8_m_A() {}
uint8_t CPU::POP_HL() {}
uint8_t CPU::LD_C_m_A() {}
uint8_t CPU::PUSH_HL() {}
uint8_t CPU::AND_d8() {}
uint8_t CPU::RST_4() {}
uint8_t CPU::ADD_SP_s8() {}
uint8_t CPU::JP_HL() {}
uint8_t CPU::LD_a16_m_A() {}
uint8_t CPU::XOR_d8() {}
uint8_t CPU::RST_5() {}
uint8_t CPU::LD_A_a8_m() {}
uint8_t CPU::POP_AF() {}
uint8_t CPU::LD_A_C_m() {}
uint8_t CPU::DI() {}
uint8_t CPU::PUSH_AF() {}
uint8_t CPU::OR_d8() {}
uint8_t CPU::RST_6() {}
uint8_t CPU::LD_HL_SP_s8() {}
uint8_t CPU::LD_SP_HL() {}
uint8_t CPU::LD_A_a16_m() {}
uint8_t CPU::EI() {}
uint8_t CPU::CP_d8() {}
uint8_t CPU::RST_7(){}

uint8_t CPU::RLC_B() {}
uint8_t CPU::RLC_C() {}
uint8_t CPU::RLC_D() {}
uint8_t CPU::RLC_E() {}
uint8_t CPU::RLC_H() {}
uint8_t CPU::RLC_L() {}
uint8_t CPU::RLC_HL_m() {}
uint8_t CPU::RLC_A() {}
uint8_t CPU::RRC_B() {}
uint8_t CPU::RRC_C() {}
uint8_t CPU::RRC_D() {}
uint8_t CPU::RRC_E() {}
uint8_t CPU::RRC_H() {}
uint8_t CPU::RRC_L() {}
uint8_t CPU::RRC_HL_m() {}
uint8_t CPU::RRC_A() {}
uint8_t CPU::RL_B() {}
uint8_t CPU::RL_C() {}
uint8_t CPU::RL_D() {}
uint8_t CPU::RL_E() {}
uint8_t CPU::RL_H() {}
uint8_t CPU::RL_L() {}
uint8_t CPU::RL_HL_m() {}
uint8_t CPU::RL_A() {}
uint8_t CPU::RR_B() {}
uint8_t CPU::RR_C() {}
uint8_t CPU::RR_D() {}
uint8_t CPU::RR_E() {}
uint8_t CPU::RR_H() {}
uint8_t CPU::RR_L() {}
uint8_t CPU::RR_HL_m() {}
uint8_t CPU::RR_A() {}
uint8_t CPU::SLA_B() {}
uint8_t CPU::SLA_C() {}
uint8_t CPU::SLA_D() {}
uint8_t CPU::SLA_E() {}
uint8_t CPU::SLA_H() {}
uint8_t CPU::SLA_L() {}
uint8_t CPU::SLA_HL_m() {}
uint8_t CPU::SLA_A() {}
uint8_t CPU::SRC_B() {}
uint8_t CPU::SRC_C() {}
uint8_t CPU::SRC_D() {}
uint8_t CPU::SRC_E() {}
uint8_t CPU::SRC_H() {}
uint8_t CPU::SRC_L() {}
uint8_t CPU::SRC_HL_m() {}
uint8_t CPU::SRC_A() {}
uint8_t CPU::SWAP_B() {}
uint8_t CPU::SWAP_C() {}
uint8_t CPU::SWAP_D() {}
uint8_t CPU::SWAP_E() {}
uint8_t CPU::SWAP_H() {}
uint8_t CPU::SWAP_L() {}
uint8_t CPU::SWAP_HL_m() {}
uint8_t CPU::SWAP_A() {}
uint8_t CPU::SRL_B() {}
uint8_t CPU::SRL_C() {}
uint8_t CPU::SRL_D() {}
uint8_t CPU::SRL_E() {}
uint8_t CPU::SRL_H() {}
uint8_t CPU::SRL_L() {}
uint8_t CPU::SRL_HL_m() {}
uint8_t CPU::SRL_A() {}
uint8_t CPU::BIT_0_B() {}
uint8_t CPU::BIT_0_C() {}
uint8_t CPU::BIT_0_D() {}
uint8_t CPU::BIT_0_E() {}
uint8_t CPU::BIT_0_H() {}
uint8_t CPU::BIT_0_L() {}
uint8_t CPU::BIT_0_HL_m() {}
uint8_t CPU::BIT_0_A() {}
uint8_t CPU::BIT_1_B() {}
uint8_t CPU::BIT_1_C() {}
uint8_t CPU::BIT_1_D() {}
uint8_t CPU::BIT_1_E() {}
uint8_t CPU::BIT_1_H() {}
uint8_t CPU::BIT_1_L() {}
uint8_t CPU::BIT_1_HL_m() {}
uint8_t CPU::BIT_1_A() {}
uint8_t CPU::BIT_2_B() {}
uint8_t CPU::BIT_2_C() {}
uint8_t CPU::BIT_2_D() {}
uint8_t CPU::BIT_2_E() {}
uint8_t CPU::BIT_2_H() {}
uint8_t CPU::BIT_2_L() {}
uint8_t CPU::BIT_2_HL_m() {}
uint8_t CPU::BIT_2_A() {}
uint8_t CPU::BIT_3_B() {}
uint8_t CPU::BIT_3_C() {}
uint8_t CPU::BIT_3_D() {}
uint8_t CPU::BIT_3_E() {}
uint8_t CPU::BIT_3_H() {}
uint8_t CPU::BIT_3_L() {}
uint8_t CPU::BIT_3_HL_m() {}
uint8_t CPU::BIT_3_A() {}
uint8_t CPU::BIT_4_B() {}
uint8_t CPU::BIT_4_C() {}
uint8_t CPU::BIT_4_D() {}
uint8_t CPU::BIT_4_E() {}
uint8_t CPU::BIT_4_H() {}
uint8_t CPU::BIT_4_L() {}
uint8_t CPU::BIT_4_HL_m() {}
uint8_t CPU::BIT_4_A() {}
uint8_t CPU::BIT_5_B() {}
uint8_t CPU::BIT_5_C() {}
uint8_t CPU::BIT_5_D() {}
uint8_t CPU::BIT_5_E() {}
uint8_t CPU::BIT_5_H() {}
uint8_t CPU::BIT_5_L() {}
uint8_t CPU::BIT_5_HL_m() {}
uint8_t CPU::BIT_5_A() {}
uint8_t CPU::BIT_6_B() {}
uint8_t CPU::BIT_6_C() {}
uint8_t CPU::BIT_6_D() {}
uint8_t CPU::BIT_6_E() {}
uint8_t CPU::BIT_6_H() {}
uint8_t CPU::BIT_6_L() {}
uint8_t CPU::BIT_6_HL_m() {}
uint8_t CPU::BIT_6_A() {}
uint8_t CPU::BIT_7_B() {}
uint8_t CPU::BIT_7_C() {}
uint8_t CPU::BIT_7_D() {}
uint8_t CPU::BIT_7_E() {}
uint8_t CPU::BIT_7_H() {}
uint8_t CPU::BIT_7_L() {}
uint8_t CPU::BIT_7_HL_m() {}
uint8_t CPU::BIT_7_A() {}
uint8_t CPU::RES_0_B() {}
uint8_t CPU::RES_0_C() {}
uint8_t CPU::RES_0_D() {}
uint8_t CPU::RES_0_E() {}
uint8_t CPU::RES_0_H() {}
uint8_t CPU::RES_0_L() {}
uint8_t CPU::RES_0_HL_m() {}
uint8_t CPU::RES_0_A() {}
uint8_t CPU::RES_1_B() {}
uint8_t CPU::RES_1_C() {}
uint8_t CPU::RES_1_D() {}
uint8_t CPU::RES_1_E() {}
uint8_t CPU::RES_1_H() {}
uint8_t CPU::RES_1_L() {}
uint8_t CPU::RES_1_HL_m() {}
uint8_t CPU::RES_1_A() {}
uint8_t CPU::RES_2_B() {}
uint8_t CPU::RES_2_C() {}
uint8_t CPU::RES_2_D() {}
uint8_t CPU::RES_2_E() {}
uint8_t CPU::RES_2_H() {}
uint8_t CPU::RES_2_L() {}
uint8_t CPU::RES_2_HL_m() {}
uint8_t CPU::RES_2_A() {}
uint8_t CPU::RES_3_B() {}
uint8_t CPU::RES_3_C() {}
uint8_t CPU::RES_3_D() {}
uint8_t CPU::RES_3_E() {}
uint8_t CPU::RES_3_H() {}
uint8_t CPU::RES_3_L() {}
uint8_t CPU::RES_3_HL_m() {}
uint8_t CPU::RES_3_A() {}
uint8_t CPU::RES_4_B() {}
uint8_t CPU::RES_4_C() {}
uint8_t CPU::RES_4_D() {}
uint8_t CPU::RES_4_E() {}
uint8_t CPU::RES_4_H() {}
uint8_t CPU::RES_4_L() {}
uint8_t CPU::RES_4_HL_m() {}
uint8_t CPU::RES_4_A() {}
uint8_t CPU::RES_5_B() {}
uint8_t CPU::RES_5_C() {}
uint8_t CPU::RES_5_D() {}
uint8_t CPU::RES_5_E() {}
uint8_t CPU::RES_5_H() {}
uint8_t CPU::RES_5_L() {}
uint8_t CPU::RES_5_HL_m() {}
uint8_t CPU::RES_5_A() {}
uint8_t CPU::RES_6_B() {}
uint8_t CPU::RES_6_C() {}
uint8_t CPU::RES_6_D() {}
uint8_t CPU::RES_6_E() {}
uint8_t CPU::RES_6_H() {}
uint8_t CPU::RES_6_L() {}
uint8_t CPU::RES_6_HL_m() {}
uint8_t CPU::RES_6_A() {}
uint8_t CPU::RES_7_B() {}
uint8_t CPU::RES_7_C() {}
uint8_t CPU::RES_7_D() {}
uint8_t CPU::RES_7_E() {}
uint8_t CPU::RES_7_H() {}
uint8_t CPU::RES_7_L() {}
uint8_t CPU::RES_7_HL_m() {}
uint8_t CPU::RES_7_A() {}
uint8_t CPU::SET_0_B() {}
uint8_t CPU::SET_0_C() {}
uint8_t CPU::SET_0_D() {}
uint8_t CPU::SET_0_E() {}
uint8_t CPU::SET_0_H() {}
uint8_t CPU::SET_0_L() {}
uint8_t CPU::SET_0_HL_m() {}
uint8_t CPU::SET_0_A() {}
uint8_t CPU::SET_1_B() {}
uint8_t CPU::SET_1_C() {}
uint8_t CPU::SET_1_D() {}
uint8_t CPU::SET_1_E() {}
uint8_t CPU::SET_1_H() {}
uint8_t CPU::SET_1_L() {}
uint8_t CPU::SET_1_HL_m() {}
uint8_t CPU::SET_1_A() {}
uint8_t CPU::SET_2_B() {}
uint8_t CPU::SET_2_C() {}
uint8_t CPU::SET_2_D() {}
uint8_t CPU::SET_2_E() {}
uint8_t CPU::SET_2_H() {}
uint8_t CPU::SET_2_L() {}
uint8_t CPU::SET_2_HL_m() {}
uint8_t CPU::SET_2_A() {}
uint8_t CPU::SET_3_B() {}
uint8_t CPU::SET_3_C() {}
uint8_t CPU::SET_3_D() {}
uint8_t CPU::SET_3_E() {}
uint8_t CPU::SET_3_H() {}
uint8_t CPU::SET_3_L() {}
uint8_t CPU::SET_3_HL_m() {}
uint8_t CPU::SET_3_A() {}
uint8_t CPU::SET_4_B() {}
uint8_t CPU::SET_4_C() {}
uint8_t CPU::SET_4_D() {}
uint8_t CPU::SET_4_E() {}
uint8_t CPU::SET_4_H() {}
uint8_t CPU::SET_4_L() {}
uint8_t CPU::SET_4_HL_m() {}
uint8_t CPU::SET_4_A() {}
uint8_t CPU::SET_5_B() {}
uint8_t CPU::SET_5_C() {}
uint8_t CPU::SET_5_D() {}
uint8_t CPU::SET_5_E() {}
uint8_t CPU::SET_5_H() {}
uint8_t CPU::SET_5_L() {}
uint8_t CPU::SET_5_HL_m() {}
uint8_t CPU::SET_5_A() {}
uint8_t CPU::SET_6_B() {}
uint8_t CPU::SET_6_C() {}
uint8_t CPU::SET_6_D() {}
uint8_t CPU::SET_6_E() {}
uint8_t CPU::SET_6_H() {}
uint8_t CPU::SET_6_L() {}
uint8_t CPU::SET_6_HL_m() {}
uint8_t CPU::SET_6_A() {}
uint8_t CPU::SET_7_B() {}
uint8_t CPU::SET_7_C() {}
uint8_t CPU::SET_7_D() {}
uint8_t CPU::SET_7_E() {}
uint8_t CPU::SET_7_H() {}
uint8_t CPU::SET_7_L() {}
uint8_t CPU::SET_7_HL_m() {}
uint8_t CPU::SET_7_A() {}

uint8_t CPU::INVALID() {}