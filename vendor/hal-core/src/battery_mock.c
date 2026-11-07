/* Mock battery gauge for sim builds. Real builds link fuel-gauge driver. */
#include "logi/hal.h"

uint8_t hal_battery_percent(void) {
    return 87u;
}
