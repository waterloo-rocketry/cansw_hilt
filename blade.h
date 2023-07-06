#ifndef BLADE_H
#define	BLADE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    volatile uint16_t *tris;
    volatile uint16_t *lat;
    volatile uint16_t *port;
    volatile uint16_t *ansel;
} port_t;

typedef struct {
    volatile uint16_t *rpor;
} pps_out_t;

typedef struct {
    uint8_t pin_n;
    uint8_t an_n;
    uint8_t pps_n;
} pin_t;

typedef struct {
    pin_t pps1;
    pin_t pps2;
    pin_t an1;
    pin_t an2;
    pin_t d1;
    pin_t d2;
} slot_t;

extern const slot_t slots[10];

void set_tris(uint8_t pin_n, bool value);
void set_lat(uint8_t pin_n, bool value);
void set_ansel(uint8_t pin_n, bool value);
bool get_lat(uint8_t pin_n);
bool get_port(uint8_t pin_n);
void set_pps_out(uint8_t pps_n, uint8_t id);

#endif	/* BLADE_H */
