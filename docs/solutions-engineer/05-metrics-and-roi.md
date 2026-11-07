# 05 — Metrics & ROI (the numbers story)

Presales rule: numbers first where Logitech's own roadmap already has the
variable. Their roadmap has three priors: factory-date firmware (waterfall),
agent fluency in job specs, and an evidence burden (LVFS, Select-tier SLA).
Score the demo against those, not generic "productivity."

## Framing: NPI, not sprint

Talk in **agents per track**, not developers per sprint. Their own org talks
2,500-strong creator challenges and multi-track NPIs. Anchor:

- "One NPI firmware track ≈ 3–6 engineers × 12–20 weeks (public: EVT→DVT→PVT
  sequencing). The agent proposal is not 'replace'; it's 'n agents per track
  can run the parallel workstreams — drivers, unit tests, docs, fixture
  scripts — under your same gates.'"

## What the workspace already meters (no agent runs needed)

```bash
nool workspace goal-status --goal <goal>       # task completion across repos
nool thread show <epic> --json                 # cost attributed per task
nool usage --compact                           # tokens/cost by provider+model
nool usage thread --name <epic>                # writer-attributed only
nool task telemetry --id <id>                  # runs + reviewer-council spend
nool policy stats --since 30d --json           # which gates block how often
nool code metrics --days 30 --json             # per-verified-goal cost+time
```

These run TODAY on this workspace with zero model calls — the demo shows
receipts, not estimates. `policy stats` is the governance ROI chart: blocks
per gate, first-run pass rate; it doubles as which policies actually carry
their weight.

## The fleet/pyramid proof (post-sales)

`nool fleet capacity --builder <spec> --json` names the binder on this
machine (CPU / memory / daily budget). For Logitech: "give me your per-task
budget in dollars and your team's machine fleet; this command answers your
agent headcount ceiling before you spend a dollar of runs." Team-level:
`width=auto` folds cores, memory, remaining budget.

## LVFS / compliance math

A coordinated TLV change would normally mean chasing status across several
repos and CI systems after the fact. Here it is one object: the changeset
tracks per-member state, and the audit document is generated on demand
(`nool audit verify --framework pci-dss-4.0.1` — and SLSA/SOC2/ISO packs are
listed by `nool audit packs`). Every control verdict is signed; the export is
replayable from a fresh clone. "Your seasonal audit prep becomes a command
you run the day you file LVFS metadata."

## What to measure in the pilot (offer a 2-week instrumented pilot)

1. `verified goals / agent-week` from `nool code metrics` (their metric, not
   ours).
2. `blocks by gate` vs. human-review findings for the same period — the
   sponsorship number: how often did the machine catch what review would.
3. Multi-repo change → changeset lead time: open each member's first
   `task start` → last `task finish --landed-knot` timestamp delta.

Do NOT promise cost reduction on agent spend yet; that claim must come from
the pilot's own `usage` data, not from a slide.
