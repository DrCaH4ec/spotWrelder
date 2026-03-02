#include "mosfetCtl_lib.h"

static mosfetCtlCtxt_t *tmpMe;

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

ISR(TIMER1_COMPA_vect) {
    if (++tmpMe->pulseDurCnt >= tmpMe->pulseDurCntMax) {
        TIMSK1 &= ~(1 << OCIE1A); // Disable Timer1 output compare A match interrupt
        tmpMe->pulseDurCnt = 0;
        // Deactivate MOSFET when timer compare match occurs
        _setState(tmpMe, false);

        tmpMe = NULL; // Clear the temporary context pointer
    } else {
        return; // Skip deactivation until we've reached the desired count
    }
}

/*============================================================================*/
/*                       Public functions definition                          */
/*============================================================================*/
void mosfet_ctl_init(mosfetCtlCtxt_t *me) {
    // Set the MOSFET control pin as output
    *(me->fetPulseDDR) |= (1 << me->fetPulseBIT);

    me->pulseDurCnt = 0; // Initialize pulse duration counter

    // Set the MOSFET to inactive state
    _setState(me, false);

    // Configure Timer1 for CTC mode with a prescaler of 8
    TCCR1B |= (1 << WGM12); // CTC mode, top OCR1A
    TCCR1B |= (1 << CS11); // Prescaler 8
    OCR1A = 99; // Set top value for 1ms at 16MHz with prescaler 8 (16,000,000 / 8 = 2,000,000 counts per second, so 1ms = 2,000 counts)
    TCNT1 = 0; // Reset timer count
}

////////////////////////////////////////////////////////////////////////////////

void mosfet_ctl_state(mosfetCtlCtxt_t *me, bool state)
{
    // Set the MOSFET to the desired state
    _setState(me, state);
}

////////////////////////////////////////////////////////////////////////////////

void mosfet_ctl_do_pulse(mosfetCtlCtxt_t *me)
{
    if (me == NULL || me->pulseDurCntMax == 0) {
        return; // Invalid context, do nothing
    }

    tmpMe = me; // Store the context for use in the ISR
    me->pulseDurCnt = 0; // Reset pulse duration counter

    _setState(me, true); // Activate MOSFET
    TCNT1 = 0; // Reset timer count
    TIMSK1 |= (1 << OCIE1A); // IRQ enable
}

////////////////////////////////////////////////////////////////////////////////

void mosfet_ctl_set_pulse_duration(mosfetCtlCtxt_t *me, uint32_t usec)
{
    if (me == NULL) {
        return; // Invalid context, do nothing
    }

    me->pulseDurCntMax = (usec + 49) / 50; // Set the maximum count for pulse duration (e.g., 20 for 1ms at 50us intervals)
}
