#ifndef TIMERS_H
#define TIMERS_H

#include <cstdint>

class Timers
{
    private:
        uint8_t div_ = 0x0;
        uint8_t tima_ = 0x0;
        uint8_t tma_ = 0x0;
        uint8_t tac_ = 0x0;
};

#endif