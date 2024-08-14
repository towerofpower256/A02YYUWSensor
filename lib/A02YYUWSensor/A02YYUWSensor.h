
// TODO: do I need to handle message timeouts? If there's a broken message and silence for 1 second, should I reset.

#pragma once
#include <stdint.h>

#define A02YYUW_START 0xFF
#define A02YYUW_LOWLIMIT 30 // Value should never be below this
#define A02YYUW_ERRORCHECKSUM 1
#define A02YYUW_ERRORLOWLIMIT 2

class A02YYUWSensor {
    public:
    void begin();
    bool read(uint8_t c);
    void resetHaveVal() { _haveVal = false; };
    bool haveVal() { return _haveVal; };
    uint16_t getVal() { return _val; };
    uint8_t getError() { return _error; };
    uint8_t resetError() { _error = 0; };

    protected:
    uint16_t _val;
    uint8_t _bufH;
    uint8_t _bufL;
    uint8_t _idx;
    bool _haveVal;
    uint8_t _checksum();
    uint8_t _error;
};

void A02YYUWSensor::begin() {
    _haveVal = false;
    _idx = 0;
    _error = 0;
}

bool A02YYUWSensor::read(uint8_t c) {
    if (_idx == 0) {
        // Waiting for the start char
        if (c == A02YYUW_START) {
            _idx = 1;
        } else {
            return false;
        }
    }
    else if (_idx == 1) {
        _bufH = c;
        _idx=2;
    }
    else if (_idx == 2) {
        _bufL = c;
        _idx=3;
    }
    else if (_idx >= 3) {
        // Reading checksum, or overflow
        if (_checksum() == c) {
            // Checksum good, save value
            uint16_t val = (_bufH << 8) + _bufL;
            if (val == A02YYUW_LOWLIMIT) {
                _error = A02YYUW_ERRORLOWLIMIT;
            } else {
                // Good value, lets save it
                _haveVal = true;
                _val = val;
                
                // Reset
                _idx = 0;
                return true;
            }
        } else {
            _error = A02YYUW_ERRORCHECKSUM;
        }

        // Reset
        _idx = 0;
    }

    
    return false;
};

uint8_t A02YYUWSensor::_checksum() {
    return ((A02YYUW_START + _bufH + _bufL)& 0x00FF);
}