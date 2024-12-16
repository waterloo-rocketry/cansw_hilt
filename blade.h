#ifndef BLADE_H
#define BLADE_H

#include <stdint.h>
#include <stdbool.h>

// Pointers to the TRISx, LATx, PORTx and ANSELx registers for a port.
typedef struct {
    volatile uint16_t *tris;
    volatile uint16_t *lat;
    volatile uint16_t *port;
    volatile uint16_t *ansel;
} port_t;

// Pointer to the RPORx register for a given pair of PPS-remappable pins
// Each RPORx stores the output function for two pins.
// Eg RPOR0 has the mappings for RP0 and RP1, RPOR1 has the mappings for RP2 and RP3, etc.
typedef struct {
    volatile uint16_t *rpor;
} pps_out_t;

// Stores the digital RP number, analog pin number and pps output number for a given pin.
// For example, pin RD15/AN29/RP5 has
//   pin_n = 0x3F (port D = 0x3, pin 15 = 0xF within that port)
//   an_n = 29
//   pps_n = 5
typedef struct {
    uint8_t pin_n;
    uint8_t an_n;
    uint8_t pps_n;
} pin_t;

// Stores the 6 pins associated with a single slot.
typedef struct {
    pin_t pps1;
    pin_t pps2;
    pin_t an1;
    pin_t an2;
    pin_t d1;
    pin_t d2;
} slot_t;

#define NUM_SLOTS 10
// Array of slot objects describing the 10 slots
extern const slot_t slots[NUM_SLOTS];

// Set the TRIS value for a given pin
void set_tris(pin_t pin, bool value);
// Set the LAT value for a given pin
void set_lat(pin_t pin, bool value);
// Set the ANSEL value for a given pin
void set_ansel(pin_t pin, bool value);
// Read the value of the LAT register for a given pin
bool get_lat(pin_t pin);
// Read the value of the PORT register for a given pin
bool get_port(pin_t pin);
// Set the output PPS function for a given pin
void set_pps_out(pin_t pin, uint8_t pps_function);

#endif /* BLADE_H */
