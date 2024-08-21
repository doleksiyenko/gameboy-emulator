#ifndef TIMERS_H
#define TIMERS_H

#include <cstdint>

class Bus;

class Timers
{
    public:
        void increment_cycle_counter();
        void connect_bus(Bus* bus);

        void write(uint16_t address, uint8_t value);
        void write_div(); // writing any value to this register resets it to 0
        void increment_tima();
        void write_tma(uint8_t value);
        void write_tac(uint8_t value);
        void write_tima(uint8_t value);

        uint8_t read(uint16_t address);
        uint8_t read_div();
        uint8_t read_tima();
        uint8_t read_tma();
        uint8_t read_tac();

    private:
        Bus* bus_; // connect to Bus to request timer interrupts
        int t_cycle_counter = 0; // count t cycles so that we can update the div register and tima registers based on M cycle count

        uint16_t div_ = 0x0; // the actual DIV register is only the top 8 bits of the system clock
        uint8_t tima_ = 0x0;
        uint8_t tma_ = 0x0;
        uint8_t tac_ = 0x0;

        uint8_t prev_cycle_AND_result = 0;
};

#endif