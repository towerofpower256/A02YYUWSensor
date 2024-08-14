// TODO: do I need to handle message timeouts? If there's a broken message and silence for 1 second, should I reset.

#pragma once
#ifndef A02YYUWSensor_h
#define A02YYUWSensor_h

#include <stdint.h>

#define A02YYUW_START 0xFF
#define A02YYUW_LOWLIMIT 30 // Value should never be below this
#define A02YYUW_HIGHLIMIT 4500 // Value should never be above this
#define A02YYUW_ERRORCHECKSUM 1
#define A02YYUW_ERRORLOWLIMIT 2
#define A02YYUW_ERRORHIGHLIMIT 3

class A02YYUWSensor {
    public:
    void begin();
    bool read(uint8_t c);
    void resetHaveVal() { _haveVal = false; };
    bool haveVal() { return _haveVal; };
    uint16_t getVal() { return _val; };
    uint8_t getError() { return _error; };
    void resetError() { _error = 0; };

    protected:
    uint16_t _val;
    uint8_t _bufH;
    uint8_t _bufL;
    uint8_t _idx;
    bool _haveVal;
    uint8_t _checksum();
    uint8_t _error;
};

#endif