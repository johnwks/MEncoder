#ifndef MENCODER_H
#define MENCODER_H

#include "Arduino.h"

#define ME_NONE     0x0
#define ME_CW       0x1
#define ME_CCW      0x2

#define ME_MULTIPLIER  0.4
#define ME_INVALID_POS 255


class MEncoder
{
    public:
        MEncoder(uint8_t, uint8_t, uint16_t);
        void init();
        void init(uint16_t);
        void process();
        void resetState();
        bool IsCW(int, int);
        bool IsBetween(int, int, int);
        uint8_t findPosition(uint16_t);
        uint16_t readVal();

        uint8_t pin;
        uint8_t numpos;
        uint8_t position;
        uint8_t state;
        uint16_t baseValue;
        uint16_t value;
        uint16_t maxval;
};

#endif
