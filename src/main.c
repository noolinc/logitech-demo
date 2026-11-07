/* mouse-fw main (sim build): superloop paces sensor sample, button poll,
 * and radio TX. Real MCU builds replace the mocked HAL sources. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "logi/hal.h"
#include "logi/mouse.h"

int mouse_main(int ticks) {
    hal_gpio_init();

    logi_button_t btns[2];
    hal_button_init(&btns[0], 10, 2);  /* left */
    hal_button_init(&btns[1], 11, 2);  /* right */

    logi_sensor_t sensor;
    hal_sensor_init(&sensor);

    static const logi_led_t link_led = { 9, false };
    hal_led_init(&link_led);

    int sent = 0;
    for (int t = 0; t < ticks; t++) {
        /* sim: feed sensor deltas every 2 ticks (real: SPI polling) */
        if (t % 2 == 1) {
            hal_sensor_sample(&sensor, 1, 0);
        }
        uint8_t pins[3] = {
            (uint8_t)hal_button_poll(&btns[0], t == 5),
            (uint8_t)hal_button_poll(&btns[1], false),
            0u
        };

        if (sensor.motion) {
            logi_frame_t f;
            if (mouse_motion_frame(&f, (uint8_t)hal_radio_tx_count(),
                                   sensor.dx_acc, sensor.dy_acc, 0u) &&
                hal_radio_send(f.buf, f.len)) {
                sent++;
            }
            sensor.dx_acc = 0;
            sensor.dy_acc = 0;
            sensor.motion = false;
        }
        /* battery-health telemetry: every 5 ticks (changeset incl. cloud) */
        if (t % 5 == 4) {
            logi_frame_t f;
            if (mouse_batt_health_frame(&f, (uint8_t)hal_radio_tx_count()) &&
                hal_radio_send(f.buf, f.len)) {
                sent++;
            }
        }
        (void)pins;
        (void)link_led;
    }
    return sent;
}

#ifndef LOGI_FW_LIBONLY
int main(int argc, char **argv) {
    int ticks = argc > 1 ? atoi(argv[1]) : 10;
    int sent = mouse_main(ticks);
    printf("mouse-fw sim: %d frames sent over %d ticks\n", sent, ticks);
    return sent > 0 ? 0 : 1;
}
#endif
