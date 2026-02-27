#include "lcdTwi_lib.h"

#define LOW 0
#define HIGH 1

//-----------------------------------------------------------------------------
#define ON 		1
#define OFF 	0
//-----------------------------------------------------------------------------
#define CLEAR_DISPLAY 				(1<<0)
#define RETURN_HOME 				(1<<1)
#define ENTRY_MODE_SET				(1<<2)
#define DISPLAY_ON_OFF_CONTROL		(1<<3)
#define CURSOR_OR_DISPLAY_SHIFT		(1<<4)
#define FUNCTION_SET				(1<<5)
#define SET_CGRAM_ADDRES			(1<<6)
#define SET_DDRAM_ADDRES			(1<<7)

// Entry mode set
#define SH 							(1<<0)
#define I_D							(1<<1)

// Display ON/OFF control
#define B 							(1<<0)
#define C 							(1<<1)
#define D 							(1<<2)

// Cursor or Display Shift
#define R_L							(1<<2)
#define S_C 						(1<<3)

// Function set
#define F 							(1<<2)
#define N							(1<<3)
#define DL 							(1<<4)

#define LCD_RS 						(1<<0)
#define LCD_RW						(1<<1)
#define LCD_E  						(1<<2)
#define LCD_LED						(1<<3)
#define LCD_D4 						(1<<4)
#define LCD_D5						(1<<5)
#define LCD_D6						(1<<6)
#define LCD_D7						(1<<7)

#define LCD_DELAY_US(x) do { \
		_delay_us(x); \
	} while(0)

#define LCD_DELAY_MS(x) do { \
		_delay_ms(x); \
	} while(0)

//#############################################################################
//#####################---INTERNAL FUNCTIONS---################################
//#############################################################################

static void pulseEn(twiLcdCtxt_t *me)
{
	me->lcd_reg &= ~LCD_E;
    twi_write_data(me->addr, me->lcd_reg, 0x00, 0);
	LCD_DELAY_US(10);
	me->lcd_reg |= LCD_E;
  	twi_write_data(me->addr, me->lcd_reg, 0x00, 0);
	LCD_DELAY_US(10);		
	me->lcd_reg &= ~LCD_E;
    twi_write_data(me->addr, me->lcd_reg, 0x00, 0);
	LCD_DELAY_US(100);
}

//-----------------------------------------------------------------------------

// part=1 -> high
// part=0 -> low
static void sendHalfByte(twiLcdCtxt_t *me, uint8_t data, bool part)
{

	me->lcd_reg &= ~(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7);

	if(part){
		me->lcd_reg = data&(1<<4) ? me->lcd_reg|LCD_D4 : me->lcd_reg;
		me->lcd_reg = data&(1<<5) ? me->lcd_reg|LCD_D5 : me->lcd_reg;
		me->lcd_reg = data&(1<<6) ? me->lcd_reg|LCD_D6 : me->lcd_reg;
		me->lcd_reg = data&(1<<7) ? me->lcd_reg|LCD_D7 : me->lcd_reg;
	}
	else{
		me->lcd_reg = data&(1<<0) ? me->lcd_reg|LCD_D4 : me->lcd_reg;
		me->lcd_reg = data&(1<<1) ? me->lcd_reg|LCD_D5 : me->lcd_reg;
		me->lcd_reg = data&(1<<2) ? me->lcd_reg|LCD_D6 : me->lcd_reg;
		me->lcd_reg = data&(1<<3) ? me->lcd_reg|LCD_D7 : me->lcd_reg;
	}

	pulseEn(me);
}

//-----------------------------------------------------------------------------

static void sendInst(twiLcdCtxt_t *me, uint8_t data)
{
	me->lcd_reg &= ~LCD_RS;

	sendHalfByte(me, data, HIGH);
	sendHalfByte(me, data, LOW);
}

//-----------------------------------------------------------------------------

static void sendData(twiLcdCtxt_t *me, uint8_t data)
{
	me->lcd_reg |= LCD_RS;

	LCD_DELAY_US(1);

	sendHalfByte(me, data, HIGH);
	sendHalfByte(me, data, LOW);

	LCD_DELAY_US(50);
}

//#############################################################################
//#####################---EXTERNAL FUNCTIONS---################################
//#############################################################################

void lcd_backlight(twiLcdCtxt_t *me, bool mode)
{
	me->lcd_reg = mode ? me->lcd_reg|LCD_LED : me->lcd_reg&(~LCD_LED);

	twi_write_data(me->addr, me->lcd_reg, 0x00, 0);
}

//-----------------------------------------------------------------------------

void lcd_clear(twiLcdCtxt_t *me)
{
	sendInst(me, CLEAR_DISPLAY);
	LCD_DELAY_MS(2);
}

//-----------------------------------------------------------------------------

void lcd_home(twiLcdCtxt_t *me)
{
	sendInst(me, RETURN_HOME);
	LCD_DELAY_MS(2);
}

//-----------------------------------------------------------------------------

void lcd_init(twiLcdCtxt_t *me)
{
	uint8_t tmp = 0x00;

	LCD_DELAY_MS(50);

	tmp = FUNCTION_SET | DL;
	sendHalfByte(me, tmp, HIGH);
	LCD_DELAY_MS(5);
	sendHalfByte(me, tmp, HIGH);
	LCD_DELAY_US(150);
	sendHalfByte(me, tmp, HIGH);
	LCD_DELAY_US(50);

	tmp = 0x00;
	tmp |= FUNCTION_SET;
	sendHalfByte(me, tmp, HIGH);

	//2 lines mode set
	//5x8 dots
	tmp = me->lines > 1 ? (tmp | N) : tmp;
	sendInst(me, tmp);
	sendInst(me, tmp);

	tmp = 0x00;
	tmp = DISPLAY_ON_OFF_CONTROL | D;
	sendInst(me, tmp);

	tmp = 0x00;
	tmp = ENTRY_MODE_SET | I_D;
	sendInst(me, tmp);

	lcd_clear(me);	
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

	sendInst(me, tmp);
	LCD_DELAY_US(100);
}

//-----------------------------------------------------------------------------

void lcd_puts(twiLcdCtxt_t *me, char data[])
{
	uint8_t i = 0;

	while(data[i] != '\0' && i < 16){
		sendData(me, data[i]);
		i++;
	}
}

//-----------------------------------------------------------------------------

void lcd_putChar(twiLcdCtxt_t *me, char data)
{
	sendData(me, data);
}

//-----------------------------------------------------------------------------

void lcd_cursor(twiLcdCtxt_t *me, bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL | D;

	tmp = mode ? (tmp | C) : tmp;

	sendInst(me, tmp);
}

//-----------------------------------------------------------------------------

void lcd_cursorBlink(twiLcdCtxt_t *me, bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL | D;

	tmp = mode ? (tmp | B) : tmp;

	sendInst(me, tmp);
}

//-----------------------------------------------------------------------------

void lcd_display(twiLcdCtxt_t *me, bool mode)
{
	uint8_t tmp = DISPLAY_ON_OFF_CONTROL;

	tmp = mode ? (tmp | D) : tmp;

	sendInst(me, tmp);
}

//-----------------------------------------------------------------------------

void lcd_shiftDispLeft(twiLcdCtxt_t *me)
{
	uint8_t tmp = CURSOR_OR_DISPLAY_SHIFT | S_C;

	sendInst(me, tmp);
}

//-----------------------------------------------------------------------------

void lcd_shiftDispRight(twiLcdCtxt_t *me)
{
	sendInst(me, (CURSOR_OR_DISPLAY_SHIFT | S_C | R_L));
}

//-----------------------------------------------------------------------------

void lcd_createChar(twiLcdCtxt_t *me, uint8_t data[], uint8_t location)
{
	uint8_t tmp = SET_CGRAM_ADDRES | (location<<3);
	sendInst(me, tmp);

	uint8_t i;
	for(i = 0; i < 8; i++)
		sendData(me, data[i]);
}
