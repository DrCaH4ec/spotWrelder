#ifndef _TWI_H_
#define _TWI_H_


#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>

// #define TWI_PRESC_1     ( 0 << TWPS1 ) | ( 0 << TWPS0 )
// #define TWI_PRESC_4     ( 0 << TWPS1 ) | ( 1 << TWPS0 )
// #define TWI_PRESC_16    ( 1 << TWPS1 ) | ( 0 << TWPS0 )
// #define TWI_PRESC_64    ( 1 << TWPS1 ) | ( 1 << TWPS0 )

#define TWI_ENABLE      true
#define TWI_DISABLE     false

#define TWI_READ        0x01
#define TWI_WRITE       0x00


enum twi_presc_t {
    TWI_PRESC_1 = ( 0 << TWPS1 ) | ( 0 << TWPS0 ),
    TWI_PRESC_4 = ( 0 << TWPS1 ) | ( 1 << TWPS0 ),
    TWI_PRESC_16 = ( 1 << TWPS1 ) | ( 0 << TWPS0 ),
    TWI_PRESC_64 = ( 1 << TWPS1 ) | ( 1 << TWPS0 )
};

/**
 * @brief Uses for init of I2C interface. It enables TWI and
 * set SCL frequency due to formula:
 * 
 * SCL_freq = F_CPU / (16 + 2 * twi_bitrate * twi_prescaler)
 * 
 * @param twi_prescaler = value of the prescaler
 * Possible options:
 * - TWI_PRESC_1    -> no prescaler
 * - TWI_PRESC_4    -> prescaler = 4
 * - TWI_PRESC_16   -> prescaler = 16
 * - TWI_PRESC_64   -> prescaler = 64
 * 
 * @param twi_bitrate = value of the TWI Bit Rate register [0:255]
 */
void twi_init(enum twi_presc_t twi_prescaler, uint8_t twi_bitrate);


/**
 * @brief set one of prescalers for TWI
 * Possible options:
 * - TWI_PRESC_1    -> no prescaler
 * - TWI_PRESC_4    -> 
 * - TWI_PRESC_16   ->
 * - TWI_PRESC_64   ->
 * 
 * @param twi_prescaler 
 */
// void twi_set_prescaler(enum twi_presc_t twi_prescaler);

// void twi_set_bitrate(uint8_t twi_bitrate);

void twi_state(bool twi_status);

void twi_start_cond(void);

void twi_stop_cond(void);

void twi_send_dev_addr(uint8_t dev_addr, uint8_t twi_r_w);

uint8_t twi_read_byte(void);

uint8_t twi_read_last_byte(void);

void twi_write_byte(uint8_t data);

void twi_read_data(uint8_t dev_addr, uint8_t start_addr, uint8_t *data, uint16_t num_of_bytes);

void twi_write_data(uint8_t dev_addr, uint8_t start_addr, const uint8_t *data, uint16_t num_of_bytes);

/*_TWI_H_*/
#endif
