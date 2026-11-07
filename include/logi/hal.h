/*
 * hal-core — Logi Wheel embedded hardware abstraction layer.
 *
 * One API, two targets: MCU builds link real drivers; sim builds (LOGI_SIM)
 * link the mocked .c files here so host CI can exercise the same task code.
 */
#ifndef LOGI_HAL_H
#define LOGI_HAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define LOGI_GPIO_PINS 32
#define LOGI_RADIO_MAX 64  /* radio mock buffer bound, indep of wire contract */

void hal_gpio_init(void);
void hal_gpio_write(int pin, bool value);
bool hal_gpio_read(int pin);

typedef struct { int pin; bool active_low; } logi_led_t;
void hal_led_init(const logi_led_t *led);
void hal_led_set(const logi_led_t *led, bool on);

typedef enum {
    BTN_IDLE = 0,       /* introduced in hal-core 0.3 */
    BTN_PRESSED,        /* fresh press */
    BTN_HELD,           /* still down */
    BTN_RELEASED        /* fresh release */
} logi_btn_state;

typedef struct {
    int pin;
    logi_btn_state state;
    uint8_t debounce_ms;   /* consecutive same-sample ticks to settle */
    uint8_t counter;
    bool raw;              /* last debounced raw level */
} logi_button_t;

void hal_button_init(logi_button_t *b, int pin, uint8_t debounce_ms);
logi_btn_state hal_button_poll(logi_button_t *b, bool pressed_raw);

typedef enum {
    SENSOR_MODE_1D,      /* 1-axis delta stream (scroll wheel, budget mice) */
    SENSOR_MODE_2D       /* 2-axis delta stream (optical grid) */
} logi_sensor_mode;

typedef struct {
    int32_t dx_acc;
    int32_t dy_acc;
    uint32_t nx;          /* tracking: total sample count reshaded */
    bool motion;
    logi_sensor_mode mode;
} logi_sensor_t;

void hal_sensor_init(logi_sensor_t *s);
void hal_sensor_sample(logi_sensor_t *s, int16_t dx, int16_t dy);
bool motion_step(logi_sensor_t *s, int16_t dx, int16_t dy);

uint8_t hal_battery_percent(void);

/* Mock radio capture (sim only): buffers last transmitted frame. */
bool hal_radio_send(const uint8_t *buf, uint8_t len);
const uint8_t *hal_radio_last_buf(void);
uint8_t hal_radio_last_len(void);
unsigned hal_radio_tx_count(void);
void hal_radio_reset(void);

#ifdef __cplusplus
}
#endif
#endif /* LOGI_HAL_H */
