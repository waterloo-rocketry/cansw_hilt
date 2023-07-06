#ifndef PWM_H
#define	PWM_H

#include <stdint.h>

typedef struct oc_pwm {
    volatile uint16_t *r;
    volatile uint16_t *rs;
    volatile uint16_t *con1;
    volatile uint16_t *con2;
    uint8_t pps_id;
} oc_pwm_t;

void pwm_init(void);
void pwm_set(uint8_t channel, uint8_t value);
void pwm_assign(uint8_t channel, uint8_t pps_n);

#endif
