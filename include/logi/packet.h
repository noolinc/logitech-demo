/*
 * mcu-protocol — Logi Wheel MCU wire-framing library.
 *
 * Builds and parses the frame defined by platform/contracts.
 * Runs on-mcu (same C source) and in sim builds for host tests.
 */
#ifndef LOGI_PACKET_H
#define LOGI_PACKET_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "logi/proto.h"

typedef struct {
    uint8_t buf[LOGI_FRAME_MAX];
    uint8_t len;
} logi_frame_t;

/* Build frame: SYNC0 SYNC1 ver seq plen payload... crc8 */
bool logi_frame_build(logi_frame_t *f, uint8_t ver, uint8_t seq,
                      const uint8_t *payload, uint8_t plen);

/* Parse and CRC-check a raw buffer; returns false on any corruption. */
bool logi_frame_parse(const uint8_t *raw, uint8_t n,
                      uint8_t *ver, uint8_t *seq,
                      const uint8_t **payload, uint8_t *plen);

/* Append a TLV at off; returns new offset (unchanged if too large). */
size_t logi_tlv_write(uint8_t *dst, size_t cap, size_t off,
                      uint8_t type, const void *val, uint8_t vlen);

/* Iterate TLVs inside a payload; returns offset of TLV header, or (size_t)-1. */
size_t logi_tlv_find(const uint8_t *payload, uint8_t plen, uint8_t type,
                     const uint8_t **val, uint8_t *vlen);

uint8_t logi_crc8(const uint8_t *data, size_t len);

#ifdef __cplusplus
}
#endif
#endif /* LOGI_PACKET_H */
