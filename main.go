package main

func main() {
	// entry point; aggregation logic lives in aggregate.go to stay testable.
	_ = Summarize(nil)
	_ = Aggregate(nil)
}
