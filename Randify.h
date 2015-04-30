/**
 * TrueRandom - A true randomLong number generator for Arduino.
 *
 * Copyright (c) 2010 Peter Knight, Tinker.it! All rights reserved.
 */

#ifndef RANDIFY_H
#define RANDIFY_H

#include <inttypes.h>

class RandifyClass {
public:
    uint16_t randomWord();

    uint32_t randomDWord();

    long random(long howBig);

    long random(long howsmall, long how);

    uint8_t randomByte();

    void memfill(uint8_t *location, int size);

    void mac(uint8_t *macLocation);

    void uuid(uint8_t *uuidLocation);
};

extern RandifyClass Randify;
#endif
