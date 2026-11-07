#include "logi/keyboard.h"

#include <string.h>

bool kb_report_frame(logi_frame_t *f, uint8_t seq, uint16_t key_bits) {
    uint8_t pl[2];
    pl[0] = (uint8_t)(key_bits & 0xFFu);
    pl[1] = (uint8_t)((key_bits >> 8) & 0xFFu);
    return logi_frame_build(f, LOGI_PROTO_VERSION, seq, pl, sizeof pl);
}
