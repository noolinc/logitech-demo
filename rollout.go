/* svc-ota — firmware OTA rollout microservice.
 * Deterministic cohort-based rollout: device ids hash into the rollout
 * percent; the same device always gets the same answer for a plan. */
package main

import (
	"crypto/sha256"
	"encoding/binary"
	"errors"
	"fmt"
)// ErrBadPercent is returned for rollout percentages outside [0, 100].
var ErrBadPercent = errors.New("rollout percent must be in [0, 100]")

// ShouldRollout reports whether deviceID belongs to the pct-percent cohort.
// Deterministic: same device + same pct -> same answer.
func ShouldRollout(deviceID string, pct int) (bool, error) {
	if pct < 0 || pct > 100 {
		return false, ErrBadPercent
	}
	sum := sha256.Sum256([]byte(deviceID))
	n := binary.BigEndian.Uint32(sum[:4])
	return int(n%100) < pct, nil
}

// PlanTarget is a device selected by PlanRelease.
type PlanTarget struct {
	DeviceID string `json:"deviceId"`
	Hit      bool   `json:"hit"`
}

// PlanRelease evaluates a device list against a rollout percent.
func PlanRelease(devices []string, pct int) ([]PlanTarget, error) {
	out := make([]PlanTarget, 0, len(devices))
	for _, d := range devices {
		hit, err := ShouldRollout(d, pct)
		if err != nil {
			return nil, fmt.Errorf("plan %s: %w", d, err)
		}
		out = append(out, PlanTarget{DeviceID: d, Hit: hit})
	}
	return out, nil
}
