package main

import "fmt"

func main() {
	targets, err := PlanRelease([]string{"LOGI-001A", "LOGI-00B2", "LOGI-00C3"}, 50)
	if err != nil {
		panic(err)
	}
	for _, tg := range targets {
		fmt.Printf("%s -> rollout=%v\n", tg.DeviceID, tg.Hit)
	}
}
