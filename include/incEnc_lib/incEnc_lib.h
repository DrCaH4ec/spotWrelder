#ifndef _INC_ENC_LIB_H_
#define _INC_ENC_LIB_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    volatile uint8_t *port_A;
    volatile uint8_t *ddr_A;
    volatile uint8_t *pin_A;
    uint8_t bit_A;

    volatile uint8_t *port_B;
    volatile uint8_t *ddr_B;
    volatile uint8_t *pin_B;
    uint8_t bit_B;

    uint8_t upState;
    uint8_t downState;
    uint8_t prevState;
    volatile uint8_t moved;
    int16_t acc_counts_x4;
} incEncCtxt_t;

#define INC_ENC_DECLARE_CTXT(name, portA, bitA, portB, bitB) \
    incEncCtxt_t name = { \
        /* DDR register is PORT - 1 */ \
        /* PIN register is PORT - 2 */ \
        .port_A = &(portA), \
        .ddr_A = &(portA) - 1, \
        .pin_A = &(portA) - 2, \
        .bit_A = (bitA), \
        /* Pin B setup */ \
        .port_B = &(portB), \
        .ddr_B = &(portB) - 1, \
        .pin_B = &(portB) - 2, \
        .bit_B = (bitB), \
        /* Internal state */ \
        .upState = 0, \
        .downState = 0, \
        .prevState = 0, \
        .moved = 0, \
        .acc_counts_x4 = 0 \
    };

/**
 * @brief Initializes the incremental encoder context.
 * @param ctxt Pointer to the encoder context to initialize.
 */
void incEnc_init(incEncCtxt_t* me);

/**
 * @brief Tick function to be called periodically to update encoder state.
 * @param ctxt Pointer to the encoder context.
 */
void incEnc_tick(incEncCtxt_t* me);

uint8_t incEnc_available(incEncCtxt_t* me);

int8_t incEnc_getDelta(incEncCtxt_t* me);

#endif // _INC_ENC_LIB_H_
