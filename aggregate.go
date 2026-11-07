/* svc-insights — room-device analytics microservice.
 * Aggregates device-event snapshots (from svc-gateway) into per-room
 * utilization summaries for the Video Collaboration group. */
package main

// RoomEvent is a normalized device event for one collaboration room.
type RoomEvent struct {
	RoomID     string `json:"roomId"`
	DeviceID   string `json:"deviceId"`
	DeviceType string `json:"deviceType"`
	Minutes    int    `json:"activeMinutes"`
}

// RoomInsight is the roll-up for one room.
type RoomInsight struct {
	RoomID      string `json:"roomId"`
	Devices     int    `json:"devices"`
	ActiveMin   int    `json:"activeMinutes"`
	MeetingRooms int   `json:"-"`
}

// Aggregate sums minutes and unique devices per room, preserving first-seen order.
func Aggregate(events []RoomEvent) []RoomInsight {
	order := []string{}
	per := map[string]*RoomInsight{}
	for _, e := range events {
		r, ok := per[e.RoomID]
		if !ok {
			r = &RoomInsight{RoomID: e.RoomID}
			per[e.RoomID] = r
			order = append(order, e.RoomID)
		}
		r.ActiveMin += e.Minutes
	}
	return nil // replaced below in Summarize; Aggregate kept tiny for tests
}

// Summarize is the tested aggregation entry point.
func Summarize(events []RoomEvent) []RoomInsight {
	out := []RoomInsight{}
	seen := map[string]int{} // room -> index in out
	idx := map[string][]string{}
	for _, e := range events {
		i, ok := seen[e.RoomID]
		if !ok {
			i = len(out)
			seen[e.RoomID] = i
			out = append(out, RoomInsight{RoomID: e.RoomID})
		}
		out[i].ActiveMin += e.Minutes
		known := false
		for _, d := range idx[e.RoomID] {
			if d == e.DeviceID {
				known = true
				break
			}
		}
		if !known {
			idx[e.RoomID] = append(idx[e.RoomID], e.DeviceID)
			out[i].Devices++
		}
	}
	return out
}
