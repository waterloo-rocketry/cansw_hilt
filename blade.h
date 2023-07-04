#ifndef BLADE_H
#define	BLADE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct port {
    uint16_t *tris;
    uint16_t *lat;
    uint16_t *port;
    uint16_t *ansel;
} port_t;

typedef struct pps_out {
    uint16_t *rpor;
} pps_out_t;

typedef struct pin {
    uint8_t pin_n;
    uint8_t an_n;
    uint8_t pps_n;
} pin_t;

struct slot {
    pin_t pps1;
    pin_t pps2;
    pin_t an1;
    pin_t an2;
    pin_t d1;
    pin_t d2;
};
typedef struct slot slot_t;

extern const slot_t slots[10];

void set_tris(uint8_t pin_n, bool value);
void set_lat(uint8_t pin_n, bool value);
void set_ansel(uint8_t pin_n, bool value);
bool get_lat(uint8_t pin_n);
bool get_port(uint8_t pin_n);
void set_pps_out(uint8_t pps_n, uint8_t id);

#endif	/* BLADE_H */
