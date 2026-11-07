/* mouse-fw — MX Wheel wireless mouse firmware (superloop MCU, sim-buildable). */
#ifndef LOGI_MOUSE_H
#define LOGI_MOUSE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "logi/packet.h"

/* Motion task: build a MOTION TLV frame from sensor accumulation + buttons. */
bool mouse_motion_frame(logi_frame_t *f, uint8_t seq,
                        int32_t dx, int32_t dy, uint8_t buttons);

/* Buttons task: map raw pin states to the wire button bitmask. */
enum mouse_buttons {
    MOUSE_BTN_LEFT = 1u << 0,
    MOUSE_BTN_RIGHT = 1u << 1,
    MOUSE_BTN_MIDDLE = 1u << 2
};

uint8_t mouse_button_bits(const uint8_t *pins);

/* Superloop entry (sim build); exported for the smoke test. */
int mouse_main(int ticks);

#ifdef __cplusplus
}
#endif
#endif /* LOGI_MOUSE_H */
