#ifndef COMMANDS_H
#define	COMMANDS_H

#include <stdint.h>

typedef struct {
    char name;
    uint8_t max_len;
    void (*command_fcn)(uint8_t *data, uint8_t data_len);
} command_t;

void commands_handle_character(char c);

#endif
