/* keyboard-fw sim main: scan a synthetic pattern, send a KEYS frame. */
#include <stdio.h>

#include "logi/hal.h"
#include "logi/keyboard.h"

#ifndef LOGI_FW_LIBONLY
static const logi_led_t caps_led = { 12, false };
#endif

#ifndef LOGI_FW_LIBONLY
int main(int argc, char **argv) {
    (void)argc;
    (void)argv;
    hal_gpio_init();
    hal_led_init(&caps_led);
    uint16_t keys = kb_matrix_scan(0x03);       /* two keys pressed */
    logi_frame_t f;
    if (!kb_report_frame(&f, (uint8_t)hal_radio_tx_count(), keys) ||
        !hal_radio_send(f.buf, f.len)) {
        return 1;
    }
    hal_led_set(&caps_led, (keys & 0x8000u) != 0);
    printf("keyboard-fw sim: sent %u-byte KEYS frame (keys=0x%04X)\n",
           (unsigned)f.len, keys);
    return 0;
}
#endif
