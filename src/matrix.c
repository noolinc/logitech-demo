#include "logi/keyboard.h"

uint16_t kb_matrix_scan(uint8_t pattern) {
    /* simulated matrix: bit i of pattern -> key i pressed */
    return (uint16_t)(pattern & ((1u << KB_MAX_KEYS) - 1u));
}
