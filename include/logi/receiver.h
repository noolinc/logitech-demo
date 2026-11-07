/* receiver-fw — Logi Bolt-class receiver: radio RX -> host forward (sim). */
#ifndef LOGI_RECEIVER_H
#define LOGI_RECEIVER_H

#include <stdbool.h>
#include <stdint.h>

#include "logi/packet.h"

typedef enum {
    RX_PAIRING = 0,
    RX_READY   = 1
} logi_rx_state_t;

/* Pairing state machine advance; returns the new state and status TLV. */
logi_rx_state_t rx_pairing_advance(logi_rx_state_t s, bool pair_frame);

/* Validate + forward one raw radio frame as a host HID event. */
bool rx_forward(const uint8_t *raw, uint8_t n, uint8_t *hid_bytes_out);

#endif /* LOGI_RECEIVER_H */
