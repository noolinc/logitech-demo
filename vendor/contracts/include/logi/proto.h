/*
 * Logi Wheel — device-to-cloud wire protocol contracts.
 *
 * Single source of truth shared by every MCU firmware repo (C) and the cloud
 * services (Go/Python ports mirror these constants — keep them in lockstep).
 *
 * Frame layout (Nool's semantic layer sees these contract changes as the
 * dependency root of the workspace):
 *
 *   +--------+--------+-----+-----+---------+-------------+------+
 *   | SYNC0  | SYNC1  | ver | seq | payload | payload ... | crc8 |
 *   | 0x5A   | 0xA5   |  u8 | u8  | len     | plen bytes  |  u8  |
 *   +--------+--------+-----+-----+---------+-------------+------+
 *
 * TLV section inside the payload: [type][len][value...], len excludes header.
 */
#ifndef LOGI_PROTO_H
#define LOGI_PROTO_H

#include <stdint.h>

#define LOGI_PROTO_VERSION  2        /* wire protocol version */
#define LOGI_SYNC0          0x5A
#define LOGI_SYNC1          0xA5
#define LOGI_FRAME_MAX      64       /* max frame bytes */
#define LOGI_PAYLOAD_MAX    58       /* LOGI_FRAME_MAX - 6 overhead */
#define LOGI_TLV_MAX        16       /* max TLVs per payload */

/* TLV type IDs */
enum logi_tlv_type {
    TLV_MOTION        = 0x01,   /* 5B: dx_lo dx_hi dy_lo dy_hi buttons */
    TLV_KEYS          = 0x02,   /* 2B: key_bits_lo key_bits_hi (up to 16 keys) */
    TLV_BATTERY       = 0x03,   /* 1B: percent */
    TLV_SENSOR_QC     = 0x04,   /* 1B: quality code */
    TLV_PAIR_STATUS   = 0x05,   /* 1B: pairing state */
    TLV_BATTERY_HEALTH = 0x11,  /* 3B: pct, cycles, temp_c, additive to v2 */
    TLV_PROTO_MIN     = 0x01,
    TLV_PROTO_MAX     = 0x05
};

/* Battery percent thresholds shared by firmware and cloud alerts */
enum logi_battery {
    LOGI_BATT_LOW_PCT     = 15,
    LOGI_BATT_CRITICAL_PCT = 5
};

#endif /* LOGI_PROTO_H */
