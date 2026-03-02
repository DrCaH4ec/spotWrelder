#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "twi_lib.h"
#include "lcdTwi_lib.h"
TWI_LCD_CTXT_DECLARE(lcd, 0x27, 20, 4);

#include "beeper_lib.h"
BEEPER_CTXT_DECLARE(beeper, PORTD, 7, 1);

#include "mosfetCtl_lib.h"
MOSFET_CTL_CTXT_DECLARE(mosfet, PORTC, 1, 1);

#include "incEnc_lib.h"
INC_ENC_DECLARE_CTXT(enc, PORTD, 3, PORTD, 4);

#include "button_lib.h"
BUTTON_DECLARE_CTXT(encBtn, PORTD, 2, 0, 0);
BUTTON_DECLARE_CTXT(pulseBtn, PORTD, 1, 0, 0);

#include "extInt_lib.h"
#include "millis_lib.h"
#include "timer2_lib.h"

bool pulseBlock = false;
uint32_t pulseBlock_millis = 0;
uint32_t EEMEM readPulseDuration = 0;
uint32_t pulseDuration = 0;
bool pulseDurationChanged = false;

void tim2_handler(void) {
    incEnc_tick(&enc);
}

int main(void) {
    twi_init(TWI_PRESC_4, 18);

    lcd_init(&lcd);
    lcd_backlight(&lcd, 1);

    beeper_init(&beeper);
    beeper_beep(&beeper, 100); // Beep for 100 milliseconds

    tim2_set_prescaler(TIM2_PRESC_1024);
    tim2_waveform_gen_mode(TIM2_CTC);
    tim2_int_mask_set(TIM2_OUT_COMP_A);
    tim2_compa_set_value(77); // Set Compare Match Value for 5ms (16MHz / 1024 / 200Hz = 78.125)
    tim2_compa_attach_handler(tim2_handler);

    millis_init();
    incEnc_init(&enc);
    button_initHw(&encBtn);
    button_initHw(&pulseBtn);

    lcd_gotoxy(&lcd, 6, 0);
    lcd_puts(&lcd, "Ushan");
    _delay_ms(1000);

    pulseDuration = eeprom_read_dword(&readPulseDuration);

    mosfet_ctl_init(&mosfet);
    mosfet_ctl_set_pulse_duration(&mosfet, pulseDuration); // Set pulse duration from EEPROM

    char buffer[20];
    lcd_clear(&lcd);
    snprintf(buffer, sizeof(buffer), "T: %ld.%01ld ms", pulseDuration / 1000, (pulseDuration % 1000) / 100);
    lcd_gotoxy(&lcd, 0, 0);
    lcd_puts(&lcd, buffer);

    sei(); // Enable global interrupts

    while (1) {

        if (incEnc_available(&enc)) {
            lcd_clear(&lcd);
            pulseDuration += (incEnc_getDelta(&enc)) * 200;
            
            snprintf(buffer, sizeof(buffer), "T: %ld.%01ld ms", pulseDuration / 1000, (pulseDuration % 1000) / 100);
            lcd_gotoxy(&lcd, 0, 0);
            lcd_puts(&lcd, buffer);
            mosfet_ctl_set_pulse_duration(&mosfet, pulseDuration);
            pulseDurationChanged = true;
        }

        if (button_isPressed(&encBtn) && !pulseBlock) {
            beeper_beep(&beeper, 25); // Beep for 25 milliseconds
            mosfet_ctl_do_pulse(&mosfet);
            pulseBlock = true;
            pulseBlock_millis = millis();
            if (pulseDurationChanged) {
                eeprom_update_dword(&readPulseDuration, pulseDuration); // Save pulse duration to EEPROM
                pulseDurationChanged = false;
            }
        }

        if (pulseBlock && (millis() - pulseBlock_millis >= 500) && !button_isPressed(&encBtn)) {
            pulseBlock_millis = millis();
            pulseBlock = false;
        }
    }

    return 0;
}
