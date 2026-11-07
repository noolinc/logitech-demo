package main

import (
	"fmt"
	"net/http"
)

func serve() error {
	mux := http.NewServeMux()
	mux.HandleFunc("/v1/device-events", handleIngest)
	fmt.Println("svc-gateway listening on :8080")
	return http.ListenAndServe(":8080", mux)
}

func main() {
	if err := serve(); err != nil {
		panic(err)
	}
}
