# 01 — Demo Script (20 minutes, one terminal)

Audience assumption: mixed — firmware engineers, a software-lead, someone from
quality/compliance. The script hits all three. Timings are aggressive; the
single most important 7 minutes are minutes 4–11 (the changeset).

**Rules of engagement while demoing:**
- Every command below was executed in this workspace on 2026-11-07; expected
  outputs are in [`artifacts/`](artifacts/). If live execution misbehaves for
  ANY reason, switch to the artifact verbatim and keep the narrative moving —
  never debug live on stage.
- Terminal setup: large font, one window, `cd /Users/arunbalakrishnan/Research/logitech`.
  Two panes only if demoing the fencing collision (see §2).

---

## 0:00–2:00 — The org you built for this conversation

```bash
cd /Users/arunbalakrishnan/Research/logitech
nool workspace status --compact          # → artifacts/workspace-status.txt
```

Talk track while it prints:
- "9 repos — the layout is yours: a `contracts` root (the HID++-style wire
  protocol), a platform group (protocol lib + HAL), three MCU firmwares, three
  cloud services. Sound familiar? The dependency edges on this screen are the
  real coupling — contract edges from two consumers onto `contracts`."
- Point at the dependency-edges and build-order sections: "This is computed,
  not maintained. Nobody writes a docs page that decays."

## 2:00–4:00 — A repo from the inside: what an agent sees

```bash
cd platform/contracts
nool status --compact
nool outline include/logi/proto.h          # one line per symbol, live-unparsed
nool show include/logi/proto.h::logi_tlv_type
nool changelog                             # → artifacts/contracts-changelog.txt
```

Talk track: "Signed receipts, not commit messages: every landing carries
intent, test-note, gate verdicts. This changelog is the auditors' version of
your PR history — generated as a by-product."

## 4:00–6:00 — Blast radius BEFORE the first edit

```bash
nool debug blast-radius include/logi/proto.h   # → artifacts/blast-radius.txt
nool query history include/logi/proto.h        # → artifacts/query-history.txt
```

Talk track: "Blast radius answers 'if I touch the protocol header, who buys
the change?' — before anyone writes code. History answers 'who changed this
and why' — one command, signed. Embedded reality check: your base-code porting
practice means one header ripples across SKUs; this is how you see that wave."

## 6:00–11:00 — ★ The main event: one change, four repos, zero meetings

```bash
cd /Users/arunbalakrishnan/Research/logitech       # root
nool workspace changeset show changeset:8da82312c254d981
# → artifacts/changeset-show.txt
```

Narrate exactly what this change set was: "Battery-health telemetry — TLV
0x11 across the 2.4 GHz link. We shipped it end-to-end earlier today; these
are the receipts: `contracts` landed knot 066348e7 (attested), `mouse-fw`
landed 8d5123f9, `svc-gateway` c1b85ff0 — and the manifest recorded
compatibility pins between them, so the workspace KNOWS mouse-fw's landing is
only valid against a specific contracts revision."

Then show one member's path in slow motion (this is the "what does an agent's
day look like" slide):

```bash
cd devices/mouse-fw
nool task list --compact                 # the changeset's member tasks
nool log --compact --limit 12            # pick → start → intent → propose → seal → finish
```

Key talking points, in order:
- `task pick --id d702c536 -s` / `task start` — "state machine, sealed; the
  task binds the landing. Scope rails on the task (`fuzz/, Makefile, tests/`)
  are ENFORCED — a proposal that touches a rail is refused at the seal."
- `announce intent -k <paths>` — "a fencing lease with a sequence number.
  This is not a convention in a markdown file; it's a CAS-checked claim."
- `propose --task … --solidify` — "validation runs in an isolated private
  worktree — HEAD plus only this repo. Nobody's half-finished WIP can poison
  the verdict. Mutation probes then try to break your change against your
  own tests; the seal records whether your tests constrain the change."

## 11:00–13:00 — The gates that caught real bugs (proof, not marketing)

Show the two live catches, pre-staged in terminal scrollback or via artifacts:

1. **Fencing**: "A propose under a different agent id was refused with a
   coordination conflict — against the lease its own colleague held. Every
   co-located agent needs a stable identity (NOOL_AGENT_ID), and stale leases
   are refused at solidify even if they looked valid at grant time."
2. **Repo-isolated build**: "A makefile that reached into `../contracts`
   passed locally and failed reification. The org learned this ONCE, the
   lesson is a signed finding (nool learn 05c5f78d), and the policy — vendor
   shared sources — is now executable history."

## 13:00–16:00 — Ship it: doctor → checkpoint → audit evidence

```bash
cd platform/contracts
nool doctor                  # → artifacts/contracts-doctor.txt
nool changelog | tail -8     # release 0.2.0 knot visible
nool audit verify --since 3d --framework pci-dss-4.0.1
# → artifacts/contracts-audit-pci.after-repair.txt
```

Talk track: "Exit-code discipline: 2 = policy says no (fix it), 3 = someone
else holds the scope (back off), 4 = edition boundary. `doctor --strict` is
your release gate. And this audit line — 'signed by ed25519 key' — is the
evidence artifact for LVFS submissions or your Select-tier enterprise claims:
signed, replayable from a fresh clone."

## 16:00–18:00 — The goal spine (who does what, attributed, org-wide)

```bash
cd /Users/arunbalakrishnan/Research/logitech
nool workspace goal-status --goal ship-battery-health-telemetry-end-to-end 2>/dev/null || \
  nool workspace goals
nool usage thread --name ship-battery-health-telemetry-end-to-end 2>/dev/null || \
  nool usage --compact
```

Talk track: "Org → dept → team → project → epic → task, one spine everywhere.
Tasks carry acceptance criteria and paid-token attribution. This is what you
show the CFO when they ask what agent time actually bought."

## 18:00–20:00 — Close on their number

- "Your firmware model is waterfall-to-factory with fixed scope. That's not a
  limitation of agile — it's a compliance property. Fixed scope, enforced, is
  what `--rails` puts in config; hard dates are what `doctor --strict` +
  `checkpoint X.Y.Z` make auditable. Agents become safe BECAUSE the org stays
  strict."
- Ask: "Walk me through your last cross-repo protocol change — how many
  people tracked the ripple?" Then be quiet and let them answer.

---

## Live-collision optional segment (swap out if time is short)

Only if the room is firmware-heavy, run the fencing collision LIVE in two
panes (this was demonstrated in the build; commands are non-destructive):

```bash
# pane A
cd devices/mouse-fw && NOOL_AGENT_ID=agent-A nool announce intent \
    -k src/motion.c --intent "pane A holds scope"
# pane B (must be refused)
cd devices/mouse-fw && NOOL_AGENT_ID=agent-B nool propose \
    --path src/motion.c --intent "pane B collides" --solidify   # exit 3
# cleanup
NOOL_AGENT_ID=agent-A nool announce release <lease-id>
```

Failure protocol for this segment: if pane B does NOT refuse, stop the meeting
 momentarily, show the `announce status` output, and say "identity plumbing —
that's exactly why this table exists, and we run it as a pre-flight" →
[`04-environment-prep.md`](04-environment-prep.md).
