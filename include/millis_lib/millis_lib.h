#ifndef _MILLIS_LIB_H_
#define _MILLIS_LIB_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <avr/interrupt.h>

#define CLOCK_CYCLES_PER_USEC() ( F_CPU / 1000000L )

#define CLOCK_CYCLES_TO_USEC(a) ( (a) / CLOCK_CYCLES_PER_USEC() )

#define MICROSECONDS_PER_TIMER0_OVERFLOW (CLOCK_CYCLES_TO_USEC(64 * 256))

// the whole number of milliseconds per timer0 overflow
#define MILLIS_INC (MICROSECONDS_PER_TIMER0_OVERFLOW / 1000)

// the fractional number of milliseconds per timer0 overflow. we shift right
// by three to fit these numbers into a byte. (for the clock speeds we care
// about - 8 and 16 MHz - this doesn't lose precision.)
#define FRACT_INC ((MICROSECONDS_PER_TIMER0_OVERFLOW % 1000) >> 3)
#define FRACT_MAX (1000 >> 3)

/**
 * @brief Initializes the millis and micros timer functionality.
 */
void millis_init();

/**
 * @brief Returns the number of microseconds since the program started.
 * @return The number of microseconds.
 */
uint32_t micros();

/**
 * @brief Returns the number of milliseconds since the program started.
 * @return The number of milliseconds.
 */
uint32_t millis();

#endif // _MILLIS_LIB_H_
