#include "pwm.h"
#include <xc.h>

#include "blade.h"

const oc_pwm_t pwm_modules[9] = {
    {&OC1R, &OC1RS, &OC1CON1, &OC1CON2, 18},
    {&OC2R, &OC2RS, &OC2CON1, &OC2CON2, 19},
    {&OC3R, &OC3RS, &OC3CON1, &OC3CON2, 20},
    {&OC4R, &OC4RS, &OC4CON1, &OC4CON2, 21},
    {&OC5R, &OC5RS, &OC4CON1, &OC5CON2, 22},
    {&OC6R, &OC6RS, &OC5CON1, &OC6CON2, 23},
    {&OC7R, &OC7RS, &OC7CON1, &OC7CON2, 24},
    {&OC8R, &OC8RS, &OC8CON1, &OC8CON2, 25},
    {&OC9R, &OC9RS, &OC9CON1, &OC9CON2, 35}
};

void pwm_init() {
    for (uint8_t i = 0; i < 9; i++) {
        oc_pwm_t module = pwm_modules[i];
        *module.r = 0; // duty cycle
        *module.rs = 100; // period
        *module.con1 = 0x1C00; // Use peripherical clock (16 MHz)
        *module.con2 = 0x001F; // Set ourselves as sync source
        *module.con1 |= 0b111; // Center-aligned PWM
    }
//    
//    OC1R = 0; // duty cycle
//    OC1RS = 100; // period
//    OC1CON1bits.OCTSEL = 0b111; // Peripherial clock, 16 MHz
//    OC1CON2bits.SYNCSEL = 0x1F; // Select ourselves as sync source
//    OC1CON2bits.OCTRIG = 0; // Select ourselves as sync source
//    OC1CON1bits.OCM = 0b110; // Enable edge-aligned PWM
}

void pwm_set(uint8_t channel, uint8_t value) {
    oc_pwm_t module = pwm_modules[channel];
    *module.r = value;
}

void pwm_assign(uint8_t channel, uint8_t pps_n) {
    oc_pwm_t module = pwm_modules[channel];
    set_pps_out(pps_n, module.pps_id);
}