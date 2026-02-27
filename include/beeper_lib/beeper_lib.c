#include "beeper_lib.h"

/*============================================================================*/
/*                      Internal functions definition                         */
/*============================================================================*/
static void _setState(beeperCtxt_t *me, bool state) {
    if (state) {
        // Set to active state
        if (me->activeState) {
            // Active HIGH
            *(me->port) |= (1 << me->bit);
        } else {
            // Active LOW
            *(me->port) &= ~(1 << me->bit);
        }
    } else {
        // Set to inactive state
        if (me->activeState) {
            // Active HIGH
            *(me->port) &= ~(1 << me->bit);
        } else {
            // Active LOW
            *(me->port) |= (1 << me->bit);
        }
    }
}

/*============================================================================*/
/*                       Public functions definition                          */
/*============================================================================*/
void beeper_init(beeperCtxt_t *me) {
    // Set the beeper pin as output
    *(me->ddr) |= (1 << me->bit);

    // Set the beeper to inactive state
    _setState(me, false);
}

////////////////////////////////////////////////////////////////////////////////

void beeper_beep(beeperCtxt_t *me, uint16_t duration_ms) {
    // Set the beeper to active state
    _setState(me, true);
    // Wait for the specified duration
    BEEPER_DELAY_MS(duration_ms);
    // Set the beeper back to inactive state
    _setState(me, false);
}
