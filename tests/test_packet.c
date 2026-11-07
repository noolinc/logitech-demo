#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "logi/packet.h"

static void test_roundtrip(void) {
    logi_frame_t f;
    uint8_t pl[8];
    size_t off = 0;

    off = logi_tlv_write(pl, sizeof pl, off, TLV_MOTION,
                         (const uint8_t[]){0x01, 0x00, 0x02, 0x00, 0x00}, 5);
    assert(off == 7);

    assert(logi_frame_build(&f, LOGI_PROTO_VERSION, 3, pl, (uint8_t)off));
    uint8_t ver = 0, seq = 0, plen = 0;
    const uint8_t *parsed = NULL;
    assert(logi_frame_parse(f.buf, f.len, &ver, &seq, &parsed, &plen));
    assert(ver == LOGI_PROTO_VERSION);
    assert(seq == 3);
    assert(plen == 7);
    assert(memcmp(parsed, pl, plen) == 0);
}

static void test_crc_reject(void) {
    logi_frame_t f;
    assert(logi_frame_build(&f, LOGI_PROTO_VERSION, 1,
                            (const uint8_t[]){0x03, 0x01, 0x2A}, 3));
    f.buf[f.len - 1] ^= 0xFF;  /* corrupt CRC */
    uint8_t ver;
    assert(!logi_frame_parse(f.buf, f.len, &ver, NULL, NULL, NULL));
}

static void test_tlv_find(void) {
    uint8_t pl[16];
    size_t off = 0;
    off = logi_tlv_write(pl, sizeof pl, off, TLV_BATTERY, (const uint8_t[]){87}, 1);
    off = logi_tlv_write(pl, sizeof pl, off, TLV_SENSOR_QC, (const uint8_t[]){9}, 1);
    const uint8_t *val = NULL;
    uint8_t vlen = 0;
    size_t at = logi_tlv_find(pl, (uint8_t)off, TLV_BATTERY, &val, &vlen);
    assert(at == 0);
    assert(vlen == 1);
    assert(val[0] == 87);
    assert(logi_tlv_find(pl, (uint8_t)off, 0x7F, &val, &vlen) == (size_t)-1);
}

int main(void) {
    test_roundtrip();
    test_crc_reject();
    test_tlv_find();
    printf("mcu-protocol tests OK\n");
    return 0;
}
