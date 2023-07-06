#include "usb.h"
#include <xc.h>

#include "mcc_generated_files/usb/usb.h"
#include "commands.h"

void usb_init(void) {
    USBDeviceInit();
    USBDeviceAttach();
}

uint8_t read_buf[128];

void usb_heartbeat(void) {
    if (USBGetDeviceState() < CONFIGURED_STATE || USBIsDeviceSuspended()) {
        return; // USB is not attached
    }
    
    CDCTxService();
    
    if (!USBUSARTIsTxTrfReady()) {
        return; // USB is not ready to send/receive
    }
    
    uint8_t n = getsUSBUSART(read_buf, sizeof(read_buf));
    if (n > 0) {
//        putUSBUSART(read_buf, n);
    }
    for (uint8_t i = 0; i < n; i++) {
        commands_handle_character(read_buf[i]);
    }
}
