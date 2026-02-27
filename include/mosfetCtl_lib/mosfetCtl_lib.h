#ifndef _MOSFETCTL_LIB_H_
#define _MOSFETCTL_LIB_H_

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint8_t *fetPulseDDR;
    volatile uint8_t *fetPulsePORT;
    uint8_t fetPulseBIT;
    bool activeState; // true for active HIGH, false for active LOW
} mosfetCtlCtxt_t;

#define MOSFET_CTL_CTXT_DECLARE(name, fetPulsePort, fetPulseBit, fetPulseActiveState) \
mosfetCtlCtxt_t name = { \
    .fetPulsePORT = &(fetPulsePort), \
    .fetPulseDDR = &(fetPulsePort) - 1, \
    .fetPulseBIT = (fetPulseBit), \
    .activeState = (fetPulseActiveState == 1) ? true : false \
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

#endif /* _MOSFETCTL_LIB_H_ */