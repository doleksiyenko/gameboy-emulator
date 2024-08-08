#ifndef SERIAL_H
#define SERIAL_H

#include <cstdint>

class Serial 
{
    public:
        uint8_t read_sb();
        void write_sb(uint8_t value);
        void write_sc(uint8_t value);

    private:
        uint8_t sb_ = 0;
        uint8_t sc_ = 0;
};

#endif