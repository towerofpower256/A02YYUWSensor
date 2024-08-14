#include <A02YYUWSensor.h>

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
            if (val <= A02YYUW_LOWLIMIT) {
                _error = A02YYUW_ERRORLOWLIMIT;
            } else if (val >= A02YYUW_HIGHLIMIT) {
                _error = A02YYUW_ERRORHIGHLIMIT;
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