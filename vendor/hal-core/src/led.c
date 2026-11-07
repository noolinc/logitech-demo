#include "logi/hal.h"

void hal_led_init(const logi_led_t *led) {
    hal_gpio_write(led->pin, led->active_low ? true : false);
}

void hal_led_set(const logi_led_t *led, bool on) {
    hal_gpio_write(led->pin, led->active_low ? !on : on);
}
