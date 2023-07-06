#include "adc.h"
#include <xc.h>

void adc_init() {
    ADCON1bits.FORM = 0b0000; // Integer, raw data
    ADCON2bits.BUFORG = 1; // Result buffers map to sample list entries
    ADCON3bits.SLEN0 = 1; // Enable sample list 0
    ADCON3bits.ADRC = 1; // Use 8MHz FRC as clock
    ADCON3bits.ADCS = 4; // 8x prescaler, should give 1 MHz
    ADCON1bits.ADON = 1; // Turn on ADC
    while (!ADSTATHbits.ADREADY); // Wait for startup
    ADL0CONHbits.SAMC = 1; // Sample time of 1 T_AD
    ADL0CONHbits.SLINT = 1; // Interrupt after every sample
}

uint16_t adc_sample(uint8_t channel) {
    if (channel == 0xFF) return -1; // Invalid
    ADTBL0bits.ADCH = channel; // Set channel
    ADL0CONLbits.SLEN = 1; // Enable sample list
    ADL0CONLbits.SAMP = 1; // Prepare to trigger
    ADSTATLbits.SL0IF = 0; // Clear interrupt flag
    ADL0CONLbits.SAMP = 0; // Trigger
    while (!ADSTATLbits.SL0IF); // Wait for completion
    uint16_t res = ADRES0;
    ADSTATLbits.SL0IF = 0; // Clear interrupt flag
    ADL0CONLbits.SAMP = 1; // Prepare to trigger
    ADL0CONLbits.SLEN = 0; // Disable sample list
    return res; // Read result
}
