#ifndef _TIMER2_LIB_H_
#define _TIMER2_LIB_H_

#include <avr/io.h>
#include <avr/interrupt.h>

enum tim2_presc_t {
	TIM2_NO_CLK_SOURCE = 0, //Timer/Counter stopped
	TIM2_PRESC_1,
	TIM2_PRESC_8,
    TIM2_PRESC_32,
	TIM2_PRESC_64,
    TIM2_PRESC_128,
	TIM2_PRESC_256,
	TIM2_PRESC_1024,
	TIM2_EXT_T0_FALL,       //External clock source T0 pin. Clock on falling edge
	TIM2_EXT_T0_RISE        //External clock source T0 pin. Clock on rising edge
};

void tim2_set_prescaler(enum tim2_presc_t presc_type);


enum tim2_mode_t {                                          /*TOP*/
	TIM2_NORMAL = 0,        //Normal                        | 0xFF
	TIM2_PWM_PC,            //PWM, phase correct            | 0xFF
	TIM2_CTC,				//CTC 							| OCRA		
	TIM2_FAST_PWM,			//Fast PWM 						| 0xFF
	TIM2_RESERVED_0,		//Reserved(don't use)			| -
	TIM2_PWM_PC_OCRA,		//PWM, phase correct			| OCRA
	TIM2_RESERVED_1,		//Reserved(don't use)			| -
	TIM2_FAST_PWM_OCR,		//Fast PWM						| OCRA
};  

void tim2_waveform_gen_mode(enum tim2_mode_t mode);


//Additional information see in DATASHEET
enum tim2_comp_out_mode_t {
	TIM2_NORM_PORT_OP = 0,  //Normal port operation, OC2x disconnected
	TIM2_TOGGLE_OC2x,
	TIM2_CLEAR_OC2x,
	TIM2_SET_OC2x
};

void tim2_comp_out_mode_set_a(enum tim2_comp_out_mode_t mode);

void tim2_comp_out_mode_set_b(enum tim2_comp_out_mode_t mode);


#define TIM2_OUT_COMP_B     (1<<2)  //Output Compare B Match Interrupt Enable
#define TIM2_OUT_COMP_A     (1<<1)  //Output Compare A Match Interrupt Enable
#define TIM2_OVF            (1<<0)  //Overflow Interrupt Enable

void tim2_int_mask_set(uint8_t mask);

void tim2_compa_attach_handler(void (*foo)(void));
void tim2_compa_set_value(uint8_t value);

#endif /* _TIMER2_LIB_H_ */
