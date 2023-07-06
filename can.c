#include "can.h"
#include <xc.h>

#include "canlib/can_common.h"
#include "canlib/mcp2515/mcp_2515.h"
#include "canlib/util/can_tx_buffer.h"
#include "canlib/util/can_rcv_buffer.h"
#include "canlib/util/timing_util.h"
#include "spi.h"

uint8_t tx_pool[500];
uint8_t rx_pool[500];

void can_init(void) {
    can_timing_t can_setup;
    can_generate_timing_params(12000000, &can_setup);
    mcp_can_init(&can_setup, spi_read, spi_write, cs_drive);
    txb_init(tx_pool, sizeof(tx_pool), mcp_can_send, mcp_can_send_rdy);
    rcvb_init(rx_pool, sizeof(rx_pool));
    
    TRISEbits.TRISE8 = 1; // INT pin
}

void can_heartbeat(void) {
    txb_heartbeat();
    if (!PORTEbits.RE8) { // Interrupt signal from MCP2515
        can_msg_t rcv;
        if (mcp_can_receive(&rcv)) {
            rcvb_push_message(&rcv);
        }
    }
}
