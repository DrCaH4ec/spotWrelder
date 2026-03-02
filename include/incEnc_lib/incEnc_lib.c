#include "incEnc_lib.h"

static const int8_t lut[16] = {
     0, +1, -1,  0,
    -1,  0,  0, +1,
    +1,  0,  0, -1,
     0, -1, +1,  0
};

/*----------------------------------------------------------------------------*/
/*                       Internal Functions Definition                        */
/*----------------------------------------------------------------------------*/
static inline uint8_t pinRead(volatile uint8_t* pinReg, uint8_t bit)
{
    return ((*pinReg) & (1 << bit)) ? 1 : 0;
}

/*----------------------------------------------------------------------------*/
/*                        Public Functions Definition                         */
/*----------------------------------------------------------------------------*/
void incEnc_init(incEncCtxt_t* me)
{
    // Set pins as input
    *(me->ddr_A) &= ~(1 << me->bit_A);
    *(me->ddr_B) &= ~(1 << me->bit_B);

    // Enable pull-up resistors
    *(me->port_A) |= (1 << me->bit_A);
    *(me->port_B) |= (1 << me->bit_B);
}

////////////////////////////////////////////////////////////////////////////////

void incEnc_tick(incEncCtxt_t* me)
{
    static uint8_t old = 0;
    static int8_t  accum = 0;

    uint8_t new_state = (uint8_t)((pinRead(me->pin_A, me->bit_A) << 1) |
                                   pinRead(me->pin_B, me->bit_B));

    int8_t d = lut[(old << 2) | new_state];
    old = new_state;

    if (d == 0) {
        me->moved = false;
        return;
    }

    accum += d;

    if (accum >= 4) {
        me->acc_counts_x4++;
        accum = 0;
        me->moved = true;
    } else if (accum <= -4) {
        me->acc_counts_x4--;
        accum = 0;
        me->moved = true;
    } else {
        me->moved = false; // був рух фази, але ще не назбиралося на "крок"
    }
}

////////////////////////////////////////////////////////////////////////////////

uint8_t incEnc_available(incEncCtxt_t* me)
{
    return me->moved;
}

////////////////////////////////////////////////////////////////////////////////

int8_t incEnc_getDelta(incEncCtxt_t* me)
{
    int16_t d = me->acc_counts_x4;
    me->acc_counts_x4 = 0;
    me->moved = false;
    return d;
}
