#include "millis_lib.h"

volatile uint32_t tim0_ovfCnt = 0;
volatile uint32_t tim0_millis = 0;
static uint8_t tim0_fract = 0;

ISR(TIMER0_OVF_vect)
{
	uint32_t m = tim0_millis;
	uint8_t f = tim0_fract;

	m += MILLIS_INC;
	f += FRACT_INC;

	if(f >= FRACT_MAX) {
		f -= FRACT_MAX;
		m++;
	}

	tim0_fract = f;
	tim0_millis = m;
	tim0_ovfCnt++;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

void millis_init()
{
	TCCR0B |= (1 << CS01) | (1 << CS00);
	TIMSK0 = (1 << TOIE0);
	TCCR0A = 0x00;
	TCNT0 = 0x00;

	sei();
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t micros()
{
	uint32_t m;
	uint8_t SREG_prev = SREG;
	uint8_t t;

	cli();
	m = tim0_ovfCnt;
	t = TCNT0;

	if ((TIFR0 & (1 << TOV0)) && (t < 255))
		m++;

	SREG = SREG_prev;

	return ((m << 8) + t) * (64 / CLOCK_CYCLES_PER_USEC());
}

///////////////////////////////////////////////////////////////////////////////////////////////////

uint32_t millis()
{
	uint32_t m;
	uint8_t SREG_prev = SREG;

	cli();
	m = tim0_millis;

	SREG = SREG_prev;

	return m;
}
