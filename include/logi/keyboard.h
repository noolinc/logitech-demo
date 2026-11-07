/* keyboard-fw — Logi Wheel 61-key compact wireless keyboard (sim-buildable).
 * Matrix scan + HID report assembly over the shared Logi wheel protocol. */
#ifndef LOGI_KEYBOARD_H
#define LOGI_KEYBOARD_H

#include <stdint.h>

#include "logi/packet.h"

#define KB_MAX_KEYS 16

/* Matrix scan: mock — synthetic key state from a row/col pattern (bg injects
 * via sim driver; real build scans GPIO row drivers). Returns key bitmask. */
uint16_t kb_matrix_scan(uint8_t pattern);

/* Build a KEYS TLV frame from a 16-bit key bitmap. */
bool kb_report_frame(logi_frame_t *f, uint8_t seq, uint16_t key_bits);

#endif /* LOGI_KEYBOARD_H */
