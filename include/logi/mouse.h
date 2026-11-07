/* mouse-fw — MX Wheel wireless mouse firmware (superloop MCU, sim-buildable). */
#ifndef LOGI_MOUSE_H
#define LOGI_MOUSE_H

#include <stdint.h>

#include "logi/packet.h"

/* Motion task: build a MOTION TLV frame from sensor accumulation + buttons. */
bool mouse_motion_frame(logi_frame_t *f, uint8_t seq,
                        int32_t dx, int32_t dy, uint8_t buttons);

/* Battery-health task: build a TLV_BATTERY_HEALTH (0x11, additive to v2)
 * frame from the fuel gauge. Wire payload: pct, cycles, temp_c. */
bool mouse_batt_health_frame(logi_frame_t *f, uint8_t seq);

/* Buttons task: map raw pin states to the wire button bitmask. */
enum mouse_buttons {
    MOUSE_BTN_LEFT = 1u << 0,
    MOUSE_BTN_RIGHT = 1u << 1,
    MOUSE_BTN_MIDDLE = 1u << 2
};

uint8_t mouse_button_bits(const uint8_t *pins);

/* Superloop entry (sim build); exported for the smoke test. */
int mouse_main(int ticks);

#endif /* LOGI_MOUSE_H */
