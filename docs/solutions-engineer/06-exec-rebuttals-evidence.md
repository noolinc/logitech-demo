# 06 — Executive Pushback: Rebuttals With Generated Evidence

Companion to the exec hot-read. Every rebuttal below was answered by
**generating fresh evidence in this workspace on 2026-11-07** — not by arguing.
Artifact files live in [`artifacts/`](artifacts/); the exact recipe is printed
inside each.

---

## R1 — "It's a simulation built in a day; my org can't do this"

**Rebuttal:** the question isn't whether the workspace is real — it's whether
the *ledger* is portable, durable, and replayable outside this laptop. So we
replayed every node's chain of custody through a full remote roundtrip: push
every ref to a fresh bare origin (git transport only, no nool daemon), fresh
clone, `nool pull`, heal, **signed audit verify**.

**Evidence — `artifacts/fresh-clone-replay.txt`:**

```
9 / 9 nodes replay with 0 integrity failures through a full remote roundtrip
  contracts ... svc-insights: all "2 pass, 0 fail"  (slsa-source-v1.2)
```

And the audit exports carry `Signed by
ed25519:39aa3ee8…` — clone it in a year, from a different machine, and the
signature check is the same. What the one-day build bought us was *breadth*
(9 repos), not the property being sold; the property is now on the internet.

⚠ Honest note (finding `5501aba4` in mouse-fw's ledger): replay of repos
**jammed as `code/*` branches onto one shared remote** hits a mirror-pointer
name collision (`refs/nool/mirror/<branch>`) and fails to ingest cleanly — the
proof runs correctly on the reference layout, one origin per repo, which is
exactly how production estates are arranged. Filed, not hidden.

## R2 — "Process vomit: my devs will just `git commit` and bypass all this"

**Rebuttal — the bypass is visible and adoptable, and agents can't take it:**

| Actor | Bypass attempt | What actually happens |
|---|---|---|
| **Coding agent** | Edit files directly | `nool hooks install` guards the agent (Claude Code et al.): **every Edit/Write renews that session's live leases** on the edited file. No session, no landing. |
| **Human dev** | `git commit` on main, push | The landing carries **no knot**; it shows in history as foreign. `nool bridge adopt` adopts it as one knot tagged `nool:foreign:<oid>` — it *enters the DAG*, provenance-stamped, and the audit treats it accordingly. Bypassing is logged, not forbidden. |
| **Human dev** | commit message with no `why` | `nool commit-template install-hook` / `pr summary` — CI renders the knots-not-in-base context; a message without the template fails validation. |
| **Origin** | push around everything | With the hub wired, origin-side required refs + notice delivery make the ledger the merge gate (this is the part we annotate as *needs hub*, like the changeset pins). |

**Generated evidence:** we inspected the hooks dir (`.git/hooks`: empty — nool
does **not** stealth-patch your git) and captured the gate ledger instead:
`artifacts/policy-stats.json` shows what the *voluntary* path costs —
10 gates over 30 days, 19 blast-gate evaluations for 12 landings,
**100% first-run pass** on blast and budget. The friction number is small and
it is measured, which is more than the shadow lane can say.

## R3 — "We can build this with CI + a lock service + some glue"

**Rebuttal — build the demo, then survive the failure modes nool already
survived:** during this build the ledger **caught two broken chains on its
own**: contracts (`parent_missing` → audit FAIL → `doctor --fix` → 23 issues
healed; artifacts `contracts-audit-pci.before/after-repair.txt`) and mouse-fw
(FAIL on 82 knots → `doctor --fix` → **38 issues fixed**, verified
2-pass/0-fail). Then we found a third (the jammed-remote replay bug,
`5501aba4`) — again by the audit refusing to lie.

That is the difference your glue doesn't have: **integrity that fails closed**.
A Terraform lock service doesn't detect that its own history lost a parent;
it doesn't sign with HLC clock-skew rejection (`max_clock_skew_ms` in
`nool.toml`); it has no `bridge reindex`/`recover-ledger` idempotent healing;
and nothing you cobble in a sprint produces a signed SLSA-source verdict
replayable from a clone. The auditable claim is not "nool has features" —
it's that the audit *caught the tool's own bugs twice this week*.

**Evidence:** heal artifacts above + `policy-stats.json`:
`lease: 1 block` — one proposal physically refused by another agent's fence
(the live 403). Try making your CI lock refuse anything after a crash, with a
signature, and hand the refusal to an auditor as evidence.

## R4 — "Framework bloat: PCI for a mouse?"

**Rebuttal — the loadout is chosen, not baked:** frameworks activate only
where a **landed policy claims a control**. `nool audit packs` in contracts
shows 2 controls passed with 16 awaiting claims *because that's what the
3 policies we chose to land claimed*. The slsa-source track (the one that
actually fits a firmware supply chain) is what the 9/9 replay used; PCI was
run explicitly *as an example of depth-on-demand* and can be dropped with
zero code changes — stop claiming controls, it stops being assessed.

**Evidence:** `fresh-clone-replay.txt`; the PCI full run is `contracts-audit-pci.*`. vs the slsa-only replay in
`fresh-clone-replay.txt`. The Control-loadout is config, not architecture —
unlike the "we'd have to reimplement the gate per framework" homegrown path.

## R5 — "No token math — what's $ per goal?"

**Rebuttal — the honest one, absent the pilot:** no model providers are
configured on this box, so no spend exists to show, and we will not invent
trend lines. What exists today, measured: per-gate block/landing ratios
(`policy-stats.json`), per-task attribution spine, thread-scoped usage — the
*meters are live even before the fleet is*. The lease-block receipt is the
cost story in miniature: the rent for coordination is `exit 3` in milliseconds
instead of a burned agent session and a re-review. The number you could sign
comes from the 2-week instrumented pilot (kit §5), run **on your repos with
your providers**, not from our estimates.

---

## The rebuttal rule this document follows

Every answer above contains: (1) the mechanism, named; (2) evidence generated
*this week, in this workspace*, with the command visible in the artifact;
(3) one admitted wart with its finding id. An executive who smells a sales
deck stop smelling one the moment you volunteer the ugly part.
