#include "logi/hal.h"

void hal_sensor_init(logi_sensor_t *s) {
    s->dx_acc = 0;
    s->dy_acc = 0;
    s->nx = 0;
    s->motion = false;
    s->mode = SENSOR_MODE_2D;
}

void hal_sensor_sample(logi_sensor_t *s, int16_t dx, int16_t dy) {
    bool moved = (dx != 0 || dy != 0);
    if (s->mode == SENSOR_MODE_1D) {
        dy = 0;
    }
    s->dx_acc += dx;
    s->dy_acc += dy;
    s->nx++;
    s->motion = moved;
}

bool motion_step(logi_sensor_t *s, int16_t dx, int16_t dy) {
    hal_sensor_sample(s, dx, dy);
    bool moved = s->motion;
    if (moved) {
        s->dx_acc = 0;
        s->dy_acc = 0;
        s->motion = false;
    }
    return moved;
}
