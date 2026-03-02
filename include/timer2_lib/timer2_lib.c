#include "timer2_lib.h"

static void (*_handler) (void);

ISR(TIMER2_COMPA_vect) {
    _handler();
}

void tim2_set_prescaler(enum tim2_presc_t presc_type)
{
    TCCR2B &= 0xf8;
    TCCR2B |= presc_type;
}

//-------------------------------------------------------------------------------------------------

void tim2_waveform_gen_mode(enum tim2_mode_t mode)
{

    TCCR2A &= 0xfc;
    TCCR2A |= mode & 0x03;

    TCCR2B &= 0xf7;
    TCCR2B |= ((mode&(0x04))<<1);
}

//-------------------------------------------------------------------------------------------------

void tim2_comp_out_mode_set_a(enum tim2_comp_out_mode_t mode)
{
    TCCR2A &= 0x3f;
    TCCR2A |= (mode<<6);
}

//-------------------------------------------------------------------------------------------------

void tim2_comp_out_mode_set_b(enum tim2_comp_out_mode_t mode)
{
    TCCR2A &= 0xcf;
    TCCR2A |= (mode<<4);
}

//-------------------------------------------------------------------------------------------------

void tim2_int_mask_set(uint8_t mask)
{
    TIMSK2 = 0x00;
    TIMSK2 |= mask;
}

//-------------------------------------------------------------------------------------------------

void tim2_compa_attach_handler(void (*foo)(void))
{
    _handler = foo;
}

//-------------------------------------------------------------------------------------------------

void tim2_compa_set_value(uint8_t value)
{
    OCR2A = value;
}
