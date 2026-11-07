#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "logi/hal.h"
#include "logi/receiver.h"

int main(void) {
    logi_frame_t f;
    /* build a valid motion frame (as mouse-fw would send) */
    uint8_t pl[3] = { 0x01, 0x02, 0x00 };
    assert(logi_frame_build(&f, LOGI_PROTO_VERSION, 9, pl, sizeof pl));

    uint8_t hid[2];
    assert(rx_forward(f.buf, f.len, hid));
    assert(hid[0] == 0x01 && hid[1] == 0x02);

    /* corrupt frame must be dropped */
    f.buf[f.len - 1] ^= 0xFF;
    assert(!rx_forward(f.buf, f.len, hid));

    /* pairing: pair frame moves RX_PAIRING -> RX_READY */
    assert(rx_pairing_advance(RX_PAIRING, true) == RX_READY);
    assert(rx_pairing_advance(RX_READY, false) == RX_READY);
    printf("receiver-fw tests OK\n");
    return 0;
}
