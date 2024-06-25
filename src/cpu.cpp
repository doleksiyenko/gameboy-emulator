#include <cpu.h>

CPU::CPU() 
{
  // create opcode_lookup through an initializer list, with function pointers
  // pointing to each of the instruction implementations

  opcode_lookup = 
  {
      {&CPU::NOP,0},         {&CPU::LD_BC_d16,0}, {&CPU::LD_BC_m_A,0},  {&CPU::INC_BC,0},
      {&CPU::INC_B,0},       {&CPU::DEC_B,0},     {&CPU::LD_B_d8,0},    {&CPU::RLCA,0},
      {&CPU::LD_a16_m_SP,0}, {&CPU::ADD_HL_BC,0}, {&CPU::LD_A_BC_m,0},  {&CPU::DEC_BC,0},
      {&CPU::INC_C,0},       {&CPU::DEC_C,0},     {&CPU::LD_C_d8,0},    {&CPU::RRCA,0},
      {&CPU::STOP_0,0},      {&CPU::LD_DE_d16,0}, {&CPU::LD_DE_m_A,0},  {&CPU::INC_DE,0},
      {&CPU::INC_D,0},       {&CPU::DEC_D,0},     {&CPU::LD_D_d8,0},    {&CPU::RLA,0},
      {&CPU::JR_s8,0},       {&CPU::ADD_HL_DE,0}, {&CPU::LD_A_DE_m,0},  {&CPU::DEC_DE,0},
      {&CPU::INC_E,0},       {&CPU::DEC_E,0},     {&CPU::LD_E_d8,0},    {&CPU::RRA,0},
      {&CPU::JR_NZ_s8,0},    {&CPU::LD_HL_d16,0}, {&CPU::LD_HLp_m_A,0}, {&CPU::INC_HL,0},
      {&CPU::INC_H,0},       {&CPU::DEC_H,0},     {&CPU::LD_H_d8,0},    {&CPU::DAA,0},
      {&CPU::JR_Z_s8,0},     {&CPU::ADD_HL_HL,0}, {&CPU::LD_A_HLp_m,0}, {&CPU::DEC_HL,0},
      {&CPU::INC_L,0},       {&CPU::DEC_L,0},     {&CPU::LD_L_d8,0},    {&CPU::CPL,0},
      {&CPU::JR_NC_s8,0},    {&CPU::LD_SP_d16,0}, {&CPU::LD_HLm_m_A,0}, {&CPU::INC_SP,0},
      {&CPU::INC_HL_m,0},    {&CPU::DEC_HL_m,0},  {&CPU::LD_HL_m_d8,0}, {&CPU::SCF,0},
      {&CPU::JR_C_s8,0},     {&CPU::ADD_HL_SP,0}, {&CPU::LD_A_HLm_m,0}, {&CPU::DEC_SP,0},
      {&CPU::INC_A,0},       {&CPU::DEC_A,0},     {&CPU::LD_A_d8,0},    {&CPU::CCF,0},
      {&CPU::LD_B_B,0},      {&CPU::LD_B_C,0},    {&CPU::LD_B_D,0},     {&CPU::LD_B_E,0},
      {&CPU::LD_B_H,0},      {&CPU::LD_B_L,0},    {&CPU::LD_B_HL_m,0},  {&CPU::LD_B_A,0},
      {&CPU::LD_C_B,0},      {&CPU::LD_C_C,0},    {&CPU::LD_C_D,0},     {&CPU::LD_C_E,0},
      {&CPU::LD_C_H,0},      {&CPU::LD_C_L,0},    {&CPU::LD_C_HL_m,0},  {&CPU::LD_C_A,0},
      {&CPU::LD_D_B,0},      {&CPU::LD_D_C,0},    {&CPU::LD_D_D,0},     {&CPU::LD_D_E,0},
      {&CPU::LD_D_H,0},      {&CPU::LD_D_L,0},    {&CPU::LD_D_HL_m,0},  {&CPU::LD_D_A,0},
      {&CPU::LD_E_B,0},      {&CPU::LD_E_C,0},    {&CPU::LD_E_D,0},     {&CPU::LD_E_E,0},
      {&CPU::LD_E_H,0},      {&CPU::LD_E_L,0},    {&CPU::LD_E_HL_m,0},  {&CPU::LD_E_A,0},
      {&CPU::LD_H_B,0},      {&CPU::LD_H_C,0},    {&CPU::LD_H_D,0},     {&CPU::LD_H_E,0},
      {&CPU::LD_H_H,0},      {&CPU::LD_H_L,0},    {&CPU::LD_H_HL_m,0},  {&CPU::LD_H_A,0},
      {&CPU::LD_L_B,0},      {&CPU::LD_L_C,0},    {&CPU::LD_L_D,0},     {&CPU::LD_L_E,0},
      {&CPU::LD_L_H,0},      {&CPU::LD_L_L,0},    {&CPU::LD_L_HL_m,0},  {&CPU::LD_L_A,0},
      {&CPU::LD_HL_m_B,0},   {&CPU::LD_HL_m_C,0}, {&CPU::LD_HL_m_D,0},  {&CPU::LD_HL_m_E,0},
      {&CPU::LD_HL_m_H,0},   {&CPU::LD_HL_m_L,0}, {&CPU::HALT,0},       {&CPU::LD_HL_m_A,0},
      {&CPU::LD_A_B,0},      {&CPU::LD_A_C,0},    {&CPU::LD_A_D,0},     {&CPU::LD_A_E,0},
      {&CPU::LD_A_H,0},      {&CPU::LD_A_L,0},    {&CPU::LD_A_HL_m,0},  {&CPU::LD_A_A,0},
      {&CPU::ADD_A_B,0},     {&CPU::ADD_A_C,0},   {&CPU::ADD_A_D,0},    {&CPU::ADD_A_E,0},
      {&CPU::ADD_A_H,0},     {&CPU::ADD_A_L,0},   {&CPU::ADD_A_HL_m,0}, {&CPU::ADD_A_A,0},
      {&CPU::ADC_A_B,0},     {&CPU::ADC_A_C,0},   {&CPU::ADC_A_D,0},    {&CPU::ADC_A_E,0},
      {&CPU::ADC_A_H,0},     {&CPU::ADC_A_L,0},   {&CPU::ADC_A_HL_m,0}, {&CPU::ADC_A_A,0},
      {&CPU::SUB_B,0},       {&CPU::SUB_C,0},     {&CPU::SUB_D,0},      {&CPU::SUB_E,0},
      {&CPU::SUB_H,0},       {&CPU::SUB_L,0},     {&CPU::SUB_HL_m,0},   {&CPU::SUB_A,0},
      {&CPU::SBC_A_B,0},     {&CPU::SBC_A_C,0},   {&CPU::SBC_A_D,0},    {&CPU::SBC_A_E,0},
      {&CPU::SBC_A_H,0},     {&CPU::SBC_A_L,0},   {&CPU::SBC_A_HL_m,0}, {&CPU::SBC_A_A,0},
      {&CPU::AND_B,0},       {&CPU::AND_C,0},     {&CPU::AND_D,0},      {&CPU::AND_E,0},
      {&CPU::AND_H,0},       {&CPU::AND_L,0},     {&CPU::AND_HL_m,0},   {&CPU::AND_A,0},
      {&CPU::XOR_B,0},       {&CPU::XOR_C,0},     {&CPU::XOR_D,0},      {&CPU::XOR_E,0},
      {&CPU::XOR_H,0},       {&CPU::XOR_L,0},     {&CPU::XOR_HL_m,0},   {&CPU::XOR_A,0},
      {&CPU::OR_B,0},        {&CPU::OR_C,0},      {&CPU::OR_D,0},       {&CPU::OR_E,0},
      {&CPU::OR_H,0},        {&CPU::OR_L,0},      {&CPU::OR_HL_m,0},    {&CPU::OR_A,0},
      {&CPU::CP_B,0},        {&CPU::CP_C,0},      {&CPU::CP_D,0},       {&CPU::CP_E,0},
      {&CPU::CP_H,0},        {&CPU::CP_L,0},      {&CPU::CP_HL_m,0},    {&CPU::CP_A,0},
      {&CPU::RET_NZ,0},      {&CPU::POP_BC,0},    {&CPU::JP_NZ_a16,0},  {&CPU::JP_a16,0},
      {&CPU::CALL_NZ_a16,0}, {&CPU::PUSH_BC,0},   {&CPU::ADD_A_d8,0},   {&CPU::RST_0,0},
      {&CPU::RET_Z,0},       {&CPU::RET,0},       {&CPU::JP_Z_a16,0},   {&CPU::INVALID,0},
      {&CPU::CALL_Z_a16,0},  {&CPU::CALL_a16,0},  {&CPU::ADC_A_d8,0},   {&CPU::RST_1,0},
      {&CPU::RET_NC,0},      {&CPU::POP_DE,0},    {&CPU::JP_NC_a16,0},  {&CPU::INVALID,0},
      {&CPU::CALL_NC_a16,0}, {&CPU::PUSH_DE,0},   {&CPU::SUB_d8,0},     {&CPU::RST_2,0},
      {&CPU::RET_C,0},       {&CPU::RETI,0},      {&CPU::JP_C_a16,0},   {&CPU::INVALID,0},
      {&CPU::CALL_C_a16,0},  {&CPU::INVALID,0},   {&CPU::SBC_A_d8,0},   {&CPU::RST_3,0},
      {&CPU::LD_a8_m_A,0},   {&CPU::POP_HL,0},    {&CPU::LD_C_m_A,0},   {&CPU::INVALID,0},
      {&CPU::INVALID,0},     {&CPU::PUSH_HL,0},   {&CPU::AND_d8,0},     {&CPU::RST_4,0},
      {&CPU::ADD_SP_s8,0},   {&CPU::JP_HL,0},     {&CPU::LD_a16_m_A,0}, {&CPU::INVALID,0},
      {&CPU::INVALID,0},     {&CPU::INVALID,0},   {&CPU::XOR_d8,0},     {&CPU::RST_5,0},
      {&CPU::LD_A_a8_m,0},   {&CPU::POP_AF,0},    {&CPU::LD_A_C_m,0},   {&CPU::DI,0},
      {&CPU::INVALID,0},     {&CPU::PUSH_AF,0},   {&CPU::OR_d8,0},      {&CPU::RST_6,0},
      {&CPU::LD_HL_SP_s8,0}, {&CPU::LD_SP_HL,0},  {&CPU::LD_A_a16_m,0}, {&CPU::EI,0},
      {&CPU::INVALID,0},     {&CPU::INVALID,0},   {&CPU::CP_d8,0},      {&CPU::RST_7, 0}};

  // create a lookup for all of the implementations of 16-bit instructions
  // (function pointers)
  cb_opcode_lookup = 
  {
      {&CPU::RLC_B, },   {&CPU::RLC_C, },   {&CPU::RLC_D, },      {&CPU::RLC_E, },
      {&CPU::RLC_H, },   {&CPU::RLC_L, },   {&CPU::RLC_HL_m, },   {&CPU::RLC_A, },
      {&CPU::RRC_B, },   {&CPU::RRC_C, },   {&CPU::RRC_D, },      {&CPU::RRC_E, },
      {&CPU::RRC_H, },   {&CPU::RRC_L, },   {&CPU::RRC_HL_m, },   {&CPU::RRC_A, },
      {&CPU::RL_B, },    {&CPU::RL_C, },    {&CPU::RL_D, },       {&CPU::RL_E, },
      {&CPU::RL_H, },    {&CPU::RL_L, },    {&CPU::RL_HL_m, },    {&CPU::RL_A, },
      {&CPU::RR_B, },    {&CPU::RR_C, },    {&CPU::RR_D, },       {&CPU::RR_E, },
      {&CPU::RR_H, },    {&CPU::RR_L, },    {&CPU::RR_HL_m, },    {&CPU::RR_A, },
      {&CPU::SLA_B, },   {&CPU::SLA_C, },   {&CPU::SLA_D, },      {&CPU::SLA_E, },
      {&CPU::SLA_H, },   {&CPU::SLA_L, },   {&CPU::SLA_HL_m, },   {&CPU::SLA_A, },
      {&CPU::SRC_B, },   {&CPU::SRC_C, },   {&CPU::SRC_D, },      {&CPU::SRC_E, },
      {&CPU::SRC_H, },   {&CPU::SRC_L, },   {&CPU::SRC_HL_m, },   {&CPU::SRC_A, },
      {&CPU::SWAP_B, },  {&CPU::SWAP_C, },  {&CPU::SWAP_D, },     {&CPU::SWAP_E, },
      {&CPU::SWAP_H, },  {&CPU::SWAP_L, },  {&CPU::SWAP_HL_m, },  {&CPU::SWAP_A, },
      {&CPU::SRL_B, },   {&CPU::SRL_C, },   {&CPU::SRL_D, },      {&CPU::SRL_E, },
      {&CPU::SRL_H, },   {&CPU::SRL_L, },   {&CPU::SRL_HL_m, },   {&CPU::SRL_A, },
      {&CPU::BIT_0_B, }, {&CPU::BIT_0_C, }, {&CPU::BIT_0_D, },    {&CPU::BIT_0_E, },
      {&CPU::BIT_0_H, }, {&CPU::BIT_0_L, }, {&CPU::BIT_0_HL_m, }, {&CPU::BIT_0_A, },
      {&CPU::BIT_1_B, }, {&CPU::BIT_1_C, }, {&CPU::BIT_1_D, },    {&CPU::BIT_1_E, },
      {&CPU::BIT_1_H, }, {&CPU::BIT_1_L, }, {&CPU::BIT_1_HL_m, }, {&CPU::BIT_1_A, },
      {&CPU::BIT_2_B, }, {&CPU::BIT_2_C, }, {&CPU::BIT_2_D, },    {&CPU::BIT_2_E, },
      {&CPU::BIT_2_H, }, {&CPU::BIT_2_L, }, {&CPU::BIT_2_HL_m, }, {&CPU::BIT_2_A, },
      {&CPU::BIT_3_B, }, {&CPU::BIT_3_C, }, {&CPU::BIT_3_D, },    {&CPU::BIT_3_E, },
      {&CPU::BIT_3_H, }, {&CPU::BIT_3_L, }, {&CPU::BIT_3_HL_m, }, {&CPU::BIT_3_A, },
      {&CPU::BIT_4_B, }, {&CPU::BIT_4_C, }, {&CPU::BIT_4_D, },    {&CPU::BIT_4_E, },
      {&CPU::BIT_4_H, }, {&CPU::BIT_4_L, }, {&CPU::BIT_4_HL_m, }, {&CPU::BIT_4_A, },
      {&CPU::BIT_5_B, }, {&CPU::BIT_5_C, }, {&CPU::BIT_5_D, },    {&CPU::BIT_5_E, },
      {&CPU::BIT_5_H, }, {&CPU::BIT_5_L, }, {&CPU::BIT_5_HL_m, }, {&CPU::BIT_5_A, },
      {&CPU::BIT_6_B, }, {&CPU::BIT_6_C, }, {&CPU::BIT_6_D, },    {&CPU::BIT_6_E, },
      {&CPU::BIT_6_H, }, {&CPU::BIT_6_L, }, {&CPU::BIT_6_HL_m, }, {&CPU::BIT_6_A, },
      {&CPU::BIT_7_B, }, {&CPU::BIT_7_C, }, {&CPU::BIT_7_D, },    {&CPU::BIT_7_E, },
      {&CPU::BIT_7_H, }, {&CPU::BIT_7_L, }, {&CPU::BIT_7_HL_m, }, {&CPU::BIT_7_A, },
      {&CPU::RES_0_B, }, {&CPU::RES_0_C, }, {&CPU::RES_0_D, },    {&CPU::RES_0_E, },
      {&CPU::RES_0_H, }, {&CPU::RES_0_L, }, {&CPU::RES_0_HL_m, }, {&CPU::RES_0_A, },
      {&CPU::RES_1_B, }, {&CPU::RES_1_C, }, {&CPU::RES_1_D, },    {&CPU::RES_1_E, },
      {&CPU::RES_1_H, }, {&CPU::RES_1_L, }, {&CPU::RES_1_HL_m, }, {&CPU::RES_1_A, },
      {&CPU::RES_2_B, }, {&CPU::RES_2_C, }, {&CPU::RES_2_D, },    {&CPU::RES_2_E, },
      {&CPU::RES_2_H, }, {&CPU::RES_2_L, }, {&CPU::RES_2_HL_m, }, {&CPU::RES_2_A, },
      {&CPU::RES_3_B, }, {&CPU::RES_3_C, }, {&CPU::RES_3_D, },    {&CPU::RES_3_E, },
      {&CPU::RES_3_H, }, {&CPU::RES_3_L, }, {&CPU::RES_3_HL_m, }, {&CPU::RES_3_A, },
      {&CPU::RES_4_B, }, {&CPU::RES_4_C, }, {&CPU::RES_4_D, },    {&CPU::RES_4_E, },
      {&CPU::RES_4_H, }, {&CPU::RES_4_L, }, {&CPU::RES_4_HL_m, }, {&CPU::RES_4_A, },
      {&CPU::RES_5_B, }, {&CPU::RES_5_C, }, {&CPU::RES_5_D, },    {&CPU::RES_5_E, },
      {&CPU::RES_5_H, }, {&CPU::RES_5_L, }, {&CPU::RES_5_HL_m, }, {&CPU::RES_5_A, },
      {&CPU::RES_6_B, }, {&CPU::RES_6_C, }, {&CPU::RES_6_D, },    {&CPU::RES_6_E, },
      {&CPU::RES_6_H, }, {&CPU::RES_6_L, }, {&CPU::RES_6_HL_m, }, {&CPU::RES_6_A, },
      {&CPU::RES_7_B, }, {&CPU::RES_7_C, }, {&CPU::RES_7_D, },    {&CPU::RES_7_E, },
      {&CPU::RES_7_H, }, {&CPU::RES_7_L, }, {&CPU::RES_7_HL_m, }, {&CPU::RES_7_A, },
      {&CPU::SET_0_B, }, {&CPU::SET_0_C, }, {&CPU::SET_0_D, },    {&CPU::SET_0_E, },
      {&CPU::SET_0_H, }, {&CPU::SET_0_L, }, {&CPU::SET_0_HL_m, }, {&CPU::SET_0_A, },
      {&CPU::SET_1_B, }, {&CPU::SET_1_C, }, {&CPU::SET_1_D, },    {&CPU::SET_1_E, },
      {&CPU::SET_1_H, }, {&CPU::SET_1_L, }, {&CPU::SET_1_HL_m, }, {&CPU::SET_1_A, },
      {&CPU::SET_2_B, }, {&CPU::SET_2_C, }, {&CPU::SET_2_D, },    {&CPU::SET_2_E, },
      {&CPU::SET_2_H, }, {&CPU::SET_2_L, }, {&CPU::SET_2_HL_m, }, {&CPU::SET_2_A, },
      {&CPU::SET_3_B, }, {&CPU::SET_3_C, }, {&CPU::SET_3_D, },    {&CPU::SET_3_E, },
      {&CPU::SET_3_H, }, {&CPU::SET_3_L, }, {&CPU::SET_3_HL_m, }, {&CPU::SET_3_A, },
      {&CPU::SET_4_B, }, {&CPU::SET_4_C, }, {&CPU::SET_4_D, },    {&CPU::SET_4_E, },
      {&CPU::SET_4_H, }, {&CPU::SET_4_L, }, {&CPU::SET_4_HL_m, }, {&CPU::SET_4_A, },
      {&CPU::SET_5_B, }, {&CPU::SET_5_C, }, {&CPU::SET_5_D, },    {&CPU::SET_5_E, },
      {&CPU::SET_5_H, }, {&CPU::SET_5_L, }, {&CPU::SET_5_HL_m, }, {&CPU::SET_5_A, },
      {&CPU::SET_6_B, }, {&CPU::SET_6_C, }, {&CPU::SET_6_D, },    {&CPU::SET_6_E, },
      {&CPU::SET_6_H, }, {&CPU::SET_6_L, }, {&CPU::SET_6_HL_m, }, {&CPU::SET_6_A, },
      {&CPU::SET_7_B, }, {&CPU::SET_7_C, }, {&CPU::SET_7_D, },    {&CPU::SET_7_E, },
      {&CPU::SET_7_H, }, {&CPU::SET_7_L, }, {&CPU::SET_7_HL_m, }, {&CPU::SET_7_A}
      };
}



void CPU::cycle()
{
    uint8_t instruction = CPU::fetch();
}



// instruction implementations
uint8_t CPU::NOP() {
    pc_++;
    return 0;
}

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