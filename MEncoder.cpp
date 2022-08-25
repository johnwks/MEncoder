#include "Arduino.h"
#include "MEncoder.h"


MEncoder::MEncoder(uint8_t _pin, uint8_t _num, uint16_t _maxval) : pin { _pin }, numpos { _num }, maxval { _maxval }
{

}


void MEncoder::init()
{
    pinMode(pin, INPUT_PULLUP);
    baseValue = analogRead(pin);
    for(int i = 0; i < 100 ; i++) {
        baseValue = (baseValue + analogRead(pin)) / 2;
    }
    state = ME_NONE;
}


void MEncoder::init(uint16_t _base)
{
    baseValue = _base;
    pinMode(pin, INPUT_PULLUP);
    value = analogRead(pin);
    position = findPosition(value);
    state = ME_NONE;
}


void MEncoder::process()
{
    uint8_t oldpos = position, tmp;

    value = analogRead(pin);
    tmp = findPosition(value);
    if (tmp == ME_INVALID_POS) {
        return;
    }
    position = tmp;
    if (position != oldpos) {
        if (IsCW(oldpos, position)) {
            state = ME_CW;
        } else {
            state = ME_CCW;
        }
    }
}


bool MEncoder::IsCW(int oldval, int newval)
{
    int MAX = numpos;
    int MID = numpos / 2;
    int old_max = ((oldval + MID) - (oldval + MAX)) % MAX;
    int old_new = (newval - (oldval + MAX)) % MAX;

    return (old_max <= MID) != (old_new > old_max);
}


bool MEncoder::IsBetween(int _min, int _max, int _val)
{
    int tmp;

    if ((_min < 0) && (_val > _max)) {
        tmp = maxval + _min;
        return (_val >= tmp);
    }

    if ((_max > maxval) && (_val < _min)) {
        tmp = _max - maxval;
        return (_val <= tmp);
    }

    return ((_val >= _min) && (_val <= _max));
}


uint8_t MEncoder::findPosition(uint16_t _val)
{
    bool found = false;
    uint8_t curpos = 0;
    uint16_t curbase, tmp;

    while ((!found) && (curpos < numpos)) {
        curbase = baseValue + (curpos * (maxval / numpos));
        tmp = (maxval / numpos) * ME_MULTIPLIER;
        found = IsBetween(curbase - tmp, curbase + tmp, _val);
        curpos++;
    }

    if (found) {
        return --curpos;
    }

    return ME_INVALID_POS;
}


void MEncoder::resetState()
{
    state = ME_NONE;
}


uint16_t MEncoder::readVal()
{
    value = analogRead(pin);
    for(int i = 0; i < 100 ; i++) {
        value = (value + analogRead(pin)) / 2;
    }

    return value;
}
