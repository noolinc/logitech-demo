#include <assert.h>
#include <stdio.h>

#include "logi/hal.h"

static void test_button_debounce(void) {
    logi_button_t b;
    hal_button_init(&b, 4, 3);

    /* 2 noisy samples must not settle */
    assert(hal_button_poll(&b, true) == BTN_IDLE);
    assert(hal_button_poll(&b, true) == BTN_IDLE);
    /* 3rd consecutive sample settles */
    assert(hal_button_poll(&b, true) == BTN_PRESSED);
    /* held becomes BTN_HELD */
    assert(hal_button_poll(&b, true) == BTN_HELD);
    /* release needs debounce again */
    assert(hal_button_poll(&b, false) == BTN_HELD);
    assert(hal_button_poll(&b, false) == BTN_HELD);
    assert(hal_button_poll(&b, false) == BTN_RELEASED);
    assert(hal_button_poll(&b, false) == BTN_IDLE);
}

static void test_led_and_radio(void) {
    hal_gpio_init();
    logi_led_t led = { 9, false };
    hal_led_init(&led);
    hal_led_set(&led, true);
    assert(hal_gpio_read(9));

    assert(!hal_radio_send((const uint8_t[]){0}, 0));       /* too short */
    assert(!hal_radio_send(NULL, 4));                        /* null */
    assert(hal_radio_send((const uint8_t[]){1, 2, 3, 4}, 4));
    assert(hal_radio_last_len() == 4);
    assert(hal_radio_tx_count() == 1);
}

int main(void) {
    test_button_debounce();
    test_led_and_radio();
    printf("hal-core tests OK\n");
    return 0;
}
