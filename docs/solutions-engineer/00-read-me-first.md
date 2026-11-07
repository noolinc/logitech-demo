# Nool × Logitech — Solutions Engineer Demo Kit

**Scenario:** the Logi Wheel workspace (this repository tree) demoed to
Logitech engineering leadership. Everything that follows is grounded in the
*actual, executed* build of this workspace on 2026-11-07 — knot ids referenced
in the scripts are real and reproducible.

Kit contents:

| Doc | Use it for |
|---|---|
| [`01-demo-script.md`](01-demo-script.md) | The 20-minute live demo, minute by minute, with exact commands and expected output |
| [`02-audience-playbook.md`](02-audience-playbook.md) | Who sits across the table, what each persona cares about, the opening line for each |
| [`03-objection-handling.md`](03-objection-handling.md) | Prepared answers to the pushback (git flow inertia, parser false positives, toolchains/HIL, GitLab, token costs) |
| [`04-environment-prep.md`](04-environment-prep.md) | Machine checklist before you walk in; what to pre-wire; fallback plan |
| [`05-metrics-and-roi.md`](05-metrics-and-roi.md) | The numbers story: NPI framing, token telemetry, capacity math |
| [`artifacts/`](artifacts/) | Real captured outputs — pairs with the demo script, doubles as "network dies" fallback |

## The 30-second frame (say this first)

> "Your firmware org already runs two processes at once: waterfall into the
> factory date for devices, agile for the software. Your 2026 job posts require
> AI-tool fluency. What this demo shows is the missing third piece: a way for
> AI agents — and humans — to write firmware *against a factory deadline* where
> every change is fenced, tested, and signed before it lands, and one contract
> change can be tracked across every repo it touches. We built a working
> miniature of your org to prove it: 9 repos, real embedded C, a multi-repo
> change shipped end-to-end today."

## The three claims everything must support

1. **Agents can parallelize NPI firmware work safely** (leases + fencing +
   ghost-run, not markdown conventions).
2. **One contract change across repos is a first-class object** (changeset with
   per-repo tasks and compatibility pins).
3. **Ship evidence is generated as a by-product** (doctor --strict →
   checkpoint X.Y.Z → signed audit export).

If a slide/proposal doesn't support one of those three, cut it.
