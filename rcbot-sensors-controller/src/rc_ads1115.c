/*
 * Author: Ruchir Chauhan
 * Adapted for Raspberry pi from Adafruit code
 */

#include <stdlib.h>
#include <byteswap.h>
#include "rc_ads1115.h"

/*=======================================================================*/
/*
 * AREA: I2C-ABSTRACTION
 * This area has functions for abstracting platform differences (if any)
 */
/*=======================================================================*/

static int wiringPi_fd = -1;

/*
static uint8_t i2cread(struct ads1115_module* ads1115) {
    wiringPiI2CRead(wiringPi_fd);
    return 0;
}
*/
/*
static void i2cwrite(uint8_t x) {
    wiringPiI2CWrite(wiringPi_fd, x);
}
*/

/*
 * Writes 16-bits to the specified destination register
 */
static void write_register(uint8_t i2caddress, uint8_t reg, uint16_t value) {
    wiringPiI2CWriteReg16(wiringPi_fd, reg, __bswap_16(value));
}

/*
 * Writes 16-bits to the specified destination register
 */
/**************************************************************************/
static uint16_t read_register(uint8_t i2caddress, uint8_t reg) {
    uint16_t data = 0;
    data = __bswap_16(wiringPiI2CReadReg16(wiringPi_fd, reg));

    return data;
}

/*=======================================================================*/
/*
 * AREA: API-FUNCTIONS
 */
/*=======================================================================*/

struct ads1115_module* ads1115_init(uint8_t i2caddress, ads_gain_t gain) {
    struct ads1115_module* ads1115 = (struct ads1115_module*)malloc(sizeof(struct ads1115_module)); //TODO
    ads1115->i2caddress = i2caddress;
    ads1115->conversion_delay = ADS1115_CONVERSIONDELAY;
    ads1115->bit_shift = 0;
    ads1115->gain = gain;

    wiringPi_fd = wiringPiI2CSetup(ads1115->i2caddress);

    printf("ADS1115 Initialized\n");

    return ads1115;
}

void       ads1115_start(struct ads1115_module* ads1115) {

}

uint16_t   read_adc_single_ended(struct ads1115_module* ads1115, uint8_t channel) {
    if (channel > 3){
        printf("Invalid channel number\n");
        return 0;
    }
    //printf("adc single-ended read\n");
    // Start with default values
    uint16_t config = ADS1115_REG_CONFIG_CQUE_NONE    | // Disable the comparator (default val)
                      ADS1115_REG_CONFIG_CLAT_NONLAT  | // Non-latching (default val)
                      ADS1115_REG_CONFIG_CPOL_ACTVLOW | // Alert/Rdy active low   (default val)
                      ADS1115_REG_CONFIG_CMODE_TRAD   | // Traditional comparator (default val)
                      ADS1115_REG_CONFIG_DR_128SPS    | // 128 samples per second (default)
                      ADS1115_REG_CONFIG_MODE_SINGLE;   // Single-shot mode (default)

    // Set PGA/voltage range
    config |= ads1115->gain;

    // Set single-ended input channel
    switch (channel)
    {
    case (0):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_0;
      break;
    case (1):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_1;
      break;
    case (2):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_2;
      break;
    case (3):
      config |= ADS1115_REG_CONFIG_MUX_SINGLE_3;
      break;
    }

    // Set 'start single-conversion' bit
    config |= ADS1115_REG_CONFIG_OS_SINGLE;

    // Write config register to the ADC
    write_register(ads1115->i2caddress, ADS1115_REG_POINTER_CONFIG, config);

    // Wait for the conversion to complete
    //delay(ads1115->conversion_delay);

    // Read the conversion results
    // Shift 12-bit results right 4 bits for the ADS1015
    return read_register(ads1115->i2caddress, ADS1115_REG_POINTER_CONVERT) >> ads1115->bit_shift;
}

int16_t    read_adc_differential_0_1(struct ads1115_module* ads1115) {
    int16_t adc_val = 0;

    return adc_val;
}

int16_t    read_adc_differential_2_3(struct ads1115_module* ads1115) {
    int16_t adc_val = 0;

    return adc_val;
}

void       start_comparator_single_ended(struct ads1115_module* ads1115, uint8_t channel, int16_t threshold) {

}

int16_t    get_last_conversion_results(struct ads1115_module* ads1115) {
    int16_t adc_val = 0;

    return adc_val;
}

void       setGain(struct ads1115_module* ads1115, ads_gain_t gain) {
    ads1115->gain = gain;
}

ads_gain_t getGain(struct ads1115_module* ads1115) {
    return ads1115->gain;
}


/*=======================================================================*/
/*
 * AREA: SAMPLE-MAIN-FUNCTION
 */
/*=======================================================================*/

/*
int main(void) {

    return 0;
}
*/