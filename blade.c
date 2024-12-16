#include "blade.h"
#include <xc.h>

const port_t ports[7] = {
    {&TRISA, &LATA, &PORTA},
    {&TRISB, &LATB, &PORTB},
    {&TRISC, &LATC, &PORTC},
    {&TRISD, &LATD, &PORTD},
    {&TRISE, &LATE, &PORTE},
    {&TRISF, &LATF, &PORTF},
    {&TRISG, &LATG, &PORTG}
};

const pps_out_t pps_outputs[16] = {
    {&RPOR0},
    {&RPOR1},
    {&RPOR2},
    {&RPOR3},
    {&RPOR4},
    {&RPOR5},
    {&RPOR6},
    {&RPOR7},
    {&RPOR8},
    {&RPOR9},
    {&RPOR10},
    {&RPOR11},
    {&RPOR12},
    {&RPOR13},
    {&RPOR14},
    {&RPOR15}
};

const slot_t slots[10] = {
    {
        .pps1 = {0x66, 17, 21},  // AO1 - RG6, AN16, RP21
        .pps2 = {0x67, 18, 26},  // AO2 - RG7, AN18, RP26
        .an1 =  {0x21, 8, -1},   // AI1 - RC1, AN8
        .an2 =  {0x23, 9, -1},   // AI2 - RC3, AN9
        .d1 =   {0x5C, -1, -1},  // D5 - RF12
        .d2 =   {0x2E, -1, -1}   // D6 - RC14
    },
    {
        .pps1 = {0x68, 19, 19},
        .pps2 = {0x69, 49, 27},
        .an1 =  {0x24, 16, -1},
        .an2 =  {0x49, 21, -1},
        .d1 =   {0x45, -1, -1},
        .d2 =   {0x46, -1, -1}
    },
    {
        .pps1 = {0x15, 5, 18},
        .pps2 = {0x14, 4, 28},
        .an1 =  {0x3E, 28, -1},
        .an2 =  {0x0E, 38, -1},
        .d1 =   {0x47, -1, -1},
        .d2 =   {0x00, -1, -1}
    },
    {
        .pps1 = {0x12, 2, 13},
        .pps2 = {0x16, 6, 6},
        .an1 =  {0x0F, 39, -1},
        .an2 =  {0x3C, 45, -1},
        .d1 =   {0x09, -1, -1},
        .d2 =   {0x0A, -1, -1}
    },
    {
        .pps1 = {0x17, 7, 7},
        .pps2 = {0x5D, 27, 31},
        .an1 =  {0x6F, 33, -1},
        .an2 =  {0x13, 3, -1},
        .d1 =   {0x2D, -1, -1},
        .d2 =   {0x50, -1, -1}
    },
    {
        .pps1 = {0x1E, 14, 14},
        .pps2 = {0x1F, 15, 29},
        .an1 =  {0x01, 26, -1},
        .an2 =  {0x1C, 12, -1},
        .d1 =   {0x51, -1, -1},
        .d2 =   {0x61, -1, -1}
    },
    {
        .pps1 = {0x3F, 29, 5},
        .pps2 = {0x54, 11, 10},
        .an1 =  {0x1D, 13, -1},
        .an2 =  {0x04, 36, -1},
        .d1 =   {0x60, -1, -1},
        .d2 =   {0x40, -1, -1}
    },
    {
        .pps1 = {0x55, 10, 17},
        .pps2 = {0x53, 30, 16},
        .an1 =  {0x05, 37, -1},
        .an2 =  {0x3D, 46, -1},
        .d1 =   {0x41, -1, -1},
        .d2 =   {0x6E, -1, -1}
    },
    {
        .pps1 = {0x52, 31, 30},
        .pps2 = {0x58, 32, 15},
        .an1 =  {0x36, 34, -1},
        .an2 =  {0x37, 20, -1},
        .d1 =   {0x6C, -1, -1},
        .d2 =   {0x6D, -1, -1}
    },
    {
        .pps1 = {0x38, 40, 2},
        .pps2 = {0x39, 24, 4},
        .an1 =  {0x06, 23, -1},
        .an2 =  {0x07, 22, -1},
        .d1 =   {0x42, -1, -1},
        .d2 =   {0x43, -1, -1}
    }
};

void set_tris(pin_t pin, bool value) {
    port_t port = ports[pin.pin_n >> 4];
    uint16_t mask = 1 << (pin.pin_n & 0xF);
    if (value) {
        *port.tris |= mask;
    } else {
        *port.tris &= ~mask;
    }
}

void set_lat(pin_t pin, bool value) {
    port_t port = ports[pin.pin_n >> 4];
    uint16_t mask = 1 << (pin.pin_n & 0xF);
    if (value) {
        *port.lat |= mask;
    } else {
        *port.lat &= ~mask;
    }
}

void set_ansel(pin_t pin, bool value) {
    port_t port = ports[pin.pin_n >> 4];
    uint16_t mask = 1 << (pin.pin_n & 0xF);
    if (value) {
        *port.ansel |= mask;
    } else {
        *port.ansel &= ~mask;
    }
}

bool get_lat(pin_t pin) {
    port_t port = ports[pin.pin_n >> 4];
    uint16_t mask = 1 << (pin.pin_n & 0xF);
    return *port.lat & mask;
}

bool get_port(pin_t pin) {
    port_t port = ports[pin.pin_n >> 4];
    uint16_t mask = 1 << (pin.pin_n & 0xF);
    return *port.port & mask;
}

void set_pps_out(pin_t pin, uint8_t pps_function) {
    if (pin.pps_n == 0xFF) return; // Invalid, "-1" in config above
    // Each pps output handles 2 pins
    pps_out_t pps = pps_outputs[pin.pps_n / 2];
    uint16_t mask = 0xFF         << ((~pin.pps_n & 1) * 8);
    uint16_t set  = pps_function << (( pin.pps_n & 1) * 8);
    *pps.rpor = (*pps.rpor & mask) | set;
}
