#include "commands.h"
#include "blade.h"
#include "adc.h"
#include "pwm.h"
#include "mcc_generated_files/usb/usb.h"

// Parse a two-digit pin of the form
// 0d - blade 1, digital pin, channel a
// 0e - blade 1, digital pin, channel b
// 5p - blade 6, pps pin, channel a
// 9b - blade 10, analog pin, channel b
bool parse_pin(uint8_t *data, pin_t *dest) {
    if (data[0] < '0' || data[0] > '9') return false;
    slot_t slot = slots[data[0] - '0'];
    switch (data[1]) {
        case 'a':
            *dest = slot.an1;
            return true;
        case 'b':
            *dest = slot.an2;
            return true;
        case 'd':
            *dest = slot.d1;
            return true;
        case 'e':
            *dest = slot.d2;
            return true;
        case 'p':
            *dest = slot.pps1;
            return true;
        case 'q':
            *dest = slot.pps2;
            return true;
    }
    return false;
}

void cmd_analog_read(uint8_t *data, uint8_t data_len) {
    if (data_len != 2) return;
    pin_t pin;
    if (!parse_pin(data, &pin)) return;
    
    set_pps_out(pin.pps_n, 0);
    set_ansel(pin.pin_n, true);
    set_tris(pin.pin_n, true);
    uint16_t value = adc_sample(pin.an_n);
    static char res[] = "A???????;";
    res[1] = data[0];
    res[2] = data[1];
    for (uint8_t i = 0; i < 5; i++) {
        uint16_t value_div = value / 10;
        uint8_t digit = value - value_div * 10;
        res[7 - i] = '0' + digit;
        value = value_div;
    }
    putsUSBUSART(res);
}

void cmd_analog_write(uint8_t *data, uint8_t data_len) {
    if (data_len != 5) return;
    if (data[2] < '0' || data[2] > '8') return; // PWM channel
    if (data[3] < '0' || data[3] > '9') return; // Value MSB
    if (data[4] < '0' || data[4] > '9') return; // Value LSB
    pin_t pin;
    if (!parse_pin(data, &pin)) return;
    
    uint8_t channel = data[2] - '0';
    uint8_t value = (data[3] - '0') * 10 + (data[4] - '0');
    pwm_set(channel, value);
    pwm_assign(channel, pin.pps_n);
}

void cmd_digital_read(uint8_t *data, uint8_t data_len) {
    if (data_len != 2) return;
    pin_t pin;
    if (!parse_pin(data, &pin)) return;
    
    set_pps_out(pin.pps_n, 0);
    set_ansel(pin.pin_n, false);
    set_tris(pin.pin_n, true);
    bool value = get_port(pin.pin_n);
    static char res[] = "D???;";
    res[1] = data[0];
    res[2] = data[1];
    res[3] = value ? '1' : '0';
    putsUSBUSART(res);
}

void cmd_digital_write(uint8_t *data, uint8_t data_len) {
    if (data_len != 3) return;
    if (data[2] < '0' || data[2] > '1') return;
    pin_t pin;
    if (!parse_pin(data, &pin)) return;
    
    bool value = data[2] == '1';
    set_pps_out(pin.pps_n, 0);
    set_lat(pin.pin_n, value);
    set_tris(pin.pin_n, false);
}

void cmd_send_can(uint8_t *data, uint8_t data_len) {
    
}

static command_t commands[5] = {
    {'a', 2, cmd_analog_read},
    {'b', 5, cmd_analog_write},
    {'d', 2, cmd_digital_read},
    {'e', 3, cmd_digital_write},
    {'m', 27, cmd_send_can},
};

typedef enum {
    WAITING,
    DATA,
} parse_state_t;

static parse_state_t parse_state = WAITING;
static command_t command;
static uint8_t data_buf[128];
static uint8_t data_buf_len = 0;

void commands_handle_character(char c) {
    switch (parse_state) {
    case WAITING:
        for (uint8_t i = 0; i < sizeof(commands) / sizeof(command_t); i++) {
            if (commands[i].name == c) {
                parse_state = DATA;
                command = commands[i];
                data_buf_len = 0;
                break;
            }
        }
        break;
    case DATA:
        if (c == ';') {
            command.command_fcn(data_buf, data_buf_len);
            parse_state = WAITING;
            break;
        }
        if (data_buf_len > command.max_len) {
            parse_state = WAITING;
            break;
        }
        data_buf[data_buf_len] = c;
        data_buf_len++;
        break;
    }
}
