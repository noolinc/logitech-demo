#include "logi/receiver.h"

#include <string.h>

bool rx_forward(const uint8_t *raw, uint8_t n, uint8_t *hid_bytes_out) {
    uint8_t ver, seq, plen;
    const uint8_t *pl;
    if (!logi_frame_parse(raw, n, &ver, &seq, &pl, &plen)) {
        return false;   /* corrupt or foreign frame: drop */
    }
    if (ver != LOGI_PROTO_VERSION || plen < 3 || hid_bytes_out == NULL) {
        return false;
    }
    /* host HID mapping (mock): HID-ish 2-byte report from motion TLV */
    hid_bytes_out[0] = pl[0];
    hid_bytes_out[1] = pl[1];
    return true;
}
