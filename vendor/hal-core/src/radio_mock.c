/* Mock radio for sim builds: captures the last frame so tests can assert. */
#include "logi/hal.h"

#include <string.h>

static uint8_t last_buf[LOGI_RADIO_MAX];
static uint8_t last_len = 0u;
static unsigned tx_count = 0u;
static unsigned tx_window = 0u;   /* CYW43-class TX window counter */

bool hal_radio_send(const uint8_t *buf, uint8_t len) {
    if (buf == NULL || len == 0u || len > LOGI_RADIO_MAX) {
        return false;
    }
    memcpy(last_buf, buf, len);
    last_len = len;
    tx_count++;
    tx_window++;
    return true;
}

const uint8_t *hal_radio_last_buf(void) { return last_buf; }
uint8_t hal_radio_last_len(void) { return last_len; }
unsigned hal_radio_tx_count(void) { return tx_count; }

void hal_radio_reset(void) {
    last_len = 0u;
    tx_count = 0u;
    tx_window = 0u;
}
