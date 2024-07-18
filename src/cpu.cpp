#include <cpu.h>
#include <bus.h>
#include <cstdint>
#include <sys/wait.h>
#include <unistd.h>

CPU::CPU() 
{
  // create opcode_lookup through an initializer list, with function pointers
  // pointing to each of the instruction implementations

  opcode_lookup = 
  {
      {&CPU::NOP,4},         {&CPU::LD_BC_d16,12}, {&CPU::LD_BC_m_A,8},  {&CPU::INC_BC,8},
      {&CPU::INC_B,4},       {&CPU::DEC_B,4},     {&CPU::LD_B_d8,8},    {&CPU::RLCA,4},
      {&CPU::LD_a16_m_SP,20},{&CPU::ADD_HL_BC,8}, {&CPU::LD_A_BC_m,8},  {&CPU::DEC_BC, 8},
      {&CPU::INC_C,4},       {&CPU::DEC_C,4},     {&CPU::LD_C_d8,8},    {&CPU::RRCA,4},
      {&CPU::STOP_0,4},      {&CPU::LD_DE_d16,12}, {&CPU::LD_DE_m_A,8},  {&CPU::INC_DE,8},
      {&CPU::INC_D,4},       {&CPU::DEC_D,4},     {&CPU::LD_D_d8,8},    {&CPU::RLA,4},
      {&CPU::JR_r8,12},      {&CPU::ADD_HL_DE,8}, {&CPU::LD_A_DE_m,8},  {&CPU::DEC_DE,8},
      {&CPU::INC_E,4},       {&CPU::DEC_E,4},     {&CPU::LD_E_d8,8},    {&CPU::RRA,4},
      {&CPU::JR_NZ_r8,8},    {&CPU::LD_HL_d16,12}, {&CPU::LD_HLp_m_A,8}, {&CPU::INC_HL,8},
      {&CPU::INC_H,4},       {&CPU::DEC_H,4},     {&CPU::LD_H_d8,8},    {&CPU::DAA,4},
      {&CPU::JR_Z_r8,8},     {&CPU::ADD_HL_HL,8}, {&CPU::LD_A_HLp_m,8}, {&CPU::DEC_HL,8},
      {&CPU::INC_L,4},       {&CPU::DEC_L,4},     {&CPU::LD_L_d8,8},    {&CPU::CPL,4},
      {&CPU::JR_NC_r8,8},    {&CPU::LD_SP_d16,12}, {&CPU::LD_HLm_m_A,8}, {&CPU::INC_SP,8},
      {&CPU::INC_HL_m,12},    {&CPU::DEC_HL_m,12},  {&CPU::LD_HL_m_d8,12}, {&CPU::SCF,4},
      {&CPU::JR_C_r8,8},     {&CPU::ADD_HL_SP,8}, {&CPU::LD_A_HLm_m,8}, {&CPU::DEC_SP,8},
      {&CPU::INC_A,4},       {&CPU::DEC_A,4},     {&CPU::LD_A_d8,8},    {&CPU::CCF,4},
      {&CPU::LD_B_B,4},      {&CPU::LD_B_C,4},    {&CPU::LD_B_D,4},     {&CPU::LD_B_E,4},
      {&CPU::LD_B_H,4},      {&CPU::LD_B_L,4},    {&CPU::LD_B_HL_m,8},  {&CPU::LD_B_A,4},
      {&CPU::LD_C_B,4},      {&CPU::LD_C_C,4},    {&CPU::LD_C_D,4},     {&CPU::LD_C_E,4},
      {&CPU::LD_C_H,4},      {&CPU::LD_C_L,4},    {&CPU::LD_C_HL_m,8},  {&CPU::LD_C_A,4},
      {&CPU::LD_D_B,4},      {&CPU::LD_D_C,4},    {&CPU::LD_D_D,4},     {&CPU::LD_D_E,4},
      {&CPU::LD_D_H,4},      {&CPU::LD_D_L,4},    {&CPU::LD_D_HL_m,8},  {&CPU::LD_D_A,4},
      {&CPU::LD_E_B,4},      {&CPU::LD_E_C,4},    {&CPU::LD_E_D,4},     {&CPU::LD_E_E,4},
      {&CPU::LD_E_H,4},      {&CPU::LD_E_L,4},    {&CPU::LD_E_HL_m,8},  {&CPU::LD_E_A,4},
      {&CPU::LD_H_B,4},      {&CPU::LD_H_C,4},    {&CPU::LD_H_D,4},     {&CPU::LD_H_E,4},
      {&CPU::LD_H_H,4},      {&CPU::LD_H_L,4},    {&CPU::LD_H_HL_m,8},  {&CPU::LD_H_A,4},
      {&CPU::LD_L_B,4},      {&CPU::LD_L_C,4},    {&CPU::LD_L_D,4},     {&CPU::LD_L_E,4},
      {&CPU::LD_L_H,4},      {&CPU::LD_L_L,4},    {&CPU::LD_L_HL_m,8},  {&CPU::LD_L_A,4},
      {&CPU::LD_HL_m_B,8},   {&CPU::LD_HL_m_C,8}, {&CPU::LD_HL_m_D,8},  {&CPU::LD_HL_m_E,8},
      {&CPU::LD_HL_m_H,8},   {&CPU::LD_HL_m_L,8}, {&CPU::HALT,4},       {&CPU::LD_HL_m_A,8},
      {&CPU::LD_A_B,4},      {&CPU::LD_A_C,4},    {&CPU::LD_A_D,4},     {&CPU::LD_A_E,4},
      {&CPU::LD_A_H,4},      {&CPU::LD_A_L,4},    {&CPU::LD_A_HL_m,8},  {&CPU::LD_A_A,4},
      {&CPU::ADD_A_B,4},     {&CPU::ADD_A_C,4},   {&CPU::ADD_A_D,4},    {&CPU::ADD_A_E,4},
      {&CPU::ADD_A_H,4},     {&CPU::ADD_A_L,4},   {&CPU::ADD_A_HL_m,8}, {&CPU::ADD_A_A,4},
      {&CPU::ADC_A_B,4},     {&CPU::ADC_A_C,4},   {&CPU::ADC_A_D,4},    {&CPU::ADC_A_E,4},
      {&CPU::ADC_A_H,4},     {&CPU::ADC_A_L,4},   {&CPU::ADC_A_HL_m,8}, {&CPU::ADC_A_A,4},
      {&CPU::SUB_B,4},       {&CPU::SUB_C,4},     {&CPU::SUB_D,4},      {&CPU::SUB_E,4},
      {&CPU::SUB_H,4},       {&CPU::SUB_L,4},     {&CPU::SUB_HL_m,8},   {&CPU::SUB_A,4},
      {&CPU::SBC_A_B,4},     {&CPU::SBC_A_C,4},   {&CPU::SBC_A_D,4},    {&CPU::SBC_A_E,4},
      {&CPU::SBC_A_H,4},     {&CPU::SBC_A_L,4},   {&CPU::SBC_A_HL_m,8}, {&CPU::SBC_A_A,4},
      {&CPU::AND_B,4},       {&CPU::AND_C,4},     {&CPU::AND_D,4},      {&CPU::AND_E,4},
      {&CPU::AND_H,4},       {&CPU::AND_L,4},     {&CPU::AND_HL_m,8},   {&CPU::AND_A,4},
      {&CPU::XOR_B,4},       {&CPU::XOR_C,4},     {&CPU::XOR_D,4},      {&CPU::XOR_E,4},
      {&CPU::XOR_H,4},       {&CPU::XOR_L,4},     {&CPU::XOR_HL_m,8},   {&CPU::XOR_A,4},
      {&CPU::OR_B,4},        {&CPU::OR_C,4},      {&CPU::OR_D,4},       {&CPU::OR_E,4},
      {&CPU::OR_H,4},        {&CPU::OR_L,4},      {&CPU::OR_HL_m,8},    {&CPU::OR_A,4},
      {&CPU::CP_B,4},        {&CPU::CP_C,4},      {&CPU::CP_D,4},       {&CPU::CP_E,4},
      {&CPU::CP_H,4},        {&CPU::CP_L,4},      {&CPU::CP_HL_m,8},    {&CPU::CP_A,4},
      {&CPU::RET_NZ,8},      {&CPU::POP_BC,12},    {&CPU::JP_NZ_a16,12},  {&CPU::JP_a16,16},
      {&CPU::CALL_NZ_a16,12}, {&CPU::PUSH_BC,16},   {&CPU::ADD_A_d8,8},   {&CPU::RST_0,16},
      {&CPU::RET_Z,8},       {&CPU::RET,16},       {&CPU::JP_Z_a16,12},   {&CPU::CB_PREFIX,4},
      {&CPU::CALL_Z_a16,12},  {&CPU::CALL_a16,24},  {&CPU::ADC_A_d8,8},   {&CPU::RST_1,16},
      {&CPU::RET_NC,8},      {&CPU::POP_DE,12},    {&CPU::JP_NC_a16,12},  {&CPU::INVALID,0},
      {&CPU::CALL_NC_a16,12}, {&CPU::PUSH_DE,16},   {&CPU::SUB_d8,8},     {&CPU::RST_2,16},
      {&CPU::RET_C,8},       {&CPU::RETI,16},      {&CPU::JP_C_a16,12},   {&CPU::INVALID,0},
      {&CPU::CALL_C_a16,12},  {&CPU::INVALID,0},   {&CPU::SBC_A_d8,8},   {&CPU::RST_3,16},
      {&CPU::LD_a8_m_A,12},   {&CPU::POP_HL,12},    {&CPU::LD_C_m_A,8},   {&CPU::INVALID,0},
      {&CPU::INVALID,0},     {&CPU::PUSH_HL,16},   {&CPU::AND_d8,8},     {&CPU::RST_4,16},
      {&CPU::ADD_SP_r8,16},   {&CPU::JP_HL,4},     {&CPU::LD_a16_m_A,16}, {&CPU::INVALID,0},
      {&CPU::INVALID,0},     {&CPU::INVALID,0},   {&CPU::XOR_d8,8},     {&CPU::RST_5,16},
      {&CPU::LD_A_a8_m,12},   {&CPU::POP_AF,12},    {&CPU::LD_A_C_m,8},   {&CPU::DI,4},
      {&CPU::INVALID,0},     {&CPU::PUSH_AF,16},   {&CPU::OR_d8,8},      {&CPU::RST_6,16},
      {&CPU::LD_HL_SP_r8,12}, {&CPU::LD_SP_HL,8},  {&CPU::LD_A_a16_m,16}, {&CPU::EI,4},
      {&CPU::INVALID,0},     {&CPU::INVALID,0},   {&CPU::CP_d8,8},      {&CPU::RST_7, 16}
      };

  // create a lookup for all of the implementations of 16-bit instructions
  // (function pointers)
  cb_opcode_lookup = 
  {
      {&CPU::RLC_B, 8},   {&CPU::RLC_C, 8},   {&CPU::RLC_D, 8},      {&CPU::RLC_E, 8},
      {&CPU::RLC_H, 8},   {&CPU::RLC_L, 8},   {&CPU::RLC_HL_m, 16},   {&CPU::RLC_A, 8},
      {&CPU::RRC_B, 8},   {&CPU::RRC_C, 8},   {&CPU::RRC_D, 8},      {&CPU::RRC_E, 8},
      {&CPU::RRC_H, 8},   {&CPU::RRC_L, 8},   {&CPU::RRC_HL_m, 16},   {&CPU::RRC_A, 8},
      {&CPU::RL_B, 8},    {&CPU::RL_C, 8},    {&CPU::RL_D, 8},       {&CPU::RL_E, 8},
      {&CPU::RL_H, 8},    {&CPU::RL_L, 8},    {&CPU::RL_HL_m, 16},    {&CPU::RL_A, 8},
      {&CPU::RR_B, 8},    {&CPU::RR_C, 8},    {&CPU::RR_D, 8},       {&CPU::RR_E, 8},
      {&CPU::RR_H, 8},    {&CPU::RR_L, 8},    {&CPU::RR_HL_m, 16},    {&CPU::RR_A, 8},
      {&CPU::SLA_B, 8},   {&CPU::SLA_C, 8},   {&CPU::SLA_D, 8},      {&CPU::SLA_E, 8},
      {&CPU::SLA_H, 8},   {&CPU::SLA_L, 8},   {&CPU::SLA_HL_m, 16},   {&CPU::SLA_A, 8},
      {&CPU::SRA_B, 8},   {&CPU::SRA_C, 8},   {&CPU::SRA_D, 8},      {&CPU::SRA_E, 8},
      {&CPU::SRA_H, 8},   {&CPU::SRA_L, 8},   {&CPU::SRA_HL_m, 16},   {&CPU::SRA_A, 8},
      {&CPU::SWAP_B, 8},  {&CPU::SWAP_C, 8},  {&CPU::SWAP_D, 8},     {&CPU::SWAP_E, 8},
      {&CPU::SWAP_H, 8},  {&CPU::SWAP_L, 8},  {&CPU::SWAP_HL_m, 16},  {&CPU::SWAP_A, 8},
      {&CPU::SRL_B, 8},   {&CPU::SRL_C, 8},   {&CPU::SRL_D, 8},      {&CPU::SRL_E, 8},
      {&CPU::SRL_H, 8},   {&CPU::SRL_L, 8},   {&CPU::SRL_HL_m, 16},   {&CPU::SRL_A, 8},
      {&CPU::BIT_0_B, 8}, {&CPU::BIT_0_C, 8}, {&CPU::BIT_0_D, 8},    {&CPU::BIT_0_E, 8},
      {&CPU::BIT_0_H, 8}, {&CPU::BIT_0_L, 8}, {&CPU::BIT_0_HL_m, 16}, {&CPU::BIT_0_A, 8},
      {&CPU::BIT_1_B, 8}, {&CPU::BIT_1_C, 8}, {&CPU::BIT_1_D, 8},    {&CPU::BIT_1_E, 8},
      {&CPU::BIT_1_H, 8}, {&CPU::BIT_1_L, 8}, {&CPU::BIT_1_HL_m, 16}, {&CPU::BIT_1_A, 8},
      {&CPU::BIT_2_B, 8}, {&CPU::BIT_2_C, 8}, {&CPU::BIT_2_D, 8},    {&CPU::BIT_2_E, 8},
      {&CPU::BIT_2_H, 8}, {&CPU::BIT_2_L, 8}, {&CPU::BIT_2_HL_m, 16}, {&CPU::BIT_2_A, 8},
      {&CPU::BIT_3_B, 8}, {&CPU::BIT_3_C, 8}, {&CPU::BIT_3_D, 8},    {&CPU::BIT_3_E, 8},
      {&CPU::BIT_3_H, 8}, {&CPU::BIT_3_L, 8}, {&CPU::BIT_3_HL_m, 16}, {&CPU::BIT_3_A, 8},
      {&CPU::BIT_4_B, 8}, {&CPU::BIT_4_C, 8}, {&CPU::BIT_4_D, 8},    {&CPU::BIT_4_E, 8},
      {&CPU::BIT_4_H, 8}, {&CPU::BIT_4_L, 8}, {&CPU::BIT_4_HL_m, 16}, {&CPU::BIT_4_A, 8},
      {&CPU::BIT_5_B, 8}, {&CPU::BIT_5_C, 8}, {&CPU::BIT_5_D, 8},    {&CPU::BIT_5_E, 8},
      {&CPU::BIT_5_H, 8}, {&CPU::BIT_5_L, 8}, {&CPU::BIT_5_HL_m, 16}, {&CPU::BIT_5_A, 8},
      {&CPU::BIT_6_B, 8}, {&CPU::BIT_6_C, 8}, {&CPU::BIT_6_D, 8},    {&CPU::BIT_6_E, 8},
      {&CPU::BIT_6_H, 8}, {&CPU::BIT_6_L, 8}, {&CPU::BIT_6_HL_m, 16}, {&CPU::BIT_6_A, 8},
      {&CPU::BIT_7_B, 8}, {&CPU::BIT_7_C, 8}, {&CPU::BIT_7_D, 8},    {&CPU::BIT_7_E, 8},
      {&CPU::BIT_7_H, 8}, {&CPU::BIT_7_L, 8}, {&CPU::BIT_7_HL_m, 16}, {&CPU::BIT_7_A, 8},
      {&CPU::RES_0_B, 8}, {&CPU::RES_0_C, 8}, {&CPU::RES_0_D, 8},    {&CPU::RES_0_E, 8},
      {&CPU::RES_0_H, 8}, {&CPU::RES_0_L, 8}, {&CPU::RES_0_HL_m, 16}, {&CPU::RES_0_A, 8},
      {&CPU::RES_1_B, 8}, {&CPU::RES_1_C, 8}, {&CPU::RES_1_D, 8},    {&CPU::RES_1_E, 8},
      {&CPU::RES_1_H, 8}, {&CPU::RES_1_L, 8}, {&CPU::RES_1_HL_m, 16}, {&CPU::RES_1_A, 8},
      {&CPU::RES_2_B, 8}, {&CPU::RES_2_C, 8}, {&CPU::RES_2_D, 8},    {&CPU::RES_2_E, 8},
      {&CPU::RES_2_H, 8}, {&CPU::RES_2_L, 8}, {&CPU::RES_2_HL_m, 16}, {&CPU::RES_2_A, 8},
      {&CPU::RES_3_B, 8}, {&CPU::RES_3_C, 8}, {&CPU::RES_3_D, 8},    {&CPU::RES_3_E, 8},
      {&CPU::RES_3_H, 8}, {&CPU::RES_3_L, 8}, {&CPU::RES_3_HL_m, 16}, {&CPU::RES_3_A, 8},
      {&CPU::RES_4_B, 8}, {&CPU::RES_4_C, 8}, {&CPU::RES_4_D, 8},    {&CPU::RES_4_E, 8},
      {&CPU::RES_4_H, 8}, {&CPU::RES_4_L, 8}, {&CPU::RES_4_HL_m, 16}, {&CPU::RES_4_A, 8},
      {&CPU::RES_5_B, 8}, {&CPU::RES_5_C, 8}, {&CPU::RES_5_D, 8},    {&CPU::RES_5_E, 8},
      {&CPU::RES_5_H, 8}, {&CPU::RES_5_L, 8}, {&CPU::RES_5_HL_m, 16}, {&CPU::RES_5_A, 8},
      {&CPU::RES_6_B, 8}, {&CPU::RES_6_C, 8}, {&CPU::RES_6_D, 8},    {&CPU::RES_6_E, 8},
      {&CPU::RES_6_H, 8}, {&CPU::RES_6_L, 8}, {&CPU::RES_6_HL_m, 16}, {&CPU::RES_6_A, 8},
      {&CPU::RES_7_B, 8}, {&CPU::RES_7_C, 8}, {&CPU::RES_7_D, 8},    {&CPU::RES_7_E, 8},
      {&CPU::RES_7_H, 8}, {&CPU::RES_7_L, 8}, {&CPU::RES_7_HL_m, 16}, {&CPU::RES_7_A, 8},
      {&CPU::SET_0_B, 8}, {&CPU::SET_0_C, 8}, {&CPU::SET_0_D, 8},    {&CPU::SET_0_E, 8},
      {&CPU::SET_0_H, 8}, {&CPU::SET_0_L, 8}, {&CPU::SET_0_HL_m, 16}, {&CPU::SET_0_A, 8},
      {&CPU::SET_1_B, 8}, {&CPU::SET_1_C, 8}, {&CPU::SET_1_D, 8},    {&CPU::SET_1_E, 8},
      {&CPU::SET_1_H, 8}, {&CPU::SET_1_L, 8}, {&CPU::SET_1_HL_m, 16}, {&CPU::SET_1_A, 8},
      {&CPU::SET_2_B, 8}, {&CPU::SET_2_C, 8}, {&CPU::SET_2_D, 8},    {&CPU::SET_2_E, 8},
      {&CPU::SET_2_H, 8}, {&CPU::SET_2_L, 8}, {&CPU::SET_2_HL_m, 16}, {&CPU::SET_2_A, 8},
      {&CPU::SET_3_B, 8}, {&CPU::SET_3_C, 8}, {&CPU::SET_3_D, 8},    {&CPU::SET_3_E, 8},
      {&CPU::SET_3_H, 8}, {&CPU::SET_3_L, 8}, {&CPU::SET_3_HL_m, 16}, {&CPU::SET_3_A, 8},
      {&CPU::SET_4_B, 8}, {&CPU::SET_4_C, 8}, {&CPU::SET_4_D, 8},    {&CPU::SET_4_E, 8},
      {&CPU::SET_4_H, 8}, {&CPU::SET_4_L, 8}, {&CPU::SET_4_HL_m, 16}, {&CPU::SET_4_A, 8},
      {&CPU::SET_5_B, 8}, {&CPU::SET_5_C, 8}, {&CPU::SET_5_D, 8},    {&CPU::SET_5_E, 8},
      {&CPU::SET_5_H, 8}, {&CPU::SET_5_L, 8}, {&CPU::SET_5_HL_m, 16}, {&CPU::SET_5_A, 8},
      {&CPU::SET_6_B, 8}, {&CPU::SET_6_C, 8}, {&CPU::SET_6_D, 8},    {&CPU::SET_6_E, 8},
      {&CPU::SET_6_H, 8}, {&CPU::SET_6_L, 8}, {&CPU::SET_6_HL_m, 16}, {&CPU::SET_6_A, 8},
      {&CPU::SET_7_B, 8}, {&CPU::SET_7_C, 8}, {&CPU::SET_7_D, 8},    {&CPU::SET_7_E, 8},
      {&CPU::SET_7_H, 8}, {&CPU::SET_7_L, 8}, {&CPU::SET_7_HL_m, 16}, {&CPU::SET_7_A, 8}
      };
}


void CPU::connect_bus(Bus* bus)
{
    bus_ = bus;
}

void CPU::cycle()
{
    // perform a cycle if we have the ability to (if we have the ability to - the last instruction has completed)
    if (t_cycles_delay == 0) {
        uint8_t instruction_code = read(pc_);
        pc_++; // immediately increment the pc after reading the instruction

        Instruction instruction;
        if (instruction_code == 0xcb) {
            // 16 bit instruction code, therefore we have to read another byte and then find the instruction in the 16 bit instruction table
            instruction = cb_opcode_lookup[read(pc_)];
            pc_++; // again increment the pc after reading another byte
        }
        else {
            instruction = opcode_lookup[instruction_code];
        }

        // get the number of cycles required to complete an instruction, and add it to the total number of cycles
        t_cycles_delay += instruction.t_cycles;

        // perform the instruction. if this is an instruction such as a CALL or RET or JP that requires extra cycles based on execuction, return this value
        // a member function must be called on an instance of the class, so we must explicitly say that we run the opcode_function based on this class
        uint8_t additional_cycles = (this->*instruction.opcode_function)();

        // get the final amount of cycles required to perform this instruction by getting the base cycles + additional cycles
        t_cycles_delay += additional_cycles;

        // one t cycle has passed, should never reach below zero since a new instruction should always provide more cycles, however make an explcit check
        if (t_cycles_delay > 0) {
            t_cycles_delay--;
        }
    }
}


// -------------- UTILITY ----------------
uint8_t CPU::read(uint16_t address) 
{
    return bus_->read(address);
}

void CPU::write(uint16_t address, uint8_t value) 
{
    bus_->write(address, value);
}

void CPU::set_flag(CPU::flags flag, bool val)
{
    if (val) {
        af_ |= flag; // set the <flag> bit to 1 in the flags register
    }
    else {
        af_ &= ~flag; // set the <flag> bit to 0 in the flags register
    }
}

uint8_t CPU::read_flag(CPU::flags flag) 
{
    switch (flag) {
        case CPU::flags::C:
            return (af_ & flag) >> 4;
            break;
        case CPU::flags::H:
            return (af_ & flag) >> 5;
            break;
        case CPU::flags::N:
            return (af_ & flag) >> 6;
            break;
        case CPU::flags::Z:
            return (af_ & flag) >> 7;
            break;
    }
}

// --------------- INSTRUCTION IMPLEMENTATIONS --------------------------

// return value other than 0 if additional cycles required
// based on instruction behaviour
uint8_t CPU::NOP() 
{
    // the pc has already been incremented
    return 0;
}

uint8_t CPU::LD_BC_d16() 
{
    /* set the register BC to the immediate value d16*/
    // read the next two bytes to get the immediate value
    uint8_t byte1 = read(pc_++);
    uint16_t byte2 = read(pc_++);
    bc_ = (byte2 << 8) + byte1;

    return 0;
}

uint8_t CPU::LD_DE_d16() 
{
    uint8_t byte1 = read(pc_++);
    uint16_t byte2 = read(pc_++);
    de_ = (byte2 << 8) + byte1;
    
    return 0;
}

uint8_t CPU::LD_HL_d16() 
{
    uint8_t byte1 = read(pc_++);
    uint16_t byte2 = read(pc_++);
    hl_ = (byte2 << 8) + byte1;
    
    return 0;
}

uint8_t CPU::LD_SP_d16() 
{
    uint8_t byte1 = read(pc_++);
    uint16_t byte2 = read(pc_++);
    sp_ = (byte2 << 8) + byte1;
    
    return 0;
}


// load contents of register A into memory address specified by register pairs

uint8_t CPU::LD_BC_m_A() 
{
    /* store the contents of A in the memory address specified by BC */
    write(bc_, (af_ & 0xff00) >> 8);
    return 0;
}

uint8_t CPU::LD_DE_m_A() 
{
    /* store the contents of A in the memory address specified by DE*/
    write(de_, (af_ & 0xff00) >> 8);
    return 0;
}

uint8_t CPU::LD_HLp_m_A() 
{
    /* store the contents of A in the memory address specified by HL */
    write(hl_++, (af_ & 0xff00) >> 8);
    return 0;
}

uint8_t CPU::LD_HLm_m_A() 
{
    /* store the contents of A in the memory address specified by HL */
    write(hl_--, (af_ & 0xff00) >> 8);
    return 0;
}


// increment / decrement 16 bit value
uint8_t CPU::INC_BC() { bc_++; return 0; }
uint8_t CPU::INC_DE() { de_++; return 0; }
uint8_t CPU::INC_HL() { hl_++; return 0; }
uint8_t CPU::INC_SP() { sp_++; return 0; }
uint8_t CPU::DEC_DE() { de_--; return 0; }
uint8_t CPU::DEC_BC() { bc_--; return 0; }
uint8_t CPU::DEC_HL() { hl_--; return 0; }
uint8_t CPU::DEC_SP() { sp_--; return 0; }

// increment / decrement 8 bit value registers

void CPU::INC_DEC_8BIT(uint16_t* reg, bool upper, bool inc)
{
    uint8_t reg_modified; // hold a temp variable with the inc / dec value - used for checking if flags need to be set
    if (upper) {
        // the register is the top 8 bits of the 16 bit register combo
        if (inc) {
            reg_modified = ((*reg & 0xff00) >> 8) + 1; 
            ((((*reg & 0x0f00) >> 8) + 1) > 0x0f00) ? set_flag(CPU::flags::H, 1) : set_flag(CPU::flags::H, 0); // if the lower 4 bits of B  causes carry, set H
        }
        else {
            reg_modified = ((*reg & 0xff00) >> 8) - 1; 
            ((((*reg & 0x0f00) >> 8) - 1) < 0x0000) ? set_flag(CPU::flags::H, 1) : set_flag(CPU::flags::H, 0); // if the lower 4 bits of B  causes carry, set H
        }  
    }
    else {
        // the register is the lower 8 bits of the 16 bit register combo
        if (inc) {
            reg_modified = (*reg & 0xff) + 1;
            (((*reg & 0x000f) + 1) > 0x000f) ? set_flag(CPU::flags::H, 1) : set_flag(CPU::flags::H, 0); // if the lower 4 bits of B  causes carry, set H
        }
        else {
            reg_modified = (*reg & 0xff) - 1;
            (((*reg & 0x000f) - 1) < 0x0000) ? set_flag(CPU::flags::H, 1) : set_flag(CPU::flags::H, 0); // if the lower 4 bits of B  causes carry, set H
        }
    }

    // set flags that are invariant on INC / DEC + if lower or upper register
    ((reg_modified) == 0) ? set_flag(CPU::flags::Z, 1) : set_flag(CPU::flags::Z, 0); // set Z if result is 0 
    inc ? set_flag(CPU::flags::N, 0) : set_flag(CPU::flags::N, 1); // set N = 0 if incrementing, N = 1 if decrementing 

    // finally, update the register to the updated value
    if (upper) {
        *reg = (*reg & 0xff) | (reg_modified << 8); // clear the upper register to 0, then set the upper register to reg_modified 
    }
    else {
        *reg = (*reg & 0xff00) | (reg_modified); // clear the lower register to 0, then set the lower register to reg_modified
    }
}

uint8_t CPU::INC_B() { CPU::INC_DEC_8BIT(&bc_, true, true); return 0; } 
uint8_t CPU::INC_C() { CPU::INC_DEC_8BIT(&bc_, false, true); return 0; }
uint8_t CPU::INC_D() { CPU::INC_DEC_8BIT(&de_, true, true); return 0; }
uint8_t CPU::INC_E() { CPU::INC_DEC_8BIT(&de_, false, true); return 0; }
uint8_t CPU::INC_H() { CPU::INC_DEC_8BIT(&hl_, true, true); return 0; }
uint8_t CPU::INC_L() { CPU::INC_DEC_8BIT(&hl_, false, true); return 0; }
uint8_t CPU::INC_A() { CPU::INC_DEC_8BIT(&af_, true, true); return 0; }
uint8_t CPU::INC_HL_m()
{
    // retrieve the 8 bit value from memory, but treat it as a 16 bit value to pass into the INC_DEC method
    uint16_t mem_val = static_cast<uint16_t>(read(hl_));
    CPU::INC_DEC_8BIT(&mem_val, false, true); // treat as lower, since there are only 8 bits in value
    write(hl_, static_cast<uint8_t>(mem_val));

    return 0;
}

uint8_t CPU::DEC_H() { CPU::INC_DEC_8BIT(&hl_, true, false); return 0; }
uint8_t CPU::DEC_L() { CPU::INC_DEC_8BIT(&hl_, false, false); return 0; }
uint8_t CPU::DEC_D() { CPU::INC_DEC_8BIT(&de_, true, false); return 0; }
uint8_t CPU::DEC_E() { CPU::INC_DEC_8BIT(&de_, false, false); return 0; }
uint8_t CPU::DEC_B() { CPU::INC_DEC_8BIT(&bc_, true, false); return 0; }
uint8_t CPU::DEC_C() { CPU::INC_DEC_8BIT(&bc_, false, false); return 0; }

uint8_t CPU::DEC_HL_m() 
{
    uint16_t mem_val = static_cast<uint16_t>(read(hl_));
    CPU::INC_DEC_8BIT(&mem_val, false, false);
    write(hl_, static_cast<uint8_t>(mem_val));

    return 0;
}
uint8_t CPU::DEC_A() { CPU::INC_DEC_8BIT(&af_, true, false); return 0; }


// load 8 bit immediate value
uint8_t CPU::LD_B_d8() 
{
    uint16_t immediate = read(pc_++); // read 8 bit value into 16 bit
    bc_ &= 0xff; // set b register to 0
    bc_ |= (immediate << 8); // set b register
    return 0;
}

uint8_t CPU::LD_C_d8() 
{
    uint8_t immediate = read(pc_++); // read 8 bit value into 16 bit
    bc_ &= 0xff00; // set c register to 0
    bc_ |= (immediate); // set c register
    return 0;
}

uint8_t CPU::LD_D_d8() 
{
    uint16_t immediate = read(pc_++); // read 8 bit value into 16 bit
    de_ &= 0xff; // set d register to 0
    de_ |= (immediate << 8); // set d register
    return 0;
}

uint8_t CPU::LD_E_d8() 
{
    uint8_t immediate = read(pc_++); // read 8 bit value into 16 bit
    de_ &= 0xff00; // set e register to 0
    de_ |= (immediate); // set e register
    return 0;
}

uint8_t CPU::LD_H_d8() 
{
    uint16_t immediate = read(pc_++); // read 8 bit value into 16 bit
    hl_ &= 0xff; // set h register to 0
    hl_ |= (immediate << 8); // set h register
    return 0;
}

uint8_t CPU::LD_L_d8() 
{
    uint8_t immediate = read(pc_++); // read 8 bit value into 16 bit
    hl_ &= 0xff00; // set l register to 0
    hl_ |= (immediate); // set l register
    return 0;
}

uint8_t CPU::LD_HL_m_d8() 
{
    // write the 8 bit immediate value to the memory address specified by hl
    uint8_t immediate = read(pc_++);
    write(hl_, immediate);
    return 0;
}

uint8_t CPU::LD_A_d8() 
{
    uint16_t immediate = read(pc_++); // read 8 bit value into 16 bit
    af_ &= 0xff; // set a register to 0
    af_ |= (immediate << 8); // set a register
    return 0;
}


// rotate register contents
uint8_t CPU::RLCA() 
{
    // rotate contents of register A to the left
    uint8_t a = (af_ & 0xff00) >> 8;
    uint8_t top_bit = (a >> 7); 

    // rotate the register, then set 0 bit to bit rotated out
    a <<= 1;
    a |= top_bit; // set the last bit to the top bit
    set_flag(CPU::flags::C, top_bit);

    // finally, set the modified a to the a register
    af_ &= 0xff; // clear the a register
    af_ |= (a << 8); // set the a register

    return 0;
}

uint8_t CPU::RRCA() 
{
    // rotate contents of register A to the right
    uint8_t a = (af_ & 0xff00) >> 8;
    uint8_t bottom_bit = (a & 1); 

    // rotate the register, then set 0 bit to bit rotated out
    a >>= 1;
    a |= (bottom_bit << 7); // set the top bit to the previous bottom bit
    set_flag(CPU::flags::C, bottom_bit);

    // finally, set the modified a to the a register
    af_ &= 0xff; // clear the a register
    af_ |= (a << 8); // set the a register

    return 0;
}

uint8_t CPU::RLA() 
{
    // rotate the contents of register A to the left, through the carry flag
    uint8_t a = (af_ & 0xff00) >> 8;

    // rotate the register, then set 0 bit to bit rotated out
    a <<= 1;

    // set bit 0 to the carry bit
    a |= read_flag(CPU::flags::C);

    // finally, set the modified a to the a register
    af_ &= 0xff; // clear the a register
    af_ |= (a << 8); // set the a register

    return 0;
}

uint8_t CPU::RRA() 
{
    // rotate the contents of register A to the right, through the carry flag
    uint8_t a = (af_ & 0xff00) >> 8;

    // rotate the register, then set 0 bit to bit rotated out
    a >>= 1;
    a |= (read_flag(CPU::flags::C) << 7); // set the top bit to the carry bit

    // finally, set the modified a to the a register
    af_ &= 0xff; // clear the a register
    af_ |= (a << 8); // set the a register

    return 0;
}


uint8_t CPU::JR_r8() 
{
    // jump relative instruction
    uint8_t immediate = read(pc_++);
    pc_ += TWOS_COMPLEMENT_8BIT(immediate);
    return 0;
}

uint8_t CPU::JR_NC_r8() 
{
    // jump if carry flag is not set 
    uint8_t immediate = read(pc_++);
    uint8_t c = read_flag(CPU::flags::C);

    if (c == 0) {
        pc_ += TWOS_COMPLEMENT_8BIT(immediate);
        return 4;
    }
    
    // if c is not 0, do not branch
    return 0;

}

// jump instructions
uint8_t CPU::JR_NZ_r8() 
{
    // jump if Z is not set
    uint8_t immediate = read(pc_++);
    uint8_t z = read_flag(CPU::flags::Z);

    if (z == 0) {
        pc_ += TWOS_COMPLEMENT_8BIT(immediate);
        return 4; // require 4 extra cycles
    }
    
    return 0;
}

uint8_t CPU::JR_Z_r8() 
{
    // jump if Z flag is set
    uint8_t immediate = read(pc_++);
    uint8_t z = read_flag(CPU::flags::Z);

    if (z == 1) {
        pc_ += TWOS_COMPLEMENT_8BIT(immediate);
        return 4;
    }

    return 0;
}

uint8_t CPU::JR_C_r8() 
{
    // jump if carry flag is set 
    uint8_t immediate = read(pc_++);
    uint8_t c = read_flag(CPU::flags::C);

    if (c == 1) {
        pc_ += TWOS_COMPLEMENT_8BIT(immediate);
        return 4;
    }
    
    // if c is not 0, do not branch
    return 0;
}

// register flag manipulation
//TODO: 
uint8_t CPU::STOP_0() {}
uint8_t CPU::DAA() {}

uint8_t CPU::SCF() 
{
    // set the carry flag 
    set_flag(CPU::flags::C, 1);
    set_flag(CPU::flags::N, 0);
    set_flag(CPU::flags::H, 0);

    return 0;
}

uint8_t CPU::CPL() 
{
    // take the ones complement of register A
    uint16_t a = (af_ & 0xff00) >> 8;
    a = ~a;
    // set register a
    af_ &= 0xff;
    af_ |= (a << 8);

    set_flag(CPU::flags::N, 1);
    set_flag(CPU::flags::H, 1);
    return 0;
}

uint8_t CPU::CCF() 
{
    // flip the carry flag
    set_flag(CPU::flags::C, !read_flag(CPU::flags::C)); 
    set_flag(CPU::flags::N, 0);
    set_flag(CPU::flags::H, 0);
    return 0;
}

// add registers
// helper function for add instructions to register HL
void CPU::ADD_REGISTERS_HL(uint16_t* reg1) 
{
    uint32_t addition = *reg1 + hl_;    
    set_flag(CPU::flags::N, 0);

    // set the H and CY flags
    if (addition > 0xffff) {
        // the addition exceeds 16 bits and therefore needs carry
        set_flag(CPU::flags::C, 1);
    }
    else {
        set_flag(CPU::flags::C, 0);
    }

    if (((*reg1) & 0xfff) + (hl_ & 0xfff) > 0xfff) { 
        // half carry flag is set when there is carry from bit 11 to 12
        set_flag(CPU::flags::H, 1);
    }
    else {
        set_flag(CPU::flags::H, 0);
    }

    // set the register HL to the value of the addition
    hl_ = addition & 0xffff;
} 

uint8_t CPU::ADD_HL_BC() 
{
    // add bc to hl
    ADD_REGISTERS_HL(&bc_); 
    return 0;
}

uint8_t CPU::ADD_HL_DE() 
{
    // add de to hl
    ADD_REGISTERS_HL(&de_); 
    return 0;
}

uint8_t CPU::ADD_HL_HL() 
{
    // add hl to hl
    ADD_REGISTERS_HL(&hl_); 
    return 0;
}

uint8_t CPU::ADD_HL_SP() 
{
    // add sp to hl
    ADD_REGISTERS_HL(&sp_); 
    return 0;
}

uint8_t CPU::LD_a16_m_SP() 
{
    // store the SP into memory address specified by a16

    // read 16 bit immediate value to get address to store SP
    uint8_t lower = read(pc_++);
    uint8_t upper = read(pc_++);

    uint16_t a16 = (upper << 8) + lower;
    write(a16, sp_ & 0xff);
    write(a16 + 1, (sp_ & 0xff00) >> 8);

    return 0;
}

// load data at register address into register A
uint8_t CPU::LD_A_BC_m() 
{
    uint16_t mem_val = read(bc_) << 8;
    
    // write the value of mem_val into register A
    af_ &= 0xff;
    af_ |= mem_val;

    return 0;
}

uint8_t CPU::LD_A_DE_m() 
{
    uint16_t mem_val = read(de_) << 8;

    // write into register A
    af_ &= 0xff;
    af_ |= mem_val;

    return 0;

}

uint8_t CPU::LD_A_HLp_m() 
{
    uint16_t mem_val = read(hl_++) << 8;

    // write into register A
    af_ &= 0xff;
    af_ |= mem_val;

    return 0;
}

uint8_t CPU::LD_A_HLm_m() 
{
    uint16_t mem_val = read(hl_--) << 8;

    // write into register A
    af_ &= 0xff;
    af_ |= mem_val;

    return 0;
}

// one byte load instructions

void LOAD_CONTENTS_INTO_REG(uint16_t* reg_pair, bool top, uint8_t reg2_contents) // helper function for the LD REG1, REG2 instructions
{
    // set register in reg_pair to reg2_contents. Top specifies which reigster in reg_pair is to be set
    if (top) {
        // clear the top register in reg_pair and then set it
        *reg_pair &= 0xff;
        *reg_pair |= (static_cast<uint16_t>(reg2_contents) << 8);
    }
    else {
        // clear the bottom register in reg_pair and then set it
        *reg_pair &= 0xff00;
        *reg_pair |= reg2_contents;
    }
}

uint8_t CPU::LD_B_B() { LOAD_CONTENTS_INTO_REG(&bc_, 1, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_B_C() { LOAD_CONTENTS_INTO_REG(&bc_, 1, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_B_D() { LOAD_CONTENTS_INTO_REG(&bc_, 1, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_B_E() { LOAD_CONTENTS_INTO_REG(&bc_, 1, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_B_H() { LOAD_CONTENTS_INTO_REG(&bc_, 1, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_B_L() { LOAD_CONTENTS_INTO_REG(&bc_, 1, (hl_ & 0xff)); return 0; }
uint8_t CPU::LD_B_HL_m() { LOAD_CONTENTS_INTO_REG(&bc_, 1, read(hl_)); return 0; }
uint8_t CPU::LD_B_A() { LOAD_CONTENTS_INTO_REG(&bc_, 1, (af_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_C_B() { LOAD_CONTENTS_INTO_REG(&bc_, 0, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_C_C() { LOAD_CONTENTS_INTO_REG(&bc_, 0, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_C_D() { LOAD_CONTENTS_INTO_REG(&bc_, 0, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_C_E() { LOAD_CONTENTS_INTO_REG(&bc_, 0, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_C_H() { LOAD_CONTENTS_INTO_REG(&bc_, 0, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_C_L() { LOAD_CONTENTS_INTO_REG(&bc_, 0, (hl_ & 0xff)); return 0; }
uint8_t CPU::LD_C_HL_m() { LOAD_CONTENTS_INTO_REG(&bc_, 0, read(hl_)); return 0; }
uint8_t CPU::LD_C_A() { LOAD_CONTENTS_INTO_REG(&bc_, 0, (af_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_D_B() { LOAD_CONTENTS_INTO_REG(&de_, 1, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_D_C() { LOAD_CONTENTS_INTO_REG(&de_, 1, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_D_D() { LOAD_CONTENTS_INTO_REG(&de_, 1, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_D_E() { LOAD_CONTENTS_INTO_REG(&de_, 1, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_D_H() { LOAD_CONTENTS_INTO_REG(&de_, 1, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_D_L() { LOAD_CONTENTS_INTO_REG(&de_, 1, (hl_ & 0xff)); return 0; }
uint8_t CPU::LD_D_HL_m() { LOAD_CONTENTS_INTO_REG(&de_, 1, read(hl_)); return 0; }
uint8_t CPU::LD_D_A() { LOAD_CONTENTS_INTO_REG(&de_, 1, (af_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_E_B() { LOAD_CONTENTS_INTO_REG(&de_, 0, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_E_C() { LOAD_CONTENTS_INTO_REG(&de_, 0, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_E_D() { LOAD_CONTENTS_INTO_REG(&de_, 0, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_E_E() { LOAD_CONTENTS_INTO_REG(&de_, 0, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_E_H() { LOAD_CONTENTS_INTO_REG(&de_, 0, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_E_L() { LOAD_CONTENTS_INTO_REG(&de_, 0, (hl_ & 0xff)); return 0; }
uint8_t CPU::LD_E_HL_m() { LOAD_CONTENTS_INTO_REG(&de_, 0, read(hl_)); return 0; }
uint8_t CPU::LD_E_A() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (af_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_H_B() { LOAD_CONTENTS_INTO_REG(&hl_, 1, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_H_C() { LOAD_CONTENTS_INTO_REG(&hl_, 1, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_H_D() { LOAD_CONTENTS_INTO_REG(&hl_, 1, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_H_E() { LOAD_CONTENTS_INTO_REG(&hl_, 1, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_H_H() { LOAD_CONTENTS_INTO_REG(&hl_, 1, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_H_L() { LOAD_CONTENTS_INTO_REG(&hl_, 1, (hl_ & 0xff)); return 0; }
uint8_t CPU::LD_H_HL_m() { LOAD_CONTENTS_INTO_REG(&hl_, 1, read(hl_)); return 0; }
uint8_t CPU::LD_H_A() { LOAD_CONTENTS_INTO_REG(&hl_, 1, (af_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_L_B() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_L_C() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_L_D() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_L_E() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_L_H() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_L_L() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (hl_ & 0xff)); return 0; }
uint8_t CPU::LD_L_HL_m() { LOAD_CONTENTS_INTO_REG(&hl_, 0, read(hl_)); return 0; }
uint8_t CPU::LD_L_A() { LOAD_CONTENTS_INTO_REG(&hl_, 0, (af_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_HL_m_B() { write(hl_, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_HL_m_C() { write(hl_, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_HL_m_D() { write(hl_, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_HL_m_E() { write(hl_, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_HL_m_H() { write(hl_, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_HL_m_L() { write(hl_, (hl_ & 0xff)); return 0; }
// TODO: halt intruction
uint8_t CPU::HALT() {}
uint8_t CPU::LD_HL_m_A() { write(hl_, (af_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_A_B() { LOAD_CONTENTS_INTO_REG(&af_, 1, (bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_A_C() { LOAD_CONTENTS_INTO_REG(&af_, 1, (bc_ & 0xff)); return 0; }
uint8_t CPU::LD_A_D() { LOAD_CONTENTS_INTO_REG(&af_, 1, (de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_A_E() { LOAD_CONTENTS_INTO_REG(&af_, 1, (de_ & 0xff)); return 0; }
uint8_t CPU::LD_A_H() { LOAD_CONTENTS_INTO_REG(&af_, 1, (hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::LD_A_L() { LOAD_CONTENTS_INTO_REG(&af_, 1, (hl_ & 0xff)); return 0; }
uint8_t CPU::LD_A_HL_m() { LOAD_CONTENTS_INTO_REG(&af_, 1, read(hl_)); return 0; }
uint8_t CPU::LD_A_A() { LOAD_CONTENTS_INTO_REG(&af_, 1, (af_ & 0xff00) >> 8); return 0; }

// ADD
void CPU::ADD(uint8_t reg_contents) 
{
    uint16_t result = ((af_ & 0xff00) >> 8) + reg_contents;

    if (result > 0xff) {
        set_flag(CPU::flags::C, 1);
    }
    else {
        set_flag(CPU::flags::C, 0);
    }

    if ((result & 0xff) == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    if (((af_ & 0x0f00) >> 8) + (reg_contents & 0xf) > 0xf) {
        set_flag(CPU::flags::H, 1);
    }
    else {
        set_flag(CPU::flags::H, 0);
    }

    set_flag(CPU::flags::N, 0);


    af_ &= 0xff; // clear register A
    af_ |= ((result & 0xff) << 8); // set register A to result
}

uint8_t CPU::ADD_A_B() { CPU::ADD((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::ADD_A_C() { CPU::ADD((bc_ & 0xff)); return 0; }
uint8_t CPU::ADD_A_D() { CPU::ADD((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::ADD_A_E() { CPU::ADD((de_ & 0xff)); return 0; }
uint8_t CPU::ADD_A_H() { CPU::ADD((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::ADD_A_L() { CPU::ADD((hl_ & 0xff)); return 0; }
uint8_t CPU::ADD_A_HL_m() { CPU::ADD(read(hl_)); return 0; }
uint8_t CPU::ADD_A_A() { CPU::ADD((af_ & 0xff00) >> 8); return 0; }

// ADD CARRY
void CPU::ADC(uint8_t reg_contents)
{
    // add together reg_contents and the value stored inside the carry flag to register A, and save the result to
    // register A

    uint16_t result = ((af_ & 0xff00) >> 8) + reg_contents + read_flag(CPU::flags::C) ;

    if (((af_ & 0x0f00) >> 8) + (reg_contents & 0xf) + read_flag(CPU::flags::C) > 0xf) {
        set_flag(CPU::flags::H, 1);
    }
    else {
        set_flag(CPU::flags::H, 0);
    }

    if (result > 0xff) {
        set_flag(CPU::flags::C, 1);
    }
    else {
        set_flag(CPU::flags::C, 0);
    }

    if ((result & 0xff) == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::N, 0);

    af_ &= 0xff; // clear register A
    af_ |= ((result & 0xff) << 8); // set register A to result
}

uint8_t CPU::ADC_A_B() { CPU::ADC((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::ADC_A_C() { CPU::ADC((bc_ & 0xff)); return 0; }
uint8_t CPU::ADC_A_D() { CPU::ADC((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::ADC_A_E() { CPU::ADC((de_ & 0xff)); return 0; }
uint8_t CPU::ADC_A_H() { CPU::ADC((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::ADC_A_L() { CPU::ADC((hl_ & 0xff)); return 0; }
uint8_t CPU::ADC_A_HL_m() { CPU::ADC(read(hl_)); return 0; }
uint8_t CPU::ADC_A_A() { CPU::ADC((af_ & 0xff00) >> 8); return 0; }


// SUBTRACT
void CPU::SUB(uint8_t reg_contents)
{
    uint8_t result = ((af_ & 0xff00) >> 8) - reg_contents;

    if (result < 0xff) {
        set_flag(CPU::flags::C, 1);
    }
    else {
        set_flag(CPU::flags::C, 0);
    }

    if ((result & 0xff) == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    if (((af_ & 0x0f00) >> 8) - (reg_contents & 0xf) < 0) {
        set_flag(CPU::flags::H, 1);
    }
    else {
        set_flag(CPU::flags::H, 0);
    }

    set_flag(CPU::flags::N, 1);


    af_ &= 0xff; // clear register A
    af_ |= ((result & 0xff) << 8); // set register A to result
}

uint8_t CPU::SUB_B() { CPU::SUB((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::SUB_C() { CPU::SUB((bc_ & 0xff)); return 0; }
uint8_t CPU::SUB_D() { CPU::SUB((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::SUB_E() { CPU::SUB((de_ & 0xff)); return 0; }
uint8_t CPU::SUB_H() { CPU::SUB((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::SUB_L() { CPU::SUB((hl_ & 0xff)); return 0; }
uint8_t CPU::SUB_HL_m() { CPU::SUB(read(hl_)); return 0; }
uint8_t CPU::SUB_A() { CPU::SUB((af_ & 0xff00) >> 8); return 0; }


// SUBTRACT CARRY
void CPU::SBC(uint8_t reg_contents)
{
    // subtract reg_contents and the value stored inside the carry flag from register A, and save the result to
    // register A

    uint16_t result = ((af_ & 0xff00) >> 8) - reg_contents - read_flag(CPU::flags::C) ;

    if (((af_ & 0x0f00) >> 8) - (reg_contents & 0xf) - read_flag(CPU::flags::C) < 0) {
        set_flag(CPU::flags::H, 1);
    }
    else {
        set_flag(CPU::flags::H, 0);
    }

    if (((af_ & 0xff00) >> 8) - reg_contents - read_flag(CPU::flags::C) < 0) {
        set_flag(CPU::flags::C, 1);
    }
    else {
        set_flag(CPU::flags::C, 0);
    }

    if ((result & 0xff) == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::N, 1);

    af_ &= 0xff; // clear register A
    af_ |= ((result & 0xff) << 8); // set register A to result
}

uint8_t CPU::SBC_A_B() { CPU::SBC((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::SBC_A_C() { CPU::SBC((bc_ & 0xff)); return 0; }
uint8_t CPU::SBC_A_D() { CPU::SBC((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::SBC_A_E() { CPU::SBC((de_ & 0xff)); return 0; }
uint8_t CPU::SBC_A_H() { CPU::SBC((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::SBC_A_L() { CPU::SBC((hl_ & 0xff)); return 0; }
uint8_t CPU::SBC_A_HL_m() { CPU::SBC(read(hl_)); return 0; }
uint8_t CPU::SBC_A_A() { CPU::SBC((af_ & 0xff00) >> 8); return 0; }


void CPU::AND(uint8_t reg_contents) 
{
    // main logic for taking the AND between A and reg_content, then setting to A
    uint16_t result = ((af_ & 0xff00) >> 8) & (reg_contents); 

    if (result == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::N, 0);
    set_flag(CPU::flags::H, 1);
    set_flag(CPU::flags::C, 0);

    af_ &= 0xff;
    af_ |= result << 8;
}

uint8_t CPU::AND_B() { CPU::AND((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::AND_C() { CPU::AND((bc_ & 0xff)); return 0; }
uint8_t CPU::AND_D() { CPU::AND((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::AND_E() { CPU::AND((de_ & 0xff)); return 0; }
uint8_t CPU::AND_H() { CPU::AND((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::AND_L() { CPU::AND((hl_ & 0xff)); return 0; }
uint8_t CPU::AND_HL_m() { CPU::AND(read(hl_)); return 0; }
uint8_t CPU::AND_A() { CPU::AND((af_ & 0xff00) >> 8); return 0; }

void CPU::XOR(uint8_t reg_content)
{
    // main logic for taking XOR between A and reg_content, then setting to A
    uint16_t result = ((af_ & 0xff00) >> 8) ^ (reg_content);

    if (result == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::N, 0);
    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::C, 0);

    af_ &= 0xff;
    af_ |= result << 8;
}

uint8_t CPU::XOR_B() { XOR((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::XOR_C() { XOR((bc_ & 0xff)); return 0; }
uint8_t CPU::XOR_D() { XOR((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::XOR_E() { XOR((de_ & 0xff)); return 0; }
uint8_t CPU::XOR_H() { XOR((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::XOR_L() { XOR((hl_ & 0xff)); return 0; }
uint8_t CPU::XOR_HL_m() { XOR(read(hl_)); return 0; }
uint8_t CPU::XOR_A() { XOR((af_ & 0xff00) >> 8); return 0; }

void CPU::OR(uint8_t reg_content) 
{
    // main logic for taking OR between A and reg_content, then setting to A
    uint16_t result = ((af_ & 0xff00) >> 8) | (reg_content);

    if (result == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::N, 0);
    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::C, 0);

    af_ &= 0xff;
    af_ |= result << 8;
}

uint8_t CPU::OR_B() { OR((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::OR_C() { OR((bc_ & 0xff)); return 0; }
uint8_t CPU::OR_D() { OR((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::OR_E() { OR((de_ & 0xff)); return 0; }
uint8_t CPU::OR_H() { OR((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::OR_L() { OR((hl_ & 0xff)); return 0; }
uint8_t CPU::OR_HL_m() { OR(read(hl_)); return 0; }
uint8_t CPU::OR_A() { OR((af_ & 0xff00) >> 8); return 0; }

void CPU::CP(uint8_t reg_contents) 
{
    uint8_t a = static_cast<uint8_t>((af_ & 0xff00) >> 8);
    uint8_t diff = a - reg_contents;

    if (diff == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::N, 1);

    if ((a & 0xf) - (reg_contents & 0xf)) {
        set_flag(CPU::flags::H, 1);
    }
    else {
        set_flag(CPU::flags::H, 0);
    }

    if (diff < 0) {
        set_flag(CPU::flags::C, 1);
    }
    else {
        set_flag(CPU::flags::C, 0);
    }
}

uint8_t CPU::CP_B() { CP((bc_ & 0xff00) >> 8); return 0; }
uint8_t CPU::CP_C() { CP((bc_ & 0xff)); return 0; }
uint8_t CPU::CP_D() { CP((de_ & 0xff00) >> 8); return 0; }
uint8_t CPU::CP_E() { CP((de_ & 0xff)); return 0; }
uint8_t CPU::CP_H() { CP((hl_ & 0xff00) >> 8); return 0; }
uint8_t CPU::CP_L() { CP((hl_ & 0xff)); return 0; }
uint8_t CPU::CP_HL_m() { CP(read(hl_)); return 0; }
uint8_t CPU::CP_A() { CP((af_ & 0xff00) >> 8); return 0; }

// push and pop to the memory stack
void CPU::POP(uint16_t* reg) 
{
    /* pop memory from stack, and store it inside register */

    // clear the c register, then load the first byte into c
    *reg &= 0xff00;
    *reg |= read(sp_++);

    // clear the b register, then load the next byte into b
    *reg &= 0x00ff;
    *reg |= static_cast<uint16_t>(read(sp_++)) << 8;
}

void CPU::PUSH(uint16_t* reg)
{
    /* push memory onto stack from register <reg> */
    uint8_t upper = (*reg & 0xff00) >> 8;
    uint8_t lower = (*reg * 0x00ff);

    write(sp_ - 1, upper);
    write(sp_ - 2, lower);
    
    sp_ -= 2;
}

uint8_t CPU::POP_BC() { POP(&bc_); return 0; }
uint8_t CPU::POP_DE() { POP(&de_); return 0; }
uint8_t CPU::POP_HL() { POP(&hl_); return 0; }
uint8_t CPU::POP_AF() { POP(&af_); return 0; }
uint8_t CPU::PUSH_BC() { PUSH(&bc_); return 0; }
uint8_t CPU::PUSH_DE() { PUSH(&de_); return 0; }
uint8_t CPU::PUSH_HL() { PUSH(&hl_); return 0; }
uint8_t CPU::PUSH_AF() { PUSH(&af_); return 0; }

// ADD, SUB, AND, OR, ADC, SBC, XOR, CP 8 bit immediate
uint8_t CPU::ADD_A_d8() 
{
    ADD(read(pc_++));
    return 0;
}

uint8_t CPU::SUB_d8() 
{ 
    SUB(read(pc_++));
    return 0;
}

uint8_t CPU::ADC_A_d8() 
{ 
    // add together reg_contents and the value stored inside the carry flag to register A, and save the result to
    // register A
    ADC(read(pc_++));
    return 0;
}

uint8_t CPU::SBC_A_d8() 
{
    SBC(read(pc_++));
    return 0;
}

uint8_t CPU::AND_d8() 
{
    /* take the AND of register A and an immediate 8 bit value, then set register A to the outcome */
    AND(read(pc_++));
    return 0;
}

uint8_t CPU::XOR_d8() 
{ 
    /* take the AND of register A and an immediate 8 bit value, then set register A to the outcome */
    XOR(read(pc_++));
    return 0;
}

uint8_t CPU::OR_d8() 
{ 
    /* take the AND of register A and an immediate 8 bit value, then set register A to the outcome */
    OR(read(pc_++));
    return 0;
}

uint8_t CPU::CP_d8() 
{ 
    CP(read(pc_++));
    return 0;
}


// Return calls
uint8_t CPU::RET() 
{
    /* return from subroutine */
    uint8_t lower = read(sp_++);
    uint16_t upper = read(sp_++);

    pc_ = (upper << 8) + lower;

    return 0;
}

uint8_t CPU::RET_NZ() 
{
    /* return from subroutine if Z == 0 */
    if (read_flag(CPU::flags::Z) == 0) {
        uint8_t lower = read(sp_++); 
        uint16_t upper = read(sp_++);
        pc_ = (upper << 8) + lower;
        return 12; // 3 additional M cycles required
    }
    else {
        return 0; 
    }
}

uint8_t CPU::RET_Z() 
{
    /* return from subroutine if Z == 1 */
    if (read_flag(CPU::flags::Z) == 1) {
        uint8_t lower = read(sp_++); 
        uint16_t upper = read(sp_++);
        pc_ = (upper << 8) + lower;
        return 12; // 3 additional M cycles required
    }
    else {
        return 0; 
    }
}

uint8_t CPU::RET_NC() 
{
    /* return from subroutine if C == 0 */
    if (read_flag(CPU::flags::C) == 0) {
        uint8_t lower = read(sp_++); 
        uint16_t upper = read(sp_++);
        pc_ = (upper << 8) + lower;
        return 12; // 3 additional M cycles required
    }
    else {
        return 0; 
    }
}

uint8_t CPU::RET_C() 
{
    /* return from subroutine if C == 1 */
    if (read_flag(CPU::flags::C) == 1) {
        uint8_t lower = read(sp_++); 
        uint16_t upper = read(sp_++);
        pc_ = (upper << 8) + lower;
        return 12; // 3 additional M cycles required
    }
    else {
        return 0; 
    }
}

uint8_t CPU::RETI() 
{
    /* enables interrupts and then returns */
    ime_ = true;

    uint8_t lower = read(sp_++);
    uint16_t upper = read(sp_++);

    pc_ = (upper << 8) + lower;

    return 0;
}

uint8_t CPU::JP_NZ_a16() 
{
    // load 16 bit immediate value into the program counter if the Z flag is 0
    if (read_flag(CPU::flags::Z) == 0) {
        uint8_t lower = read(pc_++);
        uint16_t upper = read(pc_++);
        pc_ = (upper << 8) + lower;
        return 4; // if we jump, require an additional M cycle
    }
    else {
        // continue executing from the next instruction
        return 0;
    }
}

uint8_t CPU::JP_a16() 
{
    // load 16 bit immediate value into the program counter
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++);
    
    pc_ = (upper << 8) + lower;

    return 0;
}

uint8_t CPU::JP_Z_a16() 
{
    // load 16 bit immediate value into the program counter if the Z flag is 1
    if (read_flag(CPU::flags::Z) == 1) {
        uint8_t lower = read(pc_++);
        uint16_t upper = read(pc_++);
        pc_ = (upper << 8) + lower;
        return 4; // if we jump, require an additional M cycle
    }
    else {
        // continue executing from the next instruction
        return 0;
    }
}

uint8_t CPU::JP_NC_a16() 
{
    // load 16 bit immediate value into the program counter if the C flag is 0
    if (read_flag(CPU::flags::C) == 0) {
        uint8_t lower = read(pc_++);
        uint16_t upper = read(pc_++);
        pc_ = (upper << 8) + lower;
        return 4; // if we jump, require an additional M cycle
    }
    else {
        // continue executing from the next instruction
        return 0;
    }
}

uint8_t CPU::JP_C_a16() 
{
    // load 16 bit immediate value into the program counter if the C flag is 1
    if (read_flag(CPU::flags::C) == 1) {
        uint8_t lower = read(pc_++);
        uint16_t upper = read(pc_++);
        pc_ = (upper << 8) + lower;
        return 4; // if we jump, require an additional M cycle
    }
    else {
        // continue executing from the next instruction
        return 0;
    }
}

uint8_t CPU::JP_HL() 
{
    /* load the contents of hl into the program counter */
    pc_ = hl_;
    return 0;
}

uint8_t CPU::RST_0() 
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x00;

    return 0;
}

uint8_t CPU::RST_1() 
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x08;

    return 0;
}

uint8_t CPU::RST_2() 
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x10;

    return 0;
}
uint8_t CPU::RST_3() 
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x18;

    return 0;
}
uint8_t CPU::RST_4() 
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x20;

    return 0;
}

uint8_t CPU::RST_5() 
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x28;

    return 0;
}

uint8_t CPU::RST_6() 
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x30;

    return 0;
}

uint8_t CPU::RST_7()
{
    /* save the current pc on the stack, and then reset the pc */
    uint8_t upper = (pc_ & 0xff00) >> 8;
    uint8_t lower = (pc_ & 0xff);

    write(--sp_, upper);
    write(--sp_, lower);

    pc_ = 0x38;

    return 0;
}

uint8_t CPU::CALL_NZ_a16() 
{
    // call if the Z flag is 0
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++);

    if (read_flag(CPU::flags::Z) == 0) {
        // write the current PC (the next instruction) to the stack
        write(--sp_, (pc_ & 0xff) >> 8);
        write(--sp_, (pc_ & 0x00ff));

        // jump to where the instruction specified
        pc_ = (upper << 8) + lower;

        return 12;
    }
    else {
        return 0;
    }
}

uint8_t CPU::CALL_Z_a16() 
{
    // call if the Z flag is 1
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++);

    if (read_flag(CPU::flags::Z) == 1) {
        // write the current PC (the next instruction) to the stack
        write(--sp_, (pc_ & 0xff) >> 8);
        write(--sp_, (pc_ & 0x00ff));

        // jump to where the instruction specified
        pc_ = (upper << 8) + lower;

        return 12;
    }
    else {
        return 0;
    }
}

uint8_t CPU::CALL_a16() 
{
    // save the PC of the next instruction on the stack, then jump to an instruction 
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++);

    // write the current PC (the next instruction) to the stack
    write(--sp_, (pc_ & 0xff) >> 8);
    write(--sp_, (pc_ & 0x00ff));

    // jump to where the instruction specified
    pc_ = (upper << 8) + lower;

    return 0;
}

uint8_t CPU::CALL_NC_a16() 
{
    // call if the carry flag is 0
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++);

    if (read_flag(CPU::flags::C) == 0) {
        // write the current PC (the next instruction) to the stack
        write(--sp_, (pc_ & 0xff) >> 8);
        write(--sp_, (pc_ & 0x00ff));

        // jump to where the instruction specified
        pc_ = (upper << 8) + lower;

        return 12;
    }
    else {
        return 0;
    }
}

uint8_t CPU::CALL_C_a16() 
{
    // call if the carry flag is 1
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++);

    if (read_flag(CPU::flags::C) == 1) {
        // write the current PC (the next instruction) to the stack
        write(--sp_, (pc_ & 0xff) >> 8);
        write(--sp_, (pc_ & 0x00ff));

        // jump to where the instruction specified
        pc_ = (upper << 8) + lower;

        return 12;
    }
    else {
        return 0;
    }
}

uint8_t CPU::LD_a8_m_A() 
{
    // store the contents of register A in address in range 0xff00 -> 0xffff specified by the 8 bit immediate value
    uint8_t immediate = read(pc_++);

    write(0xff00 + immediate, (af_ & 0xff00) >> 8);

    return 0;
}

uint8_t CPU::LD_A_a8_m() 
{
    // store into register A the value at address in range 0xff00 ->  0xffff specified by the 8 bit immediate value
    uint8_t immediate = read(pc_++);
    uint16_t mem_val = read(0xff00 + immediate) << 8;

    af_ &= 0x00ff;
    af_ |= mem_val;

    return 0;
}


uint8_t CPU::ADD_SP_r8() 
{
    /* add the contents of the 8 bit signed immediate to the stack pointer */ 

    int signed_immediate = TWOS_COMPLEMENT_8BIT(read(pc_++));

    if (signed_immediate >= 0) {
        // positive, so treat as addition when checking for carry flag and half-carry flag
        if ((sp_ & 0xff) + (signed_immediate & 0xff) > 0xff) { 
            set_flag(CPU::flags::C, 1);
        }
        else {
            set_flag(CPU::flags::C, 0);
        }

        if ((sp_ & 0xf) + (signed_immediate & 0xf) > 0xf) {
            set_flag(CPU::flags::H, 1);
        }
        else {
            set_flag(CPU::flags::H, 0);
        }
    }
    else {
        // negative, so treat as subtraction when checking for carry flag and half-carry flag
        if ((sp_ & 0xff) + (signed_immediate & 0xff) < 0x0) { 
            set_flag(CPU::flags::C, 1);
        }
        else {
            set_flag(CPU::flags::C, 0);
        }

        if ((sp_ & 0xf) + (signed_immediate & 0xf) < 0x0) {
            set_flag(CPU::flags::H, 1);
        }
        else {
            set_flag(CPU::flags::H, 0);
        }
    }

    sp_ += signed_immediate;

    return 0;
}



uint8_t CPU::LD_C_m_A() 
{
    // store the contents of register A in address in range 0xff00 -> 0xffff specified by register C 
    write(0xff00 + (bc_ & 0x00ff), (af_ & 0xff00) >> 8);

    return 0;
}

uint8_t CPU::LD_A_C_m() 
{
    // store into register A the value at address in range 0xff00 ->  0xffff specified by register C 
    uint16_t mem_val = read(0xff00 + (bc_ & 0x00ff)) << 8;

    af_ &= 0x00ff;
    af_ |= mem_val;

    return 0;
}

uint8_t CPU::LD_HL_SP_r8() 
{
    int signed_immediate = TWOS_COMPLEMENT_8BIT(read(pc_++));
    
    // set flags depending on if addition or subtraction

    if (signed_immediate >= 0) {
        // positive, so treat as addition when checking for carry flag and half-carry flag
        if ((sp_ & 0xff) + (signed_immediate & 0xff) > 0xff) { 
            set_flag(CPU::flags::C, 1);
        }
        else {
            set_flag(CPU::flags::C, 0);
        }

        if ((sp_ & 0xf) + (signed_immediate & 0xf) > 0xf) {
            set_flag(CPU::flags::H, 1);
        }
        else {
            set_flag(CPU::flags::H, 0);
        }
    }
    else {
        // negative, so treat as subtraction when checking for carry flag and half-carry flag
        if ((sp_ & 0xff) + (signed_immediate & 0xff) < 0x0) { 
            set_flag(CPU::flags::C, 1);
        }
        else {
            set_flag(CPU::flags::C, 0);
        }

        if ((sp_ & 0xf) + (signed_immediate & 0xf) < 0x0) {
            set_flag(CPU::flags::H, 1);
        }
        else {
            set_flag(CPU::flags::H, 0);
        }
    }

    // load the value in the hl register
    hl_ = sp_ + signed_immediate;

    return 0;
}

uint8_t CPU::LD_SP_HL() 
{
    /* load contents of register HL into the stack pointer */
    sp_ = hl_;
    return 0;
}

uint8_t CPU::LD_a16_m_A() 
{
    // store the contents of register A into the location specified by the 16 bit immedaite value
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++); 

    write(upper + lower, (af_ & 0xff00) >> 8);

    return 0;
}

uint8_t CPU::LD_A_a16_m() 
{
    /* load into register A the data stored at address a16 */
    uint8_t lower = read(pc_++);
    uint16_t upper = read(pc_++);

    uint16_t a16 = (upper << 8) + lower;
    uint16_t mem_val = read(a16);

    // store mem_val into register A
    af_ &= 0xff; // clear A register
    af_ |= mem_val << 8; // store mem_val into register A

    return 0;
}

uint8_t CPU::DI() 
{
    /* disable IME */
    ime_ = false;
    return 0;
}

uint8_t CPU::EI() 
{
    /* enable IME */
    ime_ = true;
    return 0;
}


// rotate register to the left 
void CPU::RLC(uint16_t* reg_pair, bool upper)
{
    /* rotate a register to the left and then set appropriate flags */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;
        reg = (reg << 1) + (reg >> 7);

        set_flag(CPU::flags::C, reg & 1); // check if the bit shifted out (and now the LSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the b register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {

        uint8_t reg = *reg_pair & 0xff;
        reg = (reg << 1) + (reg >> 7);

        set_flag(CPU::flags::C, reg & 1); // check if the bit shifted out (and now the LSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the b register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;

    }
}

uint8_t CPU::RLC_B() { RLC(&bc_, true); return 0; }
uint8_t CPU::RLC_C() { RLC(&bc_, false); return 0; }
uint8_t CPU::RLC_D() { RLC(&de_, true); return 0; }
uint8_t CPU::RLC_E() { RLC(&de_, false); return 0; }
uint8_t CPU::RLC_H() { RLC(&hl_, true); return 0; }
uint8_t CPU::RLC_L() { RLC(&hl_, false); return 0; }

uint8_t CPU::RLC_HL_m() 
{ 
    uint8_t reg = read(hl_);
    reg = (reg << 1) + (reg >> 7);
    set_flag(CPU::flags::C, reg & 1); // check if the bit shifted out (and now the LSB) is 1

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::RLC_A() { RLC(&af_, true); return 0; }

// rotate register to the right
void CPU::RRC(uint16_t* reg_pair, bool upper)
{
    /* rotate a register to the right and then set appropriate flags */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;
        reg = (reg >> 1) + ((reg & 1) << 7);

        set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (and now the MSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the b register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {
        uint8_t reg = *reg_pair & 0xff;
        reg = (reg >> 1) + ((reg & 1) << 7);

        set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (and now the LSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the b register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;
    }
}

uint8_t CPU::RRC_B() { RRC(&bc_, true); return 0; }
uint8_t CPU::RRC_C() { RRC(&bc_, false); return 0; }
uint8_t CPU::RRC_D() { RRC(&de_, true); return 0; }
uint8_t CPU::RRC_E() { RRC(&de_, false); return 0; }
uint8_t CPU::RRC_H() { RRC(&hl_, true); return 0; }
uint8_t CPU::RRC_L() { RRC(&hl_, false); return 0; }

uint8_t CPU::RRC_HL_m() 
{ 
    uint8_t reg = read(hl_);
    reg = (reg >> 1) + ((reg & 1) << 7);
    set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (and now the MSB) is 1

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::RRC_A() { RRC(&af_, true); return 0; }


void CPU::RL(uint16_t* reg_pair, bool upper) // rotate register to the left, rotate in carry flag
{
    /* rotate a register to the left from the carry and then set appropriate flags */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;
        uint8_t msb = reg & 0x80;
        reg = (reg << 1) + (read_flag(CPU::flags::C));

        set_flag(CPU::flags::C, msb); // check if the bit shifted out (previous MSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the b register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {

        uint8_t reg = *reg_pair & 0xff;
        uint8_t msb = reg & 0x80;
        reg = (reg << 1) + (read_flag(CPU::flags::C));

        set_flag(CPU::flags::C, msb); // check if the bit shifted out (previous MSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);
        
        // set the b register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;

    }
}


uint8_t CPU::RL_B() { RL(&bc_, true); return 0; }
uint8_t CPU::RL_C() { RL(&bc_, false); return 0; }
uint8_t CPU::RL_D() { RL(&de_, true); return 0; }
uint8_t CPU::RL_E() { RL(&de_, false); return 0; }
uint8_t CPU::RL_H() { RL(&hl_, true); return 0; }
uint8_t CPU::RL_L() { RL(&hl_, false); return 0; }

uint8_t CPU::RL_HL_m() 
{ 
    uint8_t reg = read(hl_);
    uint8_t msb = reg & 0x80;
    reg = (reg << 1) + (read_flag(CPU::flags::C));
    set_flag(CPU::flags::C, msb); // check if the bit shifted out (and now the LSB) is 1

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::RL_A() { RL(&af_, true); return 0; }


void CPU::RR(uint16_t* reg_pair, bool upper) // rotate register to the left, rotate in carry flag
{
    /* rotate a register to the right from the carry and then set appropriate flags */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;
        uint8_t lsb = reg & 1;
        reg = (reg >> 1) + (read_flag(CPU::flags::C) << 7);

        set_flag(CPU::flags::C, lsb); // check if the bit shifted out (previous MSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {

        uint8_t reg = *reg_pair & 0xff;
        uint8_t lsb = reg & 1;
        reg = (reg >> 1) + (read_flag(CPU::flags::C) << 7);

        set_flag(CPU::flags::C, lsb); // check if the bit shifted out (previous MSB) is 1

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;

    }
}

uint8_t CPU::RR_B() { RR(&bc_, true); return 0; }
uint8_t CPU::RR_C() { RR(&bc_, false); return 0; }
uint8_t CPU::RR_D() { RR(&de_, true); return 0; }
uint8_t CPU::RR_E() { RR(&de_, false); return 0; }
uint8_t CPU::RR_H() { RR(&hl_, true); return 0; }
uint8_t CPU::RR_L() { RR(&hl_, false); return 0; }

uint8_t CPU::RR_HL_m() 
{ 
    uint8_t reg = read(hl_);
    uint8_t lsb = reg & 1;
    reg = (reg >> 1) + (read_flag(CPU::flags::C) << 7);
    set_flag(CPU::flags::C, lsb); // check if the bit shifted out (and now the LSB) is 1

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::RR_A() { RR(&af_, true); return 0; }

void CPU::SLA(uint16_t* reg_pair, bool upper) // shift register to the left
{
    /* shift a register to the left and then set appropriate flags */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;
        set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (previous MSB) is 1

        reg = (reg << 1);

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {
        uint8_t reg = *reg_pair & 0xff;
        set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (previous MSB) is 1

        reg = (reg << 1); 

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;
    }
}


uint8_t CPU::SLA_B() { SLA(&bc_, true); return 0; }
uint8_t CPU::SLA_C() { SLA(&bc_, false); return 0; }
uint8_t CPU::SLA_D() { SLA(&de_, true); return 0; }
uint8_t CPU::SLA_E() { SLA(&de_, false); return 0; }
uint8_t CPU::SLA_H() { SLA(&hl_, true); return 0; }
uint8_t CPU::SLA_L() { SLA(&hl_, false); return 0; }

uint8_t CPU::SLA_HL_m() 
{ 
    uint8_t reg = read(hl_);
    set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (and now the LSB) is 1

    reg = (reg << 1);

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::SLA_A() { SLA(&af_, true); return 0; }


void CPU::SRA(uint16_t* reg_pair, bool upper) // shift register to the right 
{
    /* shift a register to the right and then set appropriate flags */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;
        set_flag(CPU::flags::C, reg & 1); // check if the bit shifted out (previous MSB) is 1

        reg = (reg << 1) | (reg & 1); // arithmetic shift (keep the lsb)

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {
        uint8_t reg = *reg_pair & 0xff;
        set_flag(CPU::flags::C, reg & 1); // check if the bit shifted out (previous MSB) is 1

        reg = (reg << 1) | (reg & 1); // arithmetic shift (keep the lsb)

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;
    }
}

uint8_t CPU::SRA_B() { SRA(&bc_, true); return 0; }
uint8_t CPU::SRA_C() { SRA(&bc_, false); return 0; }
uint8_t CPU::SRA_D() { SRA(&de_, true); return 0; }
uint8_t CPU::SRA_E() { SRA(&de_, false); return 0; }
uint8_t CPU::SRA_H() { SRA(&hl_, true); return 0; }
uint8_t CPU::SRA_L() { SRA(&hl_, false); return 0; }

uint8_t CPU::SRA_HL_m() 
{ 
    uint8_t reg = read(hl_);
    set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (and now the LSB) is 1

    reg = (reg << 1) | (reg & 1); // arithmetic shift (keep the lsb)

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::SRA_A() { SRA(&af_, true); return 0; }


void CPU::SWAP(uint16_t* reg_pair, bool upper) // swap lower 4 bits with upper 4 bits
{
    /* swap the contents of the lower-order bits and higher-order bits */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;

        reg = ((reg & 0xf0) >> 4) + ((reg & 0x0f) << 4);

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::C, 0);
        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {
        uint8_t reg = *reg_pair & 0xff;

        reg = ((reg & 0xf0) >> 4) + ((reg & 0x0f) << 4);

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::C, 0);
        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;
    }
}

uint8_t CPU::SWAP_B() { SWAP(&bc_, true); return 0; }
uint8_t CPU::SWAP_C() { SWAP(&bc_, false); return 0; }
uint8_t CPU::SWAP_D() { SWAP(&de_, true); return 0; }
uint8_t CPU::SWAP_E() { SWAP(&de_, false); return 0; }
uint8_t CPU::SWAP_H() { SWAP(&hl_, true); return 0; }
uint8_t CPU::SWAP_L() { SWAP(&hl_, false); return 0; }

uint8_t CPU::SWAP_HL_m() 
{ 
    uint8_t reg = read(hl_);

    reg = ((reg & 0xf0) >> 4) + ((reg & 0x0f) << 4);

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

        set_flag(CPU::flags::C, 0);
    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::SWAP_A() { SWAP(&af_, true); return 0; }


void CPU::SRL(uint16_t* reg_pair, bool upper) // shift register to the right 
{
    /* shift a register to the right and then set appropriate flags */
    if (upper) {
        uint8_t reg = *reg_pair >> 8;
        set_flag(CPU::flags::C, reg & 1); // check if the bit shifted out (previous MSB) is 1

        reg = (reg >> 1);

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0x00ff;
        *reg_pair |= static_cast<uint16_t>(reg) << 8;
    }
    else {
        uint8_t reg = *reg_pair & 0xff;
        set_flag(CPU::flags::C, reg & 1); // check if the bit shifted out (previous MSB) is 1

        reg = (reg >> 1); 

        if (reg == 0) {
            set_flag(CPU::flags::Z, 1);
        }
        else {
            set_flag(CPU::flags::Z, 0);
        }

        set_flag(CPU::flags::H, 0);
        set_flag(CPU::flags::N, 0);

        // set the register with the new value
        *reg_pair &= 0xff00;
        *reg_pair |= reg;
    }
}

uint8_t CPU::SRL_B() { SRL(&bc_, true); return 0; }
uint8_t CPU::SRL_C() { SRL(&bc_, false); return 0; }
uint8_t CPU::SRL_D() { SRL(&de_, true); return 0; }
uint8_t CPU::SRL_E() { SRL(&de_, false); return 0; }
uint8_t CPU::SRL_H() { SRL(&hl_, true); return 0; }
uint8_t CPU::SRL_L() { SRL(&hl_, false); return 0; }

uint8_t CPU::SRL_HL_m() 
{ 
    uint8_t reg = read(hl_);
    set_flag(CPU::flags::C, reg & 0x80); // check if the bit shifted out (and now the LSB) is 1

    reg = (reg << 1);

    if (reg == 0) {
        set_flag(CPU::flags::Z, 1);
    }
    else {
        set_flag(CPU::flags::Z, 0);
    }

    set_flag(CPU::flags::H, 0);
    set_flag(CPU::flags::N, 0);

    write(hl_, reg);

    return 0;
}

uint8_t CPU::SRL_A() { SRL(&af_, true); return 0; }


void CPU::BIT(uint8_t reg_content, uint8_t bit) // copy the complement of <bit> into the Z flag
{
        // set Z to the complement of the value at bit. if the bit is 1, value is not 0, therefore set to 0, and vice-versa
        set_flag(CPU::flags::Z, (reg_content & (1 << bit)) == 0); 
        set_flag(CPU::flags::N, 0);
        set_flag(CPU::flags::H, 1);
}

uint8_t CPU::BIT_0_B() { BIT(bc_ >> 8, 0); return 0; }
uint8_t CPU::BIT_0_C() { BIT(bc_ & 0xff, 0); return 0; }
uint8_t CPU::BIT_0_D() { BIT(de_ >> 8, 0); return 0; }
uint8_t CPU::BIT_0_E() { BIT(de_ & 0xff, 0); return 0; }
uint8_t CPU::BIT_0_H() { BIT(hl_ >> 8, 0); return 0; }
uint8_t CPU::BIT_0_L() { BIT(hl_ & 0xff, 0); return 0; }
uint8_t CPU::BIT_0_HL_m() { BIT(read(hl_), 0); return 0; }
uint8_t CPU::BIT_0_A() { BIT(af_ >> 8, 0); return 0; }
uint8_t CPU::BIT_1_B() { BIT(bc_ >> 8, 1); return 0; }
uint8_t CPU::BIT_1_C() { BIT(bc_ & 0xff, 1); return 0; }
uint8_t CPU::BIT_1_D() { BIT(de_ >> 8, 1); return 0; }
uint8_t CPU::BIT_1_E() { BIT(de_ & 0xff, 1); return 0; }
uint8_t CPU::BIT_1_H() { BIT(hl_ >> 8, 1); return 0; }
uint8_t CPU::BIT_1_L() { BIT(hl_ & 0xff, 1); return 0; }
uint8_t CPU::BIT_1_HL_m() { BIT(read(hl_), 1); return 0; }
uint8_t CPU::BIT_1_A() { BIT(af_ >> 8, 1); return 0; }
uint8_t CPU::BIT_2_B() { BIT(bc_ >> 8, 2); return 0; }
uint8_t CPU::BIT_2_C() { BIT(bc_ & 0xff, 2); return 0; }
uint8_t CPU::BIT_2_D() { BIT(de_ >> 8, 2); return 0; }
uint8_t CPU::BIT_2_E() { BIT(de_ & 0xff, 2); return 0; }
uint8_t CPU::BIT_2_H() { BIT(hl_ >> 8, 2); return 0; }
uint8_t CPU::BIT_2_L() { BIT(hl_ & 0xff, 2); return 0; }
uint8_t CPU::BIT_2_HL_m() { BIT(read(hl_), 2); return 0; }
uint8_t CPU::BIT_2_A() { BIT(af_ >> 8, 2); return 0; }
uint8_t CPU::BIT_3_B() { BIT(bc_ >> 8, 3); return 0; }
uint8_t CPU::BIT_3_C() { BIT(bc_ & 0xff, 3); return 0; }
uint8_t CPU::BIT_3_D() { BIT(de_ >> 8, 3); return 0; }
uint8_t CPU::BIT_3_E() { BIT(de_ & 0xff, 3); return 0; }
uint8_t CPU::BIT_3_H() { BIT(hl_ >> 8, 3); return 0; }
uint8_t CPU::BIT_3_L() { BIT(hl_ & 0xff, 3); return 0; }
uint8_t CPU::BIT_3_HL_m() { BIT(read(hl_), 3); return 0; }
uint8_t CPU::BIT_3_A() { BIT(af_ >> 8, 3); return 0; }
uint8_t CPU::BIT_4_B() { BIT(bc_ >> 8, 4); return 0; }
uint8_t CPU::BIT_4_C() { BIT(bc_ & 0xff,4); return 0; }
uint8_t CPU::BIT_4_D() { BIT(de_ >> 8, 4); return 0; }
uint8_t CPU::BIT_4_E() { BIT(de_ & 0xff, 4); return 0; }
uint8_t CPU::BIT_4_H() { BIT(hl_ >> 8, 4); return 0; }
uint8_t CPU::BIT_4_L() { BIT(hl_ & 0xff, 4); return 0; }
uint8_t CPU::BIT_4_HL_m() { BIT(read(hl_), 4); return 0; }
uint8_t CPU::BIT_4_A() { BIT(af_ >> 8, 4); return 0; }
uint8_t CPU::BIT_5_B() { BIT(bc_ >> 8, 5); return 0; }
uint8_t CPU::BIT_5_C() { BIT(bc_ & 0xff, 5); return 0; }
uint8_t CPU::BIT_5_D() { BIT(de_ >> 8, 5); return 0; }
uint8_t CPU::BIT_5_E() { BIT(de_ & 0xff, 5); return 0; }
uint8_t CPU::BIT_5_H() { BIT(hl_ >> 8, 5); return 0; }
uint8_t CPU::BIT_5_L() { BIT(hl_ & 0xff, 5); return 0; }
uint8_t CPU::BIT_5_HL_m() { BIT(read(hl_), 5); return 0; }
uint8_t CPU::BIT_5_A() { BIT(af_ >> 8, 5); return 0; }
uint8_t CPU::BIT_6_B() { BIT(bc_ >> 8, 6); return 0; }
uint8_t CPU::BIT_6_C() { BIT(bc_ & 0xff, 6); return 0; }
uint8_t CPU::BIT_6_D() { BIT(de_ >> 8, 6); return 0; }
uint8_t CPU::BIT_6_E() { BIT(de_ & 0xff, 6); return 0; }
uint8_t CPU::BIT_6_H() { BIT(hl_ >> 8, 6); return 0; }
uint8_t CPU::BIT_6_L() { BIT(hl_ & 0xff, 6); return 0; }
uint8_t CPU::BIT_6_HL_m() { BIT(read(hl_), 6); return 0; }
uint8_t CPU::BIT_6_A() { BIT(af_ >> 8, 6); return 0; }
uint8_t CPU::BIT_7_B() { BIT(bc_ >> 8, 7); return 0; }
uint8_t CPU::BIT_7_C() { BIT(bc_ & 0xff, 7); return 0; }
uint8_t CPU::BIT_7_D() { BIT(de_ >> 8, 7); return 0; }
uint8_t CPU::BIT_7_E() { BIT(de_ & 0xff, 7); return 0; }
uint8_t CPU::BIT_7_H() { BIT(hl_ >> 8, 7); return 0; }
uint8_t CPU::BIT_7_L() { BIT(hl_ & 0xff, 7); return 0; }
uint8_t CPU::BIT_7_HL_m() { BIT(read(hl_), 7); return 0; }
uint8_t CPU::BIT_7_A() { BIT(af_ >> 8, 7); return 0; }

// RESET instructions: set the specified bit in the register to 0
uint8_t CPU::RES_0_B() { bc_ &= ~(1 << 8); return 0; }
uint8_t CPU::RES_0_C() { bc_ &= ~(1 << 0); return 0; }
uint8_t CPU::RES_0_D() { de_ &= ~(1 << 8); return 0; }
uint8_t CPU::RES_0_E() { de_ &= ~(1 << 0); return 0; }
uint8_t CPU::RES_0_H() { hl_ &= ~(1 << 8); return 0; }
uint8_t CPU::RES_0_L() { hl_ &= ~(1 << 0); return 0; }
uint8_t CPU::RES_0_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 0);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_0_A() { bc_ &= ~(1 << 8); return 0; }

uint8_t CPU::RES_1_B() { bc_ &= ~(1 << 9); return 0; }
uint8_t CPU::RES_1_C() { bc_ &= ~(1 << 1); return 0; }
uint8_t CPU::RES_1_D() { de_ &= ~(1 << 9); return 0; }
uint8_t CPU::RES_1_E() { de_ &= ~(1 << 1); return 0; }
uint8_t CPU::RES_1_H() { hl_ &= ~(1 << 9); return 0; }
uint8_t CPU::RES_1_L() { hl_ &= ~(1 << 1); return 0; }
uint8_t CPU::RES_1_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 1);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_1_A() { af_ &= ~(1 << 9); return 0; }

uint8_t CPU::RES_2_B() { bc_ &= ~(1 << 10); return 0; }
uint8_t CPU::RES_2_C() { bc_ &= ~(1 << 2); return 0; }
uint8_t CPU::RES_2_D() { de_ &= ~(1 << 10); return 0; }
uint8_t CPU::RES_2_E() { de_ &= ~(1 << 2); return 0; }
uint8_t CPU::RES_2_H() { hl_ &= ~(1 << 10); return 0; }
uint8_t CPU::RES_2_L() { hl_ &= ~(1 << 2); return 0; }
uint8_t CPU::RES_2_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 2);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_2_A() { af_ &= ~(1 << 10); return 0; }

uint8_t CPU::RES_3_B() { bc_ &= ~(1 << 11); return 0; }
uint8_t CPU::RES_3_C() { bc_ &= ~(1 << 3); return 0; }
uint8_t CPU::RES_3_D() { de_ &= ~(1 << 11); return 0; }
uint8_t CPU::RES_3_E() { de_ &= ~(1 << 3); return 0; }
uint8_t CPU::RES_3_H() { hl_ &= ~(1 << 11); return 0; }
uint8_t CPU::RES_3_L() { hl_ &= ~(1 << 3); return 0; }
uint8_t CPU::RES_3_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 3);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_3_A() { af_ &= ~(1 << 11); return 0; }

uint8_t CPU::RES_4_B() { bc_ &= ~(1 << 12); return 0; }
uint8_t CPU::RES_4_C() { bc_ &= ~(1 << 4); return 0; }
uint8_t CPU::RES_4_D() { de_ &= ~(1 << 12); return 0; }
uint8_t CPU::RES_4_E() { de_ &= ~(1 << 4); return 0; }
uint8_t CPU::RES_4_H() { hl_ &= ~(1 << 12); return 0; }
uint8_t CPU::RES_4_L() { hl_ &= ~(1 << 4); return 0; }
uint8_t CPU::RES_4_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 4);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_4_A() { af_ &= ~(1 << 12); return 0; }

uint8_t CPU::RES_5_B() { bc_ &= ~(1 << 13); return 0; }
uint8_t CPU::RES_5_C() { bc_ &= ~(1 << 5); return 0; }
uint8_t CPU::RES_5_D() { de_ &= ~(1 << 13); return 0; }
uint8_t CPU::RES_5_E() { de_ &= ~(1 << 5); return 0; }
uint8_t CPU::RES_5_H() { hl_ &= ~(1 << 13); return 0; }
uint8_t CPU::RES_5_L() { hl_ &= ~(1 << 5); return 0; }
uint8_t CPU::RES_5_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 5);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_5_A() { af_ &= ~(1 << 13); return 0; }

uint8_t CPU::RES_6_B() { bc_ &= ~(1 << 14); return 0; }
uint8_t CPU::RES_6_C() { bc_ &= ~(1 << 6); return 0; }
uint8_t CPU::RES_6_D() { de_ &= ~(1 << 14); return 0; }
uint8_t CPU::RES_6_E() { de_ &= ~(1 << 6); return 0; }
uint8_t CPU::RES_6_H() { hl_ &= ~(1 << 14); return 0; }
uint8_t CPU::RES_6_L() { hl_ &= ~(1 << 6); return 0; }
uint8_t CPU::RES_6_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 6);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_6_A() { af_ &= ~(1 << 14); return 0; }

uint8_t CPU::RES_7_B() { bc_ &= ~(1 << 15); return 0; }
uint8_t CPU::RES_7_C() { bc_ &= ~(1 << 7); return 0; }
uint8_t CPU::RES_7_D() { de_ &= ~(1 << 15); return 0; }
uint8_t CPU::RES_7_E() { de_ &= ~(1 << 7); return 0; }
uint8_t CPU::RES_7_H() { hl_ &= ~(1 << 15); return 0; }
uint8_t CPU::RES_7_L() { hl_ &= ~(1 << 7); return 0; }
uint8_t CPU::RES_7_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val &= ~(1 << 7);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::RES_7_A() { af_ &= ~(1 << 15); return 0; }

// SET instructions: set the specified bit in the register to 1
uint8_t CPU::SET_0_B() { bc_ |= (1 << 8); return 0; }
uint8_t CPU::SET_0_C() { bc_ |= (1 << 0); return 0; }
uint8_t CPU::SET_0_D() { de_ |= (1 << 8); return 0; }
uint8_t CPU::SET_0_E() { de_ |= (1 << 0); return 0; }
uint8_t CPU::SET_0_H() { hl_ |= (1 << 8); return 0; }
uint8_t CPU::SET_0_L() { hl_ |= (1 << 0); return 0; }
uint8_t CPU::SET_0_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 0);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_0_A() { bc_ |= (1 << 8); return 0; }

uint8_t CPU::SET_1_B() { bc_ |= (1 << 9); return 0; }
uint8_t CPU::SET_1_C() { bc_ |= (1 << 1); return 0; }
uint8_t CPU::SET_1_D() { de_ |= (1 << 9); return 0; }
uint8_t CPU::SET_1_E() { de_ |= (1 << 1); return 0; }
uint8_t CPU::SET_1_H() { hl_ |= (1 << 9); return 0; }
uint8_t CPU::SET_1_L() { hl_ |= (1 << 1); return 0; }
uint8_t CPU::SET_1_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 1);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_1_A() { af_ |= (1 << 9); return 0; }

uint8_t CPU::SET_2_B() { bc_ |= (1 << 10); return 0; }
uint8_t CPU::SET_2_C() { bc_ |= (1 << 2); return 0; }
uint8_t CPU::SET_2_D() { de_ |= (1 << 10); return 0; }
uint8_t CPU::SET_2_E() { de_ |= (1 << 2); return 0; }
uint8_t CPU::SET_2_H() { hl_ |= (1 << 10); return 0; }
uint8_t CPU::SET_2_L() { hl_ |= (1 << 2); return 0; }
uint8_t CPU::SET_2_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 2);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_2_A() { af_ |= (1 << 10); return 0; }

uint8_t CPU::SET_3_B() { bc_ |= (1 << 11); return 0; }
uint8_t CPU::SET_3_C() { bc_ |= (1 << 3); return 0; }
uint8_t CPU::SET_3_D() { de_ |= (1 << 11); return 0; }
uint8_t CPU::SET_3_E() { de_ |= (1 << 3); return 0; }
uint8_t CPU::SET_3_H() { hl_ |= (1 << 11); return 0; }
uint8_t CPU::SET_3_L() { hl_ |= (1 << 3); return 0; }
uint8_t CPU::SET_3_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 3);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_3_A() { af_ |= (1 << 11); return 0; }

uint8_t CPU::SET_4_B() { bc_ |= (1 << 12); return 0; }
uint8_t CPU::SET_4_C() { bc_ |= (1 << 4); return 0; }
uint8_t CPU::SET_4_D() { de_ |= (1 << 12); return 0; }
uint8_t CPU::SET_4_E() { de_ |= (1 << 4); return 0; }
uint8_t CPU::SET_4_H() { hl_ |= (1 << 12); return 0; }
uint8_t CPU::SET_4_L() { hl_ |= (1 << 4); return 0; }
uint8_t CPU::SET_4_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 4);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_4_A() { af_ |= (1 << 12); return 0; }

uint8_t CPU::SET_5_B() { bc_ |= (1 << 13); return 0; }
uint8_t CPU::SET_5_C() { bc_ |= (1 << 5); return 0; }
uint8_t CPU::SET_5_D() { de_ |= (1 << 13); return 0; }
uint8_t CPU::SET_5_E() { de_ |= (1 << 5); return 0; }
uint8_t CPU::SET_5_H() { hl_ |= (1 << 13); return 0; }
uint8_t CPU::SET_5_L() { hl_ |= (1 << 5); return 0; }
uint8_t CPU::SET_5_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 5);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_5_A() { af_ |= (1 << 13); return 0; }

uint8_t CPU::SET_6_B() { bc_ |= (1 << 14); return 0; }
uint8_t CPU::SET_6_C() { bc_ |= (1 << 6); return 0; }
uint8_t CPU::SET_6_D() { de_ |= (1 << 14); return 0; }
uint8_t CPU::SET_6_E() { de_ |= (1 << 6); return 0; }
uint8_t CPU::SET_6_H() { hl_ |= (1 << 14); return 0; }
uint8_t CPU::SET_6_L() { hl_ |= (1 << 6); return 0; }
uint8_t CPU::SET_6_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 6);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_6_A() { af_ |= (1 << 14); return 0; }

uint8_t CPU::SET_7_B() { bc_ |= (1 << 15); return 0; }
uint8_t CPU::SET_7_C() { bc_ |= (1 << 7); return 0; }
uint8_t CPU::SET_7_D() { de_ |= (1 << 15); return 0; }
uint8_t CPU::SET_7_E() { de_ |= (1 << 7); return 0; }
uint8_t CPU::SET_7_H() { hl_ |= (1 << 15); return 0; }
uint8_t CPU::SET_7_L() { hl_ |= (1 << 7); return 0; }
uint8_t CPU::SET_7_HL_m() 
{

    uint8_t mem_val = read(hl_);
    mem_val |= (1 << 7);
    write(hl_, mem_val);
    return 0;

}

uint8_t CPU::SET_7_A() { af_ |= (1 << 15); return 0; }


uint8_t CPU::INVALID() { return 0; }
uint8_t CPU::CB_PREFIX() { return 0; } // this function has no utility, since we simply read the next instruction if we see 0xcb in the cycle method