#include "usb.h"
#include <xc.h>

#include "mcc_generated_files/usb/usb.h"
#include "commands.h"
#include "canlib/can_common.h"
#include "canlib/util/can_rcv_buffer.h"

void usb_init(void) {
    USBDeviceInit();
    USBDeviceAttach();
}

uint8_t read_buf[128];
uint8_t write_buf[4 + 3*8 + 2];

const static char hex_lookup_table[16] = {
    '0', '1', '2', '3',
    '4', '5', '6', '7',
    '8', '9', 'A', 'B',
    'C', 'D', 'E', 'F'
};

void usb_heartbeat(void) {
    if (USBGetDeviceState() < CONFIGURED_STATE || USBIsDeviceSuspended()) {
        return; // USB is not attached
    }

    CDCTxService();

    if (!USBUSARTIsTxTrfReady()) {
        return; // USB is not ready to send/receive
    }

    uint8_t n = getsUSBUSART(read_buf, sizeof(read_buf));

    for (uint8_t i = 0; i < n; i++) {
        commands_handle_character(read_buf[i]);
    }

    // The CAN handler puts messages into the rcvb. Pop them off and send them to the host.
    can_msg_t msg;
    while (rcvb_pop_message(&msg)) {
        write_buf[0] = 'M';
        write_buf[1] = hex_lookup_table[(msg.sid >> 8) & 0xf];
        write_buf[2] = hex_lookup_table[(msg.sid >> 4) & 0xf];
        write_buf[3] = hex_lookup_table[msg.sid & 0xf];
        write_buf[4] = ',';
        uint8_t i;
        for (i = 0; i < msg.data_len && i < 8; ++i) {
            write_buf[3 * i + 5] = hex_lookup_table[msg.data[i] >> 4];
            write_buf[3 * i + 6] = hex_lookup_table[msg.data[i] & 0xf];
            write_buf[3 * i + 7] = ',';
        }
        write_buf[3 * i + 4] = ';';
        write_buf[3 * i + 5] = '\n';

        putUSBUSART(write_buf, 3 * i + 6);
        CDCTxService();
    }
}
