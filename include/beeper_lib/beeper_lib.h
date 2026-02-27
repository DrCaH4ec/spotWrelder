#ifndef _BEEPER_LIB_H_
#define _BEEPER_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint8_t *dioDDR;
    volatile uint8_t *dioPORT;
    uint8_t dioBIT;
    bool activeState; // true for active HIGH, false for active LOW
} beeperCtxt_t;

#define BEEPER_CTXT_DECLARE(name, beeperPort, beeperBit, beeperActiveState) \
beeperCtxt_t name = { \
    .dioPORT = &(beeperPort), \
    .dioDDR = &(beeperPort) - 1, \
    .dioBIT = (beeperBit), \
    .activeState = (beeperActiveState == 1) ? true : false \
}

#define BEEPER_DELAY_MS(x) do { \
        _delay_ms(x); \
    } while(0)

/**
 * @brief Initializes the beeper by setting the appropriate pin as output and
 * ensuring it is in the inactive state.
 * @param me Pointer to the beeper context structure containing pin configuration.
 * @note This function should be called before using the beeper to ensure it is
 * properly initialized.
 */
void beeper_init(beeperCtxt_t *me);

/**
 * @brief Activates the beeper for a specified duration, creating a beep sound.
 * @param me Pointer to the beeper context structure containing pin configuration.
 * @param duration_ms Duration of the beep in milliseconds.
 * @note The function will block execution for the duration of the beep, so it
 * should be used with caution in time-sensitive applications.
 */
void beeper_beep(beeperCtxt_t *me, uint16_t duration_ms);

#endif /* _BEEPER_LIB_H_ */
