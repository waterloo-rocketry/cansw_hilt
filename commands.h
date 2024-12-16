#ifndef COMMANDS_H
#define COMMANDS_H

#include <stdint.h>

// Describe a command with a name, a maximum data length, and a handler function
typedef struct {
    char name;
    uint8_t max_len;
    void (*command_fcn)(uint8_t *data, uint8_t data_len);
} command_t;

void commands_handle_character(char c);

#endif
