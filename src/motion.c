#include "logi/mouse.h"

#include <string.h>

/* fuel-gauge source: percent from the ADC API, cycles/temp from NVM mock */
#include "logi/hal.h"

bool mouse_batt_health_frame(logi_frame_t *f, uint8_t seq) {
    uint8_t pl[6];   /* TLV header (2) + value (3) */
    size_t off = 0;
    uint8_t health[3];
    health[0] = hal_battery_percent();
    health[1] = 214u;      /* pack cycles, fuel-gauge NVM (mock feed) */
    health[2] = 31u;       /* pack temp in C */
    off = logi_tlv_write(pl, sizeof pl, off, TLV_BATTERY_HEALTH, health, 3u);
    return logi_frame_build(f, LOGI_PROTO_VERSION, seq, pl, (uint8_t)off);
}

bool mouse_motion_frame(logi_frame_t *f, uint8_t seq,
                        int32_t dx, int32_t dy, uint8_t buttons) {
    /* wire layout: dx, dy as signed 8-bit deltas (clamped), then buttons */
    int dx8 = dx > 127 ? 127 : (dx < -128 ? -128 : (int)(signed char)dx);
    int dy8 = dy > 127 ? 127 : (dy < -128 ? -128 : (int)(signed char)dy);
    (void)buttons;
    uint8_t pl[5];
    pl[0] = (uint8_t)dx8;
    pl[1] = (uint8_t)dy8;
    pl[2] = buttons;
    pl[3] = 0u;   /* reserved */
    pl[4] = 0u;   /* reserved */
    return logi_frame_build(f, LOGI_PROTO_VERSION, seq, pl, sizeof pl);
}
