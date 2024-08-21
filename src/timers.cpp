#include "timers.h"
#include "bus.h"
#include <cstdint>

void Timers::connect_bus(Bus *bus)
{
    bus_ = bus;
}

void Timers::increment_cycle_counter()
{
    t_cycle_counter += 1;
    // though we update the 16 bit register every t_cycle, since only the top 8 bits are exposed,
    // this is in effect updating every 64 M cycles
    div_ += 1; 

    // after every increment of the div register, it is possible for the TIMA register to
    // be incremented once based on the bits in the div register
    uint8_t bit_value;
    switch (tac_ & 0b11) {
        case 0b00:
            bit_value = (div_ & (1 << 9)) >> 9;
            break;
        case 0b01:
            bit_value = (div_ & (1 << 3)) >> 3;
            break;
        case 0b10:
            bit_value = (div_ & (1 << 5)) >> 5;
            break;
        case 0b11:
            bit_value = (div_ & (1 << 7)) >> 7;
            break;
    }

    // check for a falling edge in the result of the AND of the select div_ bit and the timer enable
    uint8_t timer_enable = (tac_ & 0b100) >> 2;
    if ((bit_value & timer_enable) == 0) {
        if (prev_cycle_AND_result == 1) {
            increment_tima();
        }
    }

    prev_cycle_AND_result = bit_value & timer_enable;

    // the TAC register controls whether the TIMA timer is counting, and at which clock rate 
    if (timer_enable) {
        switch (tac_ & 0b11) { // check clock select
            case 0b00:
                if (t_cycle_counter % (256 * 4) == 0) 
                {
                    increment_tima();
                }
                break;
            case 0b01:
                if (t_cycle_counter % (4 * 4) == 0)
                {
                    increment_tima();
                }
                break;
            case 0b10:
                if (t_cycle_counter % (16 * 4) == 0)
                {
                    increment_tima();
                }
                break;
            case 0b11:
                if (t_cycle_counter % (64 * 4) == 0)
                {
                    increment_tima();
                }
                break;
        }
    }
}

void Timers::increment_tima()
{
    if (tima_ == 0xff) {
        // request timer interrupt
        uint8_t interrupt_flag = bus_->read(0xff0f);
        interrupt_flag |= (1 << 2); // update the timer flag IF
        bus_->write(0xff0f, interrupt_flag); 
        tima_ = tma_;
    }
    else {
        tima_++;
    }
}

void Timers::write(uint16_t address, uint8_t value)
{
    if (address == 0xff04) {
        write_div();
    }
    else if (address == 0xff05) {
        write_tima(value);
    }
    else if (address == 0xff06) {
        write_tma(value);
    }
    else if (address == 0xff07) {
        write_tac(value);
    }
}

uint8_t Timers::read(uint16_t address)
{
    if (address == 0xff04) {
        return read_div();
    }
    else if (address == 0xff05) {
        return read_tima();
    }
    else if (address == 0xff06) {
        return read_tma();
    }
    else if (address == 0xff07) {
        return read_tac();
    }

    return 0xff;
}

void Timers::write_div()
{
    div_ = 0;
}

void Timers::write_tma(uint8_t value)
{
    tma_ = value;
}

void Timers::write_tac(uint8_t value)
{
    tac_ = value;
}

void Timers::write_tima(uint8_t value)
{
    tima_ = value;
}

uint8_t Timers::read_div()
{
    return div_ >> 8;
}
uint8_t Timers::read_tima()
{
    return tima_;
}
uint8_t Timers::read_tma()
{
    return tma_;
}
uint8_t Timers::read_tac()
{
    return tac_;
}