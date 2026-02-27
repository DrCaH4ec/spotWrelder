#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "twi_lib.h"
#include "lcdTwi_lib.h"
TWI_LCD_CTXT_DECLARE(lcd, 0x27, 20, 4);

#include "beeper_lib.h"
BEEPER_CTXT_DECLARE(beeper, PORTD, 7, 1);

#include "mosfetCtl_lib.h"
MOSFET_CTL_CTXT_DECLARE(mosfet, PORTC, 1, 0);


ISR(TIMER1_CAPT_vect) {
    // Deactivate MOSFET when timer compare match occurs
    // mosfet_ctl_state(&mosfet, false);
    PORTC &= ~(1 << PORTC1); // Deactivate MOSFET by setting the pin low
    // Disable further interrupts until the next pulse
    TIMSK1 &= ~(1 << ICIE1);
    TCNT0 = 0; // Reset timer count
    TCCR1B = 0;
}


void do_pulse(void)
{
    // mosfet_ctl_state(&mosfet, true); // Activate MOSFET
    PORTC |= (1 << PORTC1); // Activate MOSFET by setting the pin high
    TCCR1B |= (1 << WGM13) | (1 << WGM12); // CTC mode, top ICR1
    TCCR1B |= (1 << CS11); // Prescaler 8
    ICR1 = 20000; // Set top value for 1ms at 16MHz with prescaler 8
    TCNT0 = 0; // Reset timer count
    // IRQ enable
    TIMSK1 |= (1 << ICIE1);
}


int main(void) {
    twi_init(TWI_PRESC_4, 18);

    lcd_init(&lcd);
    lcd_backlight(&lcd, 1);

    lcd_gotoxy(&lcd, 6, 0);
    lcd_puts(&lcd, "Ushan");
    lcd_gotoxy(&lcd, 4, 1);
    lcd_puts(&lcd, "Ushan");
    lcd_gotoxy(&lcd, 2, 2);
    lcd_puts(&lcd, "Ushan");
    lcd_gotoxy(&lcd, 0, 3);
    lcd_puts(&lcd, "Ushanchick");

    beeper_init(&beeper);
    beeper_beep(&beeper, 100); // Beep for 1 second

    mosfet_ctl_init(&mosfet);

    sei(); // Enable global interrupts

    while (1) {
        // mosfet_ctl_state(&mosfet, true); // Activate MOSFET
        // _delay_ms(1000); // Keep it active for 1 second
        // mosfet_ctl_state(&mosfet, false); // Deactivate MOSFET
        // _delay_ms(1000); // Wait for 1 second before the next activation

        do_pulse(); // Activate MOSFET for 1ms
        _delay_ms(20); // Wait for 10ms before the next pulse
    }

    return 0;
}