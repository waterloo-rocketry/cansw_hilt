#include "commands.h"
#include "blade.h"
#include "adc.h"
#include "pwm.h"
#include "mcc_generated_files/usb/usb.h"
#include "canlib/can_common.h"
#include "canlib/util/can_tx_buffer.h"

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

    set_pps_out(pin, 0);
    set_ansel(pin, true);
    set_tris(pin, true);
    uint16_t value = adc_sample(pin);
    static char res[] = "N???????;";
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
    pwm_assign(channel, pin);
}

void cmd_digital_read(uint8_t *data, uint8_t data_len) {
    if (data_len != 2) return;
    pin_t pin;
    if (!parse_pin(data, &pin)) return;

    set_pps_out(pin, 0);
    set_ansel(pin, false);
    set_tris(pin, true);
    bool value = get_port(pin);
    static char res[] = "G???;";
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
    set_pps_out(pin, 0);
    set_lat(pin, value);
    set_tris(pin, false);
}

uint8_t hex2num(char ch) {
    if (ch >= '0' && ch <= '9')
        return ch - '0';
    if (ch >= 'A' && ch <= 'F')
        return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'f')
        return ch - 'a' + 10;
    return 255;
}

void cmd_send_can(uint8_t *data, uint8_t data_len) {
    if (data_len < 3) return;
    if (data_len % 3 != 0) return;
    for (uint8_t i = 0; i < data_len; i++) {
        if (i == 0 || i % 3 != 0) {
            if (hex2num(data[i]) == 255) return;
        } else {
            if (data[i] != ',') return;
        }
    }

    can_msg_t msg = {0};
    msg.sid  = ((uint16_t)hex2num(data[0])) << 8;
    msg.sid |= hex2num(data[1]) << 4;
    msg.sid |= hex2num(data[2]);

    for (uint8_t i = 4; i < data_len; i += 3) {
        msg.data[msg.data_len]  = hex2num(data[i]) << 4;
        msg.data[msg.data_len] |= hex2num(data[i + 1]);
        msg.data_len += 1;
    }

    txb_enqueue(&msg);
}

// Commands are made up of a single character to identify the command, some amount of data, and a
// terminating semicolon. Responses (where applicable) take a similar format, and by convention
// use an uppercase letter.

// The data length specified is a maximum, not an exact value. This is useful for things like
// sending CAN messages of variable length. It is up to the handler to verify an appropriate amount
// of data is present.

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
        if (data_buf_len > command.max_len || data_buf_len >= sizeof(data_buf)) {
            parse_state = WAITING;
            break;
        }
        data_buf[data_buf_len] = c;
        data_buf_len++;
        break;
    }
}
