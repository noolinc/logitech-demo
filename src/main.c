/* receiver-fw sim main: receive last radio frame, advance pairing, forward. */
#include <stdio.h>

#include "logi/hal.h"
#include "logi/receiver.h"

#ifndef LOGI_FW_LIBONLY
int main(void) {
    logi_rx_state_t st = RX_PAIRING;
    st = rx_pairing_advance(st, true);
    const uint8_t *raw = hal_radio_last_buf();
    uint8_t n = hal_radio_last_len();
    uint8_t hid[2];
    if (st != RX_READY || !rx_forward(raw, n, hid)) {
        printf("receiver-fw sim: no frame to forward (n=%u)\n", n);
        return 1;
    }
    printf("receiver-fw sim: forwarded 2-byte HID report (0x%02X 0x%02X)\n",
           hid[0], hid[1]);
    return 0;
}
#endif
