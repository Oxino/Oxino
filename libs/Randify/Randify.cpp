/**
 * TrueRandom - A true randomLong number generator for Arduino.
 *
 * Copyright (c) 2010 Peter Knight, Tinker.it! All rights reserved.
 */

#include <Arduino.h>
#include "Randify.h"

long ard_random(long howbig) {
    randomSeed((analogRead(A0) << random(16)) + (analogRead(A1) << random(16)) + (analogRead (A2) >> random(16)));
    return random(howbig);
}

long ard_random(long howsmall, long howbig) {
    randomSeed(analogRead(A0));
    return random(howsmall, howbig);
}

uint8_t RandifyClass::randomByte() {
    long result = ard_random(256);
    return (uint8_t) (result & 0xFF);
}

uint16_t RandifyClass::randomWord() {
    uint16_t result = randomByte();
    result <<= 8;
    result |= randomByte();
    return result;
}

uint32_t RandifyClass::randomDWord() {
    uint32_t result = randomWord();
    result <<= 16;
    result |= randomWord();
    return result;
}

long RandifyClass::random(long howBig) {
    return ard_random(howBig);
}

long RandifyClass::random(long howSmall, long howBig) {
    return ard_random(howSmall, howBig);
}

void RandifyClass::memfill(uint8_t *location, int size) {
    for (;size--;) *location++ = randomByte();
}

void RandifyClass::mac(uint8_t* macLocation) {
    memfill((uint8_t *)macLocation,6);
}

void RandifyClass::uuid(uint8_t* uuidLocation) {
    // Generate a Version 4 UUID according to RFC4122
    memfill((uint8_t *)uuidLocation,16);
    // Although the UUID contains 128 bits, only 122 of those are randomLong.
    // The other 6 bits are fixed, to indicate a version number.
    uuidLocation[6] = 0x40 | (0x0F & uuidLocation[6]);
    uuidLocation[8] = 0x80 | (0x3F & uuidLocation[8]);
}

RandifyClass Randify;
