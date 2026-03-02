#ifndef _MOSFETCTL_LIB_H_
#define _MOSFETCTL_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>

typedef struct {
    volatile uint8_t *fetPulseDDR;
    volatile uint8_t *fetPulsePORT;
    uint8_t fetPulseBIT;
    bool activeState; // true for active HIGH, false for active LOW
    uint32_t pulseDurCnt; // Counter for pulse duration
    uint32_t pulseDurCntMax; // Maximum count for pulse duration (e.g., 20 for 1ms at 50us intervals)
} mosfetCtlCtxt_t;

#define MOSFET_CTL_CTXT_DECLARE(name, fetPulsePort, fetPulseBit, fetPulseActiveState) \
mosfetCtlCtxt_t name = { \
    .fetPulsePORT = &(fetPulsePort), \
    .fetPulseDDR = &(fetPulsePort) - 1, \
    .fetPulseBIT = (fetPulseBit), \
    .activeState = (fetPulseActiveState == 1) ? true : false, \
    .pulseDurCnt = 0, \
    .pulseDurCntMax = 0 \
}

#define MOSFET_DELAY_MS(x) do { \
        _delay_ms(x); \
    } while(0)

/**
 * @brief Initializes the MOSFET by setting the appropriate pin as output and
 * ensuring it is in the inactive state.
 * @param me Pointer to the MOSFET context structure containing pin configuration.
 * @note This function should be called before using the MOSFET to ensure it is
 * properly initialized.
 */
void mosfet_ctl_init(mosfetCtlCtxt_t *me);

/**
 * @brief Activates the MOSFET for a specified duration.
 * @param me Pointer to the MOSFET context structure containing pin configuration.
 * @param state Desired state of the MOSFET (true for active, false for inactive).
 * @note The function will block execution for the duration of the activation, so it
 * should be used with caution in time-sensitive applications.
 */
void mosfet_ctl_state(mosfetCtlCtxt_t *me, bool state);

/**
 * @brief Activates the MOSFET for a fixed duration (e.g., 1ms) using a timer.
 * @param me Pointer to the MOSFET context structure containing pin configuration.
 */
void mosfet_ctl_do_pulse(mosfetCtlCtxt_t *me);

/**
 * @brief Sets the duration for which the MOSFET should remain active when a pulse is triggered.
 * @param me Pointer to the MOSFET context structure containing pin configuration.
 * @param usec Duration in microseconds for which the MOSFET should remain active.
 */
void mosfet_ctl_set_pulse_duration(mosfetCtlCtxt_t *me, uint32_t usec);

#endif /* _MOSFETCTL_LIB_H_ */