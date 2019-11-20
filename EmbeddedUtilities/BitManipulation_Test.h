//
// Created by annika on 20.11.19.
//

#ifndef EMBEDDEDUTILITIES_BITMANIPULATION_TEST_H
#define EMBEDDEDUTILITIES_BITMANIPULATION_TEST_H

#include <stdint.h>

void BitManipulation_clearBit(volatile uint8_t *byte_ptr, uint8_t offset);

void BitManipulation_setBit(volatile uint8_t *byte_ptr, uint8_t offset);

#endif //EMBEDDEDUTILITIES_BITMANIPULATION_TEST_H
