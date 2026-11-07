package main

import "testing"

func TestSummarizeAggregatesPerRoom(t *testing.T) {
	events := []RoomEvent{
		{RoomID: "R-101", DeviceID: "LOGI-D1", Minutes: 30},
		{RoomID: "R-101", DeviceID: "LOGI-D1", Minutes: 15},
		{RoomID: "R-101", DeviceID: "LOGI-D2", Minutes: 5},
		{RoomID: "R-102", DeviceID: "LOGI-D3", Minutes: 45},
	}
	got := Summarize(events)
	if len(got) != 2 {
		t.Fatalf("expected 2 rooms, got %d", len(got))
	}
	if got[0].RoomID != "R-101" || got[0].ActiveMin != 50 || got[0].Devices != 2 {
		t.Fatalf("R-101 mismatch: %+v", got[0])
	}
	if got[1].RoomID != "R-102" || got[1].ActiveMin != 45 || got[1].Devices != 1 {
		t.Fatalf("R-102 mismatch: %+v", got[1])
	}
}

func TestSummarizeEmpty(t *testing.T) {
	if got := Summarize(nil); len(got) != 0 {
		t.Fatalf("expected empty, got %v", got)
	}
}
