#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "logi/hal.h"
#include "logi/keyboard.h"

int main(void) {
    logi_frame_t f;
    assert(kb_report_frame(&f, 2, 0x8001));
    uint8_t ver, seq, plen;
    const uint8_t *pl;
    assert(logi_frame_parse(f.buf, f.len, &ver, &seq, &pl, &plen));
    assert(seq == 2 && plen == 2);
    assert(pl[0] == 0x01 && pl[1] == 0x80);
    /* frame stays inside LOGI_FRAME_MAX budget */
    assert(f.len <= LOGI_FRAME_MAX);
    printf("keyboard-fw tests OK\n");
    return 0;
}
