#include "mosfetCtl_lib.h"

/*============================================================================*/
/*                      Internal functions definition                         */
/*============================================================================*/
void _setState(mosfetCtlCtxt_t *me, bool state) {
    if (state) {
        // Set to active state
        if (me->activeState) {
            // Active HIGH
            *(me->fetPulsePORT) |= (1 << me->fetPulseBIT);
        } else {
            // Active LOW
            *(me->fetPulsePORT) &= ~(1 << me->fetPulseBIT);
        }
    } else {
        // Set to inactive state
        if (me->activeState) {
            // Active HIGH
            *(me->fetPulsePORT) &= ~(1 << me->fetPulseBIT);
        } else {
            // Active LOW
            *(me->fetPulsePORT) |= (1 << me->fetPulseBIT);
        }
    }
}

/*============================================================================*/
/*                       Public functions definition                          */
/*============================================================================*/
void mosfet_ctl_init(mosfetCtlCtxt_t *me) {
    // Set the MOSFET control pin as output
    *(me->fetPulseDDR) |= (1 << me->fetPulseBIT);

    // Set the MOSFET to inactive state
    _setState(me, false);
}

////////////////////////////////////////////////////////////////////////////////

void mosfet_ctl_state(mosfetCtlCtxt_t *me, bool state)
{
    // Set the MOSFET to the desired state
    _setState(me, state);
}
