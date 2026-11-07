#include "logi/hal.h"

void hal_button_init(logi_button_t *b, int pin, uint8_t debounce_ms) {
    b->pin = pin;
    b->state = BTN_IDLE;
    b->debounce_ms = debounce_ms > 0 ? debounce_ms : 1u;
    b->counter = 0u;
    b->raw = false;
}

logi_btn_state hal_button_poll(logi_button_t *b, bool pressed_raw) {
    if (pressed_raw != b->raw) {
        /* level changed: count consecutive differing samples to settle */
        if (++b->counter >= b->debounce_ms) {
            bool was_down = (b->state == BTN_PRESSED || b->state == BTN_HELD);
            b->raw = pressed_raw;
            b->counter = 0u;
            b->state = pressed_raw ? BTN_PRESSED
                                   : (was_down ? BTN_RELEASED : BTN_IDLE);
        }
    } else if (pressed_raw && b->state == BTN_PRESSED) {
        b->counter = 0u;
        b->state = BTN_HELD;
    } else if (!pressed_raw && b->state == BTN_RELEASED) {
        b->counter = 0u;
        b->state = BTN_IDLE;
    }
    return b->state;
}
