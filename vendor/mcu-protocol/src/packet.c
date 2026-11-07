#include "logi/packet.h"

#include <string.h>

uint8_t logi_crc8(const uint8_t *data, size_t len) {
    uint8_t c = 0xFF;
    for (size_t i = 0; i < len; i++) {
        c ^= data[i];
        for (int b = 0; b < 8; b++) {
            c = (uint8_t)((c & 0x80u) ? (uint8_t)(c << 1) ^ 0x31u : (uint8_t)(c << 1));
        }
    }
    return c;
}

bool logi_frame_build(logi_frame_t *f, uint8_t ver, uint8_t seq,
                      const uint8_t *payload, uint8_t plen) {
    if (f == NULL || payload == NULL || plen == 0 || plen > LOGI_PAYLOAD_MAX) {
        return false;
    }
    f->len = (uint8_t)(plen + 6);
    f->buf[0] = LOGI_SYNC0;
    f->buf[1] = LOGI_SYNC1;
    f->buf[2] = ver;
    f->buf[3] = seq;
    f->buf[4] = plen;
    memcpy(&f->buf[5], payload, plen);
    f->buf[5 + plen] = logi_crc8(f->buf, (size_t)(5 + plen));
    return true;
}

bool logi_frame_parse(const uint8_t *raw, uint8_t n,
                      uint8_t *ver, uint8_t *seq,
                      const uint8_t **payload, uint8_t *plen) {
    if (raw == NULL || n < 6 || n > LOGI_FRAME_MAX) {
        return false;
    }
    if (raw[0] != LOGI_SYNC0 || raw[1] != LOGI_SYNC1) {
        return false;
    }
    uint8_t l = raw[4];
    if ((uint8_t)(l + 6) != n) {
        return false;
    }
    if (raw[5 + l] != logi_crc8(raw, (size_t)(5 + l))) {
        return false;
    }
    if (ver != NULL)   *ver = raw[2];
    if (seq != NULL)   *seq = raw[3];
    if (plen != NULL)  *plen = l;
    if (payload != NULL) *payload = &raw[5];
    return true;
}

size_t logi_tlv_write(uint8_t *dst, size_t cap, size_t off,
                      uint8_t type, const void *val, uint8_t vlen) {
    if (dst == NULL || val == NULL || vlen == 0) {
        return off;
    }
    if (off + (size_t)vlen + 2u > cap) {
        return off;
    }
    dst[off] = type;
    dst[off + 1] = vlen;
    memcpy(&dst[off + 2], val, vlen);
    return off + 2u + (size_t)vlen;
}

size_t logi_tlv_find(const uint8_t *payload, uint8_t plen, uint8_t type,
                     const uint8_t **val, uint8_t *vlen) {
    size_t off = 0;
    while (off + 2u <= (size_t)plen) {
        uint8_t t = payload[off];
        uint8_t l = payload[off + 1];
        if (off + 2u + (size_t)l > (size_t)plen) {
            return (size_t)-1;  /* malformed TLV section */
        }
        if (t == type) {
            if (val)  *val = &payload[off + 2];
            if (vlen) *vlen = l;
            return off;
        }
        off += 2u + (size_t)l;
    }
    return (size_t)-1;
}
