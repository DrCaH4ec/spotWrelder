#ifndef _LCD_H_
#define _LCD_H_

//you can change it if you need
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

//-----------------------------------------------------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <util/delay.h>

#include "twi_lib.h"

#define TWI_LCD_DEFAULT_ADDR 	0x27

typedef struct {
    uint8_t addr;
    uint8_t lines;
    uint8_t cols;
    uint8_t lcd_reg;
} twiLcdCtxt_t;

#define TWI_LCD_CTXT_DECLARE(name, lcdAddr, lcdCols, lcdLines) \
twiLcdCtxt_t name = { \
    .addr = lcdAddr, \
    .lines = lcdLines, \
    .cols = lcdCols, \
    .lcd_reg = 0x00 \
}

//-----------------------------------------------------------------------------

/**
 * This function is used for initialisation of display. It initialises lcd in
 * 4-bit mode.
 * 
 * @param me pointer to struct with pins to which LCD is connected
 * @param NumOfLines number of lines of current LCD
 */
void lcd_init(twiLcdCtxt_t *me);

/**
 * Clear display and set cursor to position (0, 0)
 * @param me pointer to struct with pins to which LCD is connected
 */
void lcd_clear(twiLcdCtxt_t *me);

/**
 * Set cursor to position (0, 0) without changing data on display
 * @param me pointer to struct with pins to which LCD is connected
 */
void lcd_home(twiLcdCtxt_t *me);

/**
 * Set cursor to position (x, y) without changing data on display
 * @param me pointer to struct with pins to which LCD is connected
 * @param x   new horisontal position of cursor on display
 * @param y   new vertical position of cursor on display
 */
void lcd_gotoxy(twiLcdCtxt_t *me, uint8_t x, uint8_t y);

/**
 * Print a string to display
 * @param me pointer to struct with pins to which LCD is connected
 * @param data pointer to first element of string which will be
 * printed do display
 */
void lcd_puts(twiLcdCtxt_t *me, char data[]);

/**
 * Print only 1 character to display
 * @param me  pointer to struct with pins to which LCD is connected
 * @param data ASCII code of character or addres of custom character
 * in CGRAM
 */
void lcd_putChar(twiLcdCtxt_t *me, char data);

/**
 * Makes corsor visible on display
 * @param me  pointer to struct with pins to which LCD is connected
 * @param mode if mode = 1 cursor is visible, else cursor is invisible
 */
void lcd_cursor(twiLcdCtxt_t *me, bool mode);

/**
 * Makes corsor blinking on display
 * @param me  pointer to struct with pins to which LCD is connected
 * @param mode if mode = 1 cursor is blinking, else cursor isn't blinking
 */
void lcd_cursorBlink(twiLcdCtxt_t *me, bool mode);

/**
 * Shift all symbols on display on 1 position to left
 * @param me pointer to struct with pins to which LCD is connected
 */
void lcd_shiftDispLeft(twiLcdCtxt_t *me);

/**
 * Shift all symbols on display on 1 position to right
 * @param me pointer to struct with pins to which LCD is connected
 */
void lcd_shiftDispRight(twiLcdCtxt_t *me);

/**
 * Write to CGRAM custom character with size 5x8 dots
 * @param me      pointer to struct with pins to which LCD is connected
 * @param data     pointer to array with definition of new character
 * @param location addres of new character in CGRAM(can be from 0 to 7)
 */
void lcd_createChar(twiLcdCtxt_t *me, uint8_t data[], uint8_t location);

/**
 * Turn on or turn off a backlight
 * @param me pointer to struct with pins to which LCD is connected
 * @param mode ON or OFF
 */
void lcd_backlight(twiLcdCtxt_t *me, bool mode);

/*_LCD_H_*/
#endif
