#ifndef PWM_H
#define PWM_H

#include <stdint.h>
#include "blade.h"

// Stores the relevant registers for a single output compare/pwm channel.
typedef struct oc_pwm {
    volatile uint16_t *r;
    volatile uint16_t *rs;
    volatile uint16_t *con1;
    volatile uint16_t *con2;
    uint8_t pps_id;
} oc_pwm_t;

void pwm_init(void);

// Set the duty cycle of the given channel (value between 0 and 100)
void pwm_set(uint8_t channel, uint8_t value);

// Assign the given PWM channel to output on the given pin.
void pwm_assign(uint8_t channel, pin_t pin);

#endif
