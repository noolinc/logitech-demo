#include "logi/hal.h"

static uint8_t mock_regs[LOGI_GPIO_PINS];

void hal_gpio_init(void) {
    for (int i = 0; i < LOGI_GPIO_PINS; i++) {
        mock_regs[i] = 0u;
    }
}

void hal_gpio_write(int pin, bool value) {
    if (pin >= 0 && pin < LOGI_GPIO_PINS) {
        mock_regs[pin] = value ? 1u : 0u;
    }
}

bool hal_gpio_read(int pin) {
    if (pin < 0 || pin >= LOGI_GPIO_PINS) {
        return false;
    }
    return mock_regs[pin] != 0u;
}
