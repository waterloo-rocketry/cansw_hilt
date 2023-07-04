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
    {},
    {},
    {},
    {},
    {},
    {},
    {},
    {},
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

void set_tris(uint8_t pin_n, bool value) {
    port_t port = ports[pin_n >> 4];
    uint16_t mask = 1 << (pin_n & 0xF);
    if (value) {
        *port.tris |= mask;
    } else {
        *port.tris &= ~mask;
    }
}

void set_lat(uint8_t pin_n, bool value) {
    port_t port = ports[pin_n >> 4];
    uint16_t mask = 1 << (pin_n & 0xF);
    if (value) {
        *port.lat |= mask;
    } else {
        *port.lat &= ~mask;
    }
}

void set_ansel(uint8_t pin_n, bool value) {
    port_t port = ports[pin_n >> 4];
    uint16_t mask = 1 << (pin_n & 0xF);
    if (value) {
        *port.ansel |= mask;
    } else {
        *port.ansel &= ~mask;
    }
}

bool get_lat(uint8_t pin_n) {
    port_t port = ports[pin_n >> 4];
    uint16_t mask = 1 << (pin_n & 0xF);
    return *port.lat & mask;
}

bool get_port(uint8_t pin_n) {
    port_t port = ports[pin_n >> 4];
    uint16_t mask = 1 << (pin_n & 0xF);
    return *port.port & mask;
}

void set_pps_out(uint8_t pps_n, uint8_t id) {
    pps_out_t pps = pps_outputs[pps_n >> 1];
    uint16_t mask = 0xFF << ((~pps_n & 1) * 8);
    uint16_t set  = id   << (( pps_n & 1) * 8);
    *pps.rpor = (*pps.rpor & mask) | set;
}
