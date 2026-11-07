#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "logi/hal.h"
#include "logi/mouse.h"

static void test_motion_frame_wire(void) {
    logi_frame_t f;
    assert(mouse_motion_frame(&f, 7, 1, 2, MOUSE_BTN_LEFT));
    uint8_t ver, seq, plen;
    const uint8_t *pl;
    assert(logi_frame_parse(f.buf, f.len, &ver, &seq, &pl, &plen));
    assert(ver == LOGI_PROTO_VERSION);
    assert(seq == 7);
    assert(pl[0] == 1 && pl[1] == 2 && pl[2] == MOUSE_BTN_LEFT);
}

static void test_buttons_and_end_to_end(void) {
    uint8_t bits = mouse_button_bits(
        (const uint8_t[]){BTN_HELD, BTN_IDLE, BTN_PRESSED});
    assert(bits == (MOUSE_BTN_LEFT | MOUSE_BTN_MIDDLE));

    /* end-to-end: firmware main loop sends frames on the radio */
    assert(mouse_main(4) >= 1);
    const uint8_t *tx = hal_radio_last_buf();
    uint8_t n = hal_radio_last_len();
    assert(n > 0);
    assert(tx[0] == LOGI_SYNC0 && tx[1] == LOGI_SYNC1);
    uint8_t ver, seq, plen;
    const uint8_t *pl;
    assert(logi_frame_parse(tx, n, &ver, &seq, &pl, &plen));
}

int main(void) {
    test_motion_frame_wire();
    test_buttons_and_end_to_end();
    printf("mouse-fw tests OK\n");
    return 0;
}
