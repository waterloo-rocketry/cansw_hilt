#include <xc.h>
#include "blade.h"
#include "setup.h"
#include "mcc_generated_files/usb/usb.h"

int main(void) {
    setup();
    
    set_lat(slots[9].d1.pin_n, true);
    set_lat(slots[9].d2.pin_n, false);
    set_tris(slots[9].pps1.pin_n, false);
    set_tris(slots[9].pps2.pin_n, false);
    set_tris(slots[9].an1.pin_n, true);
    set_tris(slots[9].an2.pin_n, true);
    set_tris(slots[9].d1.pin_n, false);
    set_tris(slots[9].d2.pin_n, false);
    set_pps_out(slots[9].pps1.pps_n, 18);
    set_pps_out(slots[9].pps2.pps_n, 18);
    set_ansel(slots[9].an1.pin_n, true);
    set_ansel(slots[9].an2.pin_n, true);
    
    set_lat(slots[8].d1.pin_n, true);
    set_lat(slots[8].d2.pin_n, false);
    set_tris(slots[8].pps1.pin_n, true);
    set_tris(slots[8].pps2.pin_n, true);
    set_tris(slots[8].an1.pin_n, true);
    set_tris(slots[8].an2.pin_n, true);
    set_tris(slots[8].d1.pin_n, false);
    set_tris(slots[8].d2.pin_n, false);
    set_ansel(slots[8].pps1.pin_n, true);
    set_ansel(slots[8].pps2.pin_n, true);
    set_ansel(slots[8].an1.pin_n, true);
    set_ansel(slots[8].an2.pin_n, true);
    
    // ADC setup
    ADCON1bits.FORM = 0b0000; // Integer, raw data
    ADCON2bits.BUFORG = 1;
    ADCON3bits.SLEN0 = 1;
    ADCON3bits.ADRC = 1; // Use FRC as clock
    ADCON3bits.ADCS = 2; // 4x prescaler, should give 2 MHz
    ADCON1bits.ADON = 1;
    while (!ADSTATHbits.ADREADY);
    ADTBL0bits.ADCH = slots[9].pps1.an_n;
    ADL0CONHbits.SAMC = 1;
    ADL0CONHbits.SLINT = 1;
    ADL0CONLbits.SAMP = 1;
    ADL0CONLbits.SLEN = 1;
    
    // PLL setup
    OC1R = 150; // roughly 25% duty cycle
    OC1RS = 200; // roughly 100 Hz
    OC1CON1bits.OCTSEL = 0b111; // Peripherial clock, 4 MHz
    OC1CON2bits.SYNCSEL = 0x1F; // Select OC1 as sync source???
    OC1CON2bits.OCTRIG = 0;     // Select OC1 as sync source???
    OC1CON1bits.OCM = 0b110; // Edge-aligned PWM
    
    uint16_t res;
    while (1) {
//        ADSTATLbits.SL0IF = 0;
//        ADL0CONLbits.SAMP = 0;
//        while (!ADSTATLbits.SL0IF);
//        res = ADRES0;
//        ADL0CONLbits.SAMP = 1;
//        ADSTATLbits.SL0IF = 0;
//        
//        OC1R += 1;
//        if (OC1R > 200) {
//            OC1R = 0;
//        }
    }

    return 0;
}
