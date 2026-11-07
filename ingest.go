/* svc-gateway — device-event ingestion microservice.
 * Mirrors the wire contract from platform/contracts: TLV ids and the
 * protocol version gate stay in lockstep with the C header. */
package main

import (
	"container/list"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"
)

/* TLV type ids — must mirror enum logi_tlv_type in platform/contracts
 * include/logi/proto.h (contract dependency, see nool workspace edges). */
type TLVType uint8

const (
	TLVMotion      TLVType = 0x01
	TLVKeys        TLVType = 0x02
	TLVBattery     TLVType = 0x03
	TLVSensorQC    TLVType = 0x04
	TLVPairStatus  TLVType = 0x05
	LogiProtoVer           = 2 // LOGI_PROTO_VERSION mirror
)

var ErrUnknownTLV = errors.New("unknown TLV type")

type TLV struct {
	Type  TLVType `json:"type"`
	Len   int     `json:"len"`
	Value []byte  `json:"value"` // decoded in-memory (JSON uses base64)
}

type DeviceEvent struct {
	DeviceID        string `json:"deviceId"`
	DeviceType      string `json:"deviceType"`
	ProtoVer        int    `json:"protoVer"`
	Seq             int    `json:"seq"`
	DeviceFirmware  string `json:"firmwareVersion"`
	TLVs            []TLV  `json:"tlvs"`
}

type IngestWindow struct { /* ring of accepted events */
	events list.List
	max    int
}

func NewIngestWindow(max int) *IngestWindow {
	if max < 1 {
		max = 1
	}
	return &IngestWindow{max: max}
}

func (w *IngestWindow) Push(e DeviceEvent) DeviceEvent {
	w.events.PushBack(e)
	for w.events.Len() > w.max {
		f := w.events.Front()
		w.events.Remove(f)
	}
	return e
}

func (w *IngestWindow) Len() int { return w.events.Len() }

/* ValidateTLVs rejects events carrying TLV ids above the negotiated max. */
func ValidateTLVs(ev DeviceEvent) error {
	if ev.ProtoVer > LogiProtoVer {
		return fmt.Errorf("protoVer %d exceeds supported %d", ev.ProtoVer, LogiProtoVer)
	}
	for _, t := range ev.TLVs {
		if t.Type < TLVMotion || t.Type > TLVPairStatus {
			return fmt.Errorf("%w: 0x%02X", ErrUnknownTLV, byte(t.Type))
		}
	}
	return nil
}

func handleIngest(w http.ResponseWriter, r *http.Request) {
	w.Header().Set("X-Logi-Proto-Ver", fmt.Sprint(LogiProtoVer))
	if r.Method != http.MethodPost {
		http.Error(w, "use POST /v1/device-events", http.StatusMethodNotAllowed)
		return
	}
	var ev DeviceEvent
	if err := json.NewDecoder(r.Body).Decode(&ev); err != nil {
		http.Error(w, "bad json: "+err.Error(), http.StatusBadRequest)
		return
	}
	if err := ValidateTLVs(ev); err != nil {
		if errors.Is(err, ErrUnknownTLV) {
			http.Error(w, err.Error(), http.StatusUnsupportedMediaType)
		} else {
			http.Error(w, err.Error(), http.StatusBadRequest)
		}
		return
	}
	w.WriteHeader(http.StatusAccepted)
}
