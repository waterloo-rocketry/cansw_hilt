#include <xc.h>
#include "blade.h"
#include "setup.h"
#include "can.h"
#include "usb.h"
#include "mcc_generated_files/usb/usb.h"

int main(void) {
    setup();
    
    set_lat(slots[9].d1.pin_n, true);
    set_lat(slots[9].d2.pin_n, false);
    set_tris(slots[9].pps1.pin_n, false);
    set_tris(slots[9].pps2.pin_n, false);
    set_tris(slots[9].an1.pin_n, true);
    set_tris(slots[9].an2.pin_n, true);
    set_tris(slots[9].d1.pin_n, false);
    set_tris(slots[9].d2.pin_n, false);
    set_ansel(slots[9].an1.pin_n, true);
    set_ansel(slots[9].an2.pin_n, true);
    
    set_lat(slots[8].d1.pin_n, true);
    set_lat(slots[8].d2.pin_n, false);
    set_tris(slots[8].pps1.pin_n, true);
    set_tris(slots[8].pps2.pin_n, true);
    set_tris(slots[8].an1.pin_n, true);
    set_tris(slots[8].an2.pin_n, true);
    set_tris(slots[8].d1.pin_n, false);
    set_tris(slots[8].d2.pin_n, false);
    set_ansel(slots[8].pps1.pin_n, true);
    set_ansel(slots[8].pps2.pin_n, true);
    set_ansel(slots[8].an1.pin_n, true);
    set_ansel(slots[8].an2.pin_n, true);
    
    
    
    uint32_t res;
    while (1) {
        can_heartbeat();
        usb_heartbeat();
        
        res++;
        if (res > 600000) {
            res = 0;
        }
    }

    return 0;
}
