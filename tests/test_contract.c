/* Compile-time contract check: header builds standalone and constants are sane. */
#include <assert.h>
#include <stdio.h>
#include "logi/proto.h"

int main(void) {
    assert(LOGI_PROTO_VERSION >= 1);
    assert(LOGI_FRAME_MAX == LOGI_PAYLOAD_MAX + 6);
    assert(TLV_MOTION < TLV_PROTO_MIN + 10);
    assert(LOGI_BATT_CRITICAL_PCT < LOGI_BATT_LOW_PCT);
    printf("contracts tests OK\n");
    return 0;
}
