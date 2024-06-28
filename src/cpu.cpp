#include <cpu.h>
#include <bus.h>

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
      {&CPU::JR_s8,12},      {&CPU::ADD_HL_DE,8}, {&CPU::LD_A_DE_m,8},  {&CPU::DEC_DE,8},
      {&CPU::INC_E,4},       {&CPU::DEC_E,4},     {&CPU::LD_E_d8,8},    {&CPU::RRA,4},
      {&CPU::JR_NZ_s8,8},    {&CPU::LD_HL_d16,12}, {&CPU::LD_HLp_m_A,8}, {&CPU::INC_HL,8},
      {&CPU::INC_H,4},       {&CPU::DEC_H,4},     {&CPU::LD_H_d8,8},    {&CPU::DAA,4},
      {&CPU::JR_Z_s8,8},     {&CPU::ADD_HL_HL,8}, {&CPU::LD_A_HLp_m,8}, {&CPU::DEC_HL,8},
      {&CPU::INC_L,4},       {&CPU::DEC_L,4},     {&CPU::LD_L_d8,8},    {&CPU::CPL,4},
      {&CPU::JR_NC_s8,8},    {&CPU::LD_SP_d16,12}, {&CPU::LD_HLm_m_A,8}, {&CPU::INC_SP,8},
      {&CPU::INC_HL_m,12},    {&CPU::DEC_HL_m,12},  {&CPU::LD_HL_m_d8,12}, {&CPU::SCF,4},
      {&CPU::JR_C_s8,8},     {&CPU::ADD_HL_SP,8}, {&CPU::LD_A_HLm_m,8}, {&CPU::DEC_SP,8},
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
      {&CPU::ADD_SP_s8,16},   {&CPU::JP_HL,4},     {&CPU::LD_a16_m_A,16}, {&CPU::INVALID,0},
      {&CPU::INVALID,0},     {&CPU::INVALID,0},   {&CPU::XOR_d8,8},     {&CPU::RST_5,16},
      {&CPU::LD_A_a8_m,12},   {&CPU::POP_AF,12},    {&CPU::LD_A_C_m,8},   {&CPU::DI,4},
      {&CPU::INVALID,0},     {&CPU::PUSH_AF,16},   {&CPU::OR_d8,8},      {&CPU::RST_6,16},
      {&CPU::LD_HL_SP_s8,12}, {&CPU::LD_SP_HL,8},  {&CPU::LD_A_a16_m,16}, {&CPU::EI,4},
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
      {&CPU::SRC_B, 8},   {&CPU::SRC_C, 8},   {&CPU::SRC_D, 8},      {&CPU::SRC_E, 8},
      {&CPU::SRC_H, 8},   {&CPU::SRC_L, 8},   {&CPU::SRC_HL_m, 16},   {&CPU::SRC_A, 8},
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
    uint8_t byte1 = read(pc_);
    uint8_t byte2 = read(++pc_);
    bc_ = (byte2 << 8) + byte1;

    return 0;
}

uint8_t CPU::LD_BC_m_A() 
{
    /* store the contents of A in the memory address specified by BC */
    write(bc_, (af_ & 0xff00) >> 8);

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
uint8_t CPU::INC_B() 
{
    uint8_t b_inc = ((bc_ & 0xff00) >> 8) + 1; // hold a temp variable with the incremented value - used for checking if flags need to be set
    
    // set flags
    ((b_inc) == 0) ? set_flag(CPU::flags::Z, 1) : set_flag(CPU::flags::Z, 0); // set Z if equal to 0
    set_flag(CPU::flags::N, 0); // set N
    (((bc_ & 0x0f00) + 1) > 0x0f00) ? set_flag(CPU::flags::H, 1) : set_flag(CPU::flags::H, 0); // if the lower 4 bits of B  causes carry, set H

    // set reg B to val of b_inc
    bc_ = (bc_ & 0xff) | (b_inc << 8); // clear the bc register to 0, then set register B to value of b_inc 

    return 0;
}
uint8_t CPU::INC_C() {}
uint8_t CPU::INC_D() {}
uint8_t CPU::INC_E() {}
uint8_t CPU::INC_H() {}
uint8_t CPU::INC_L() {}
uint8_t CPU::INC_HL_m() {}
uint8_t CPU::INC_A() {}
uint8_t CPU::DEC_H() {}
uint8_t CPU::DEC_D() {}
uint8_t CPU::DEC_E() {}
uint8_t CPU::DEC_B() {}
uint8_t CPU::DEC_C() {}
uint8_t CPU::DEC_L() {}
uint8_t CPU::DEC_HL_m() {}
uint8_t CPU::DEC_A() {}






uint8_t CPU::LD_B_d8() {}
uint8_t CPU::RLCA() {}
uint8_t CPU::LD_a16_m_SP() {}
uint8_t CPU::ADD_HL_BC() {}
uint8_t CPU::LD_A_BC_m() {}
uint8_t CPU::LD_C_d8() {}
uint8_t CPU::RRCA() {}
uint8_t CPU::STOP_0() {}
uint8_t CPU::LD_DE_d16() {}
uint8_t CPU::LD_DE_m_A() {}
uint8_t CPU::LD_D_d8() {}
uint8_t CPU::RLA() {}
uint8_t CPU::JR_s8() {}
uint8_t CPU::ADD_HL_DE() {}
uint8_t CPU::LD_A_DE_m() {}
uint8_t CPU::LD_E_d8() {}
uint8_t CPU::RRA() {}
uint8_t CPU::JR_NZ_s8() {}
uint8_t CPU::LD_HL_d16() {}
uint8_t CPU::LD_HLp_m_A() {}
uint8_t CPU::LD_H_d8() {}
uint8_t CPU::DAA() {}
uint8_t CPU::JR_Z_s8() {}
uint8_t CPU::ADD_HL_HL() {}
uint8_t CPU::LD_A_HLp_m() {}
uint8_t CPU::LD_L_d8() {}
uint8_t CPU::CPL() {}
uint8_t CPU::JR_NC_s8() {}
uint8_t CPU::LD_SP_d16() {}
uint8_t CPU::LD_HLm_m_A() {}
uint8_t CPU::LD_HL_m_d8() {}
uint8_t CPU::SCF() {}
uint8_t CPU::JR_C_s8() {}
uint8_t CPU::ADD_HL_SP() {}
uint8_t CPU::LD_A_HLm_m() {}
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
uint8_t CPU::CB_PREFIX() {}