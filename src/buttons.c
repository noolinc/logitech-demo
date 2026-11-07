#include "logi/mouse.h"
#include "logi/hal.h"

uint8_t mouse_button_bits(const uint8_t *pins) {
    if (pins == NULL) {
        return 0u;
    }
    uint8_t bits = 0u;
    if (pins[0] == BTN_PRESSED || pins[0] == BTN_HELD) bits |= MOUSE_BTN_LEFT;
    if (pins[1] == BTN_PRESSED || pins[1] == BTN_HELD) bits |= MOUSE_BTN_RIGHT;
    if (pins[2] == BTN_PRESSED || pins[2] == BTN_HELD) bits |= MOUSE_BTN_MIDDLE;
    return bits;
}
