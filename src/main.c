#include <avr/io.h>
#include <util/delay.h>

#include "twi_lib.h"
#include "lcdTwi_lib.h"

TWI_LCD_CTXT_DECLARE(lcd, 0x27, 20, 4);

#include "beeper_lib.h"
BEEPER_CTXT_DECLARE(beeper, PORTD, 7, 1);


int main(void) {
    // Set pin 5 of PORTB as output
    DDRB |= (1 << DDB5);

    lcd_init(&lcd);
    lcd_backlight(&lcd, 1);

    lcd_gotoxy(&lcd, 6, 0);
    lcd_puts(&lcd, "UshanUshanUshanUshanUshan");
    lcd_gotoxy(&lcd, 4, 1);
    lcd_puts(&lcd, "Ushan");
    lcd_gotoxy(&lcd, 2, 2);
    lcd_puts(&lcd, "Ushan");
    lcd_gotoxy(&lcd, 0, 3);
    lcd_puts(&lcd, "Ushanchick");


    while (1) {
        // Toggle pin 5 of PORTB
        PORTB ^= (1 << PORTB5);
        _delay_ms(1000); // Wait for 1 second
    }

    return 0;
}