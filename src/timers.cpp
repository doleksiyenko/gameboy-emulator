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

    // every 64 M-cycles, increment the DIV register
    if (t_cycle_counter % (64 * 4) == 0) {
        // TODO: check if we are in stop mode
        div_ += 1; 
    }

    // the TAC register controls whether the TIMA timer is counting, and at which clock rate 
    if (tac_ & 0b100) { // check enable bit
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
    if (tima_ + 1 > 0xff) {
        // request timer interrupt
        bus_->write(0xff0f, 4);
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


uint8_t Timers::read_div()
{
    return div_;
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