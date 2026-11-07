#include "logi/receiver.h"

logi_rx_state_t rx_pairing_advance(logi_rx_state_t s, bool pair_frame) {
    return pair_frame ? RX_READY : s;
}
