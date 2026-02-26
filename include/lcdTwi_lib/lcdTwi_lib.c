#include "lcdTwi_lib.h"

static uint8_t lcd_reg = 0x00;

#define LOW 0
#define HIGH 1

#define LCD_DELAY_US(x) do { \
		_delay_us(x); \
	} while(0)

#define LCD_DELAY_MS(x) do { \
		_delay_ms(x); \
	} while(0)

//#############################################################################
//#####################---INTERNAL FUNCTIONS---################################
//#############################################################################

static void pulseEn()
{
	lcd_reg &= ~LCD_E;
    twi_write_data(ADDR, lcd_reg, 0x00, 0);
	LCD_DELAY_US(10);
	lcd_reg |= LCD_E;
  	twi_write_data(ADDR, lcd_reg, 0x00, 0);
	LCD_DELAY_US(10);		
	lcd_reg &= ~LCD_E;
    twi_write_data(ADDR, lcd_reg, 0x00, 0);
	LCD_DELAY_US(100);
}

//-----------------------------------------------------------------------------

// part=1 -> high
// part=0 -> low
static void sendHalfByte(uint8_t data, bool part)
{

	lcd_reg &= ~(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);

	if(part){

		lcd_reg = data&(1<<4) ? lcd_reg|LCD_D4 : lcd_reg;
		lcd_reg = data&(1<<5) ? lcd_reg|LCD_D5 : lcd_reg;
		lcd_reg = data&(1<<6) ? lcd_reg|LCD_D6 : lcd_reg;
		lcd_reg = data&(1<<7) ? lcd_reg|LCD_D7 : lcd_reg;


	}
	else{

		lcd_reg = data&(1<<0) ? lcd_reg|LCD_D4 : lcd_reg;
		lcd_reg = data&(1<<1) ? lcd_reg|LCD_D5 : lcd_reg;
		lcd_reg = data&(1<<2) ? lcd_reg|LCD_D6 : lcd_reg;
		lcd_reg = data&(1<<3) ? lcd_reg|LCD_D7 : lcd_reg;

	}

	pulseEn();
}

//-----------------------------------------------------------------------------

static void sendInst(uint8_t data)
{
	lcd_reg &= ~LCD_RS;

	sendHalfByte(data, HIGH);
	sendHalfByte(data, LOW);	
}

//-----------------------------------------------------------------------------

static void sendData(uint8_t data)
{
	lcd_reg |= LCD_RS;

	LCD_DELAY_US(1);

	sendHalfByte(data, HIGH);
	sendHalfByte(data, LOW);

	LCD_DELAY_US(50);
}

//#############################################################################
//#####################---EXTERNAL FUNCTIONS---################################
//#############################################################################

void lcd_backlight(bool mode)
{
	lcd_reg = mode ? lcd_reg|LCD_LED : lcd_reg&(~LCD_LED);

	twi_write_data(ADDR, lcd_reg, 0x00, 0);
}

//-----------------------------------------------------------------------------

void lcd_clear()
{
	sendInst(CLEAR_DISPLAY);
	LCD_DELAY_MS(2);
}

//-----------------------------------------------------------------------------

void lcd_home()
{
	sendInst(RETURN_HOME);
	LCD_DELAY_MS(2);
}

//-----------------------------------------------------------------------------

void lcd_init(twiLcdCtxt_t *lcd)
{
	uint8_t tmp = 0x00;

	LCD_DELAY_MS(50);

	tmp = FUNCTION_SET | DL;
	sendHalfByte(tmp, HIGH);
	LCD_DELAY_MS(5);
	sendHalfByte(tmp, HIGH);
	LCD_DELAY_US(150);
	sendHalfByte(tmp, HIGH);
	LCD_DELAY_US(50);

	tmp = 0x00;
	tmp |= FUNCTION_SET;
	sendHalfByte(tmp, HIGH);

	//2 lines mode set
	//5x8 dots
	tmp = lcd->lines > 1 ? (tmp | N) : tmp;
	sendInst(tmp);
	sendInst(tmp);

	tmp = 0x00;
	tmp = DISPLAY_ON_OFF_CONTROL | D;
	sendInst(tmp);

	tmp = 0x00;
	tmp = ENTRY_MODE_SET | I_D;
	sendInst(tmp);

	lcd_clear();	
}

//-----------------------------------------------------------------------------

void lcd_gotoxy(twiLcdCtxt_t *me, uint8_t x, uint8_t y)
{
	if (y >= me->lines || x >= me->rows)
		return;

	uint8_t tmp = SET_DDRAM_ADDRES;

	if (me->lines <= 2) {
		tmp |= y == 0 ? 0x00 : 0x40;
	}
	else {
		switch (y) {
			case 0:
				tmp |= 0x00;
			break;

			case 1:
				tmp |= 0x40;
			break;

			case 2:
				tmp |= 0x14;
			break;

			case 3:
				tmp |= 0x54;
			break;
		
			default:
			break;
		}
	}

	tmp += x;

	sendInst(tmp);
	LCD_DELAY_US(100);
}

//-----------------------------------------------------------------------------

void lcd_puts(char data[])
{
	uint8_t i = 0;

	while(data[i] != '\0' && i < 16){
		sendData(data[i]);
		i++;
	}
}

//-----------------------------------------------------------------------------

void lcd_put_Char(char data)
{
	sendData(data);
}

//-----------------------------------------------------------------------------

void lcd_cursor(bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL | D;

	tmp = mode ? (tmp | C) : tmp;

	sendInst(tmp);
}

//-----------------------------------------------------------------------------

void lcd_cursor_blink(bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL | D;

	tmp = mode ? (tmp | B) : tmp;

	sendInst(tmp);
}

//-----------------------------------------------------------------------------

void lcd_display(bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL;

	tmp = mode ? (tmp | D) : tmp;

	sendInst(tmp);
}

//-----------------------------------------------------------------------------

void lcd_shift_display_left()
{
	uint8_t tmp = CURSOR_OR_DISPLAY_SHIFT | S_C;

	sendInst(tmp);
}

//-----------------------------------------------------------------------------

void lcd_shift_display_right()
{
	uint8_t tmp = CURSOR_OR_DISPLAY_SHIFT | S_C | R_L;

	sendInst(tmp);
}

//-----------------------------------------------------------------------------

void lcd_create_char(uint8_t data[], uint8_t location)
{
	uint8_t tmp = SET_CGRAM_ADDRES | (location<<3);
	sendInst(tmp);

	uint8_t i;
	for(i = 0; i < 8; i++)
		sendData(data[i]);
}