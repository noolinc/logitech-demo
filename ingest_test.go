package main

import (
	"net/http"
	"net/http/httptest"
	"strings"
	"testing"
)

func TestValidateTLVsAcceptsKnownTypes(t *testing.T) {
	ev := DeviceEvent{
		ProtoVer: LogiProtoVer,
		TLVs: []TLV{
			{Type: TLVMotion, Len: 5},
			{Type: TLVBattery, Len: 1},
		},
	}
	if err := ValidateTLVs(ev); err != nil {
		t.Fatalf("expected ok, got %v", err)
	}
}

func TestValidateTLVsAcceptsBatteryHealthTLV(t *testing.T) {
	// changeset 8da82312: TLV 0x11 is additive to proto v2
	ev := DeviceEvent{
		ProtoVer: LogiProtoVer,
		TLVs:     []TLV{{Type: TLVBatteryHealth, Len: 3}},
	}
	if err := ValidateTLVs(ev); err != nil {
		t.Fatalf("expected ok, got %v", err)
	}
}

func TestValidateTLVsRejectsUnknownType(t *testing.T) {
	ev := DeviceEvent{ProtoVer: LogiProtoVer, TLVs: []TLV{{Type: 0x7F}}}
	err := ValidateTLVs(ev)
	if err == nil {
		t.Fatal("expected refusal for unknown TLV")
	}
	if got := httpErrorDomain(err); got != http.StatusUnsupportedMediaType {
		t.Fatalf("expected 415-class refusal, got %v", err)
	}
}

func TestValidateTLVsRejectsFutureProtoVer(t *testing.T) {
	ev := DeviceEvent{ProtoVer: LogiProtoVer + 1}
	if err := ValidateTLVs(ev); err == nil {
		t.Fatal("expected refusal for future protoVer")
	}
}

type errBody struct{ msg string }

func (e errBody) Error() string { return e.msg }

func httpErrorDomain(err error) int {
	if err == nil {
		return 0
	}
	if strings.Contains(err.Error(), "unknown TLV") {
		return http.StatusUnsupportedMediaType
	}
	return http.StatusBadRequest
}

func TestIngestWindow_RingEviction(t *testing.T) {
	w := NewIngestWindow(2)
	for i := 0; i < 5; i++ {
		w.Push(DeviceEvent{Seq: i})
	}
	if w.Len() != 2 {
		t.Fatalf("window must keep max=2, got %d", w.Len())
	}
}

func TestIngestHTTPHandler(t *testing.T) {
	rec := httptest.NewRecorder()
	req := httptest.NewRequest(http.MethodPost, "/v1/device-events",
		strings.NewReader(`{"deviceId":"LOGI-00A1","deviceType":"mouse","protoVer":2,"seq":1,"tlvs":[]}`))
	handleIngest(rec, req)
	if rec.Code != http.StatusAccepted {
		t.Fatalf("expected 202, got %d", rec.Code)
	}
}

func TestIngestHTTPHandlerBadTLV(t *testing.T) {
	rec := httptest.NewRecorder()
	req := httptest.NewRequest(http.MethodPost, "/v1/device-events",
		strings.NewReader(`{"deviceId":"LOGI-00A1","deviceType":"mouse","protoVer":2,"seq":1,"tlvs":[{"type":127}]}`))
	handleIngest(rec, req)
	if rec.Code != http.StatusUnsupportedMediaType {
		t.Fatalf("expected 415, got %d", rec.Code)
	}
}
