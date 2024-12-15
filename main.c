#include <xc.h>
#include "blade.h"
#include "setup.h"
#include "can.h"
#include "usb.h"

int main(void) {
    setup();

    for (uint16_t i = 0; i < NUM_SLOTS; i++) {
        // Set everything as inputs to start
        set_lat(slots[i].d1, false);
        set_lat(slots[i].d2, false);

        set_tris(slots[i].pps1, true);
        set_tris(slots[i].pps2, true);
        set_tris(slots[i].an1, true);
        set_tris(slots[i].an2, true);
        set_tris(slots[i].d1, true);
        set_tris(slots[i].d2, true);

        set_ansel(slots[i].pps1, false);
        set_ansel(slots[i].pps2, false);
        set_ansel(slots[i].an1, true);
        set_ansel(slots[i].an2, true);
        set_ansel(slots[i].d1, false);
        set_ansel(slots[i].d2, false);
    }

    while (1) {
        can_heartbeat();
        usb_heartbeat();
    }

    return 0;
}
