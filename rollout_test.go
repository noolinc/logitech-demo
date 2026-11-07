package main

import (
	"errors"
	"testing"
)

func TestShouldRolloutDeterministic(t *testing.T) {
	a, err := ShouldRollout("LOGI-00A1", 50)
	if err != nil {
		t.Fatal(err)
	}
	b, _ := ShouldRollout("LOGI-00A1", 50)
	if a != b {
		t.Fatalf("nondeterministic rollout: %v vs %v", a, b)
	}
}

func TestShouldRollout100And0(t *testing.T) {
	if hit, _ := ShouldRollout("LOGI-FFFF", 100); !hit {
		t.Fatal("100% must always hit")
	}
	if hit, _ := ShouldRollout("LOGI-0001", 0); hit {
		t.Fatal("0% must never hit")
	}
}

func TestPlanReleaseBadPercent(t *testing.T) {
	_, err := PlanRelease([]string{"LOGI-1"}, 150)
	if err == nil || !errors.Is(err, ErrBadPercent) {
		t.Fatalf("expected ErrBadPercent, got %v", err)
	}
}
