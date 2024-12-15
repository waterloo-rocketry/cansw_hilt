#ifndef ADC_H
#define ADC_H

#include <stdint.h>
#include "blade.h"

void adc_init(void);

// Read the ADC value on the given pin
uint16_t adc_sample(pin_t pin);


#endif
