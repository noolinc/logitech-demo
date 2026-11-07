# Logi Wheel — simulated Logitech org under a Nool workspace

Research sandbox that models **Logitech International S.A.** (Swiss-founded,
Lausanne HQ; the "Logitech" name is from *logiciel*, French for software) as a
multi-repo software organization under the **Nool 7.25.0** semantic-agentic VCS
(Enterprise). Everything below ran live on 2026-11-07 unless marked *planned*.

> **Demoing this to Logitech?** Start with the
> [Solutions Engineer kit](docs/solutions-engineer/00-read-me-first.md) —
> a 20-minute demo script with real receipts, an audience playbook built from
> Logitech's public engineering statements, objection handling, and machine
> prep. This README remains the technical deep-dive.

> **Remote layout** (`https://github.com/noolinc/logitech-demo.git`):
> `main` = the workspace root (this README, the SE kit, `workspace.toml`, and
> the root ledger under `.nool/`); one branch per org member,
> `code/<repo-name>` (e.g. [`code/contracts`](https://github.com/noolinc/logitech-demo/tree/code/contracts),
> [`code/mouse-fw`](https://github.com/noolinc/logitech-demo/tree/code/mouse-fw)),
> holding each repo's tip including its own `.nool/` ledger and knot refs
> (`refs/nool/*` were pushed to `main` alongside it). The workspace root and
> the code branches are independent git histories on purpose — nool is one
> workspace, not one repo; each node governs itself and the root ledger
> stitches them together.

Logitech in one line: design-led, **software-enabled hardware** for work and
play — gaming (Logitech G), keyboards/mice (pointing devices), video
collaboration (Rally Bar family, MeetUp, Sight, Tap), webcams (Brio, C920),
tablet accessories, headsets, with machine-learning/AI features and cloud
device-management services (Sync, and an Essentials/Select enterprise-support
ecosystem). One reportable segment: **Peripherals**, B2C + B2B worldwide.

---

## 1. The simulated org (9 repos, 3 groups)

Real Logitech ships device firmware + cloud device-management for peripherals;
this sim covers mouse/keyboard/receiver peripherals wired to three cloud
microservices, the classic dependency shape (one contract repo, everything
consumes it):

```
platform/ ─ contracts        C wire-protocol header + JSON schema (dependency root)
        ├─ mcu-protocol      C packet framing/CRC8/TLV codec        (build←deps, contract→contracts)
        └─ hal-core          embedded C HAL (gpio/led/button/sensor/radio/battery)
devices/ ─ mouse-fw          C superloop firmware (dep: mcu-protocol, contracts)
        ├─ keyboard-fw       C matrix-scan firmware (dep: mcu-protocol + contracts)
        └─ receiver-fw       C 2.4GHz→USB HID receive firmware
cloud/  ─ svc-gateway        Go: device-event ingestion, TLV validation
        ├─ svc-ota           Go: deterministic cohort rollout
        └─ svc-insights      Go: room-analytics aggregation
```

Dependency edges (live in `workspace.toml`):

| from | on | kind |
|---|---|---|
| mouse-fw / keyboard-fw / receiver-fw | mcu-protocol | build |
| mcu-protocol | contracts | **contract** |
| svc-gateway | contracts | **contract** |
| hal-core | contracts | build |
| svc-ota | mouse-fw | release |
| svc-insights | svc-gateway | build |

Device-side C builds are *sim-buildable* (`-DLOGI_SIM`, mocked HAL sources) so
host CI can compile and test the same task code without hardware — the
standard embedded practice: same API, two targets.

## 2. How the whole org lives under Nool

- One `nool init --governed` per repo → signed ledger, identity key, starter
  policies (Team/Enterprise), architecture recovery. 9/9 repos governed.
- Workspace root run (repo_id logi-wheel-root) + `nool workspace init` discovered all 9
  projects and generated `workspace.toml`, pre-populated with
  language/release-driver overrides and the dependency edges above.
- `nool workspace status` shows the Org→Dept→Team→Project tree, per-node owner
  (inherited downward), branch state, and a rolled-up health verdict.
- `nool workspace build-order` words work: contracts → mcu-protocol → svc-gateway → hal-core → firmware → cloud (verified output above).
- The DAG is the source of truth in each repo; git is the transport mirror
  (Bifrost auto-commits on solidify — check `git log` in any repo).

## 3. Cross-repo change, end-to-end (the part we actually ran)

**Scenario:** *battery-health telemetry over the 2.4 GHz link* — new TLV 0x11
in the contract, emitted by mouse-fw, accepted by svc-gateway.

Nool's `workspace changeset` is built exactly for this:

```bash
nool workspace changeset open --intent "Battery-health telemetry: ... (TLV 0x11, additive to proto v2)" \
  --member contracts="extend logi/proto.h with TLV_BATTERY_HEALTH (0x11)+ schema note" \
  --member mouse-fw="emit periodic battery-health TLV from the mouse superloop" \
  --member svc-gateway="accept TLV 0x11 in ValidateTLVs + tests"
```

Result (live): `changeset:8da82312c254d981`, tracking task `04ed00a9` in the
root ledger, one member task per repo (`contracts:915432b0`,
`mouse-fw:d702c536`, `svc-gateway:7450e932`). Then the golden path per member
repo:

```bash
nool task list | nool task pick --id <id> -s | nool task start --id <id> -s
NOOL_AGENT_ID=agent-a nool announce intent -k <paths> --intent "..."   # fencing lease
nool discover conflicts <paths>        # exit 3 = someone else holds it
nool debug blast-radius <path>         # impact before the first edit
…make the edit… && make -s test        # (or go test ./...)
NOOL_AGENT_ID=agent-a nool propose --tag changeset:8da82312c254d981 --task <id> \
  --intent "..." --why "..." --test-note "which tests proved it" \
  --path <file>... --solidify          # ghost-run validates in isolation, then seals
nool task finish --id <id> --landed-knot <knot> --test-note "..." -s
```

Landed and verified live:

| repo | knot | evidence |
|---|---|---|
| contracts | `066348e7` | make test, additive 0x11, release **0.2.0** |
| mouse-fw | `8d5123f9` | make test + sim; 0x11 emitted every 5 ticks |
| svc-gateway | `c1b85ff0` | go vet+test green; test asserts 0x11 accepted |
| mcu-protocol | `5ba8d415`/`a4f0c256` | vendored, self-contained build, tests OK |
| keyboard-fw / receiver-fw | `33cb1110`/`e01fb91c` | vendored + extern-C-free headers |

The workspace tracked compatibility between the three members automatically
(`compatible-with:...` obligations pinned to the manifest; they discharge when
the groups sync over remotes — see §5).

**Workspace goal fan-out** (bottom lever of the same spine):

```bash
nool workspace goal --intent "Ship battery-health telemetry end-to-end" \
  --decompose "cloud=mirror TLV 0x11 in insights rollups" \
  --decompose "devices=document the 5-tick battery schedule"
```
Live result: epic created in **6 projects** with a task each (see
`nool task list --thread ship-battery-health-telemetry-end-to-end` in
receiver-fw).

## 4. Policy layer (Enterprise): what the gates actually caught

Recorded **live** during this exercise — these are the agents' guardrails:

1. **Leasing / fencing (announce intents).** `announce intent -k <paths>`
   grants an expiring exclusive lease (`grant_seq` re-checked at solidify).
   Live catch: a `propose` under a different agent id was refused with a
   coordination conflict against the very lease its colleague held — the fix
   is stable identities (`NOOL_AGENT_ID=agent-firmware-1`) per session
   (finding `2465e83d`).
2. **Task rails.** `nool task edit --id 915432b0 --rails fuzz/,Makefile,tests/`
   pinned those paths; any bound proposal touching them is refused at the
   seal (`TASK_RAILS`). File a rail amendment via a trusted attestation if
   needed.
3. **Contract flavor.** A `-k contract` knot: the gate asks for `--why`
   (require_why noted), tests and notes; plus Nool's workspace semantic
   layer flagged the `HTTP /v1/device-events` drift between contracts and
   svc-gateway in real time (advisory, exactly what a contract edge is for).
4. **Intent/diff overlap detector.** `propose` warns when the intent
   sentence doesn't describe the diff (caught two housekeeping landings at
   ~12–20%) — a forcing function for honest commit messages.
5. **Deferred-debt hygiene.** `nool validate --all` clears fast-mode debt
   before a release (ran green across all 9 repos).
6. **Ghost-run, repo-isolated.** Integrity driver builds HEAD+your paths in a
   private worktree — *sibling repo includes do not exist there*. Live catch:
   `-I../contracts` builds passed locally but failed reification. Policy
   decided and recorded as an assertion: **platform/firmware repos vendor
   shared sources** (`vendor/ + refresh-vendor.sh`), or pin a monorepo
   checkout via `workspace.toml` edges. `nool learn` recorded the
   dependency-insight (`05c5f78d`).
7. **C-parser contract.** Nool's tree-sitter C adapter rejects
   `#ifdef __cplusplus + extern "C"` in the same header; a policy decision
   made contracts firmware headers **extern-C-free** (relocatable finding
   `f1feacce`, assertion knot `94195abf`), plus in mcu-protocol / hal-core /
   keyboard-fw / receiver-fw headers.
8. **Mutation testing on seal.** Full-mode seals run mutants of the changed
   lines against the selected test set; seals record
   `tests_constrain_change` attestations.
9. **Ship ritual.** Per member repo: `nool validate --all` → `nool doctor
   --strict` → `nool checkpoint 0.2.0` release knots (verified: contracts
   B/85, mouse-fw A/95, svc-gateway A/95, then workspace rollup
   RELEASABLE_WITH_WARNINGS across all 9).

## 5. What needs remotes / hub (partially wired — see gap notes)

The workspace is now mirrored to `https://github.com/noolinc/logitech-demo.git`
(root → `main`, each child → `code/<name>`), but flow discharge still needs the
hub:

- **Notices** (`notice list|reply|process`, auto-task on contract changes)
  ride push/pull or the hub; once each repo syncs to `origin`, contract-type
  lands (contracts pushing its `066348e7`) arrive in mcu-protocol/svc-gateway
  with lint-pin integration — then `notice process` offers to run auto-fix
  sessions (`on_notice = auto_fix`).
- **Manifest pins & obligations** in the changeset (`compatible-with: missing`
  in `artifacts/changeset-show.txt`) did **not** discharge from remotes alone —
  they need notice delivery via the hub (`nool workspace sync --once` against
  an estate where every node's origin is live).
- **Hub org-rollup functions** (`workspace status --hub`, `query search --scope org`,
  approval routes) read through `nool-hub-server` when OAC/OIDC wiring exists
  (Team/Enterprise).
- **Attestations for CI** (`nool attest record --from-head` with
  NOOL_IDENTITY_KEY) — the changeset's `Attested` state came from the mutation
  attestations locally; in CI add runtime evidence storms instead.
- **Fleets** (`nool fleet plan --thread <epic>`, `fleet code --pool …`): the
  epic is already partitionable — tasks have paths, dependencies and
  acceptance criteria, so `fleet run` would produce NodeID-disjoint waves;
  decided not to burn model budget in this sandbox, keep the commands in §6.

## 6. Agent runbook (the whole CLI, task by task)

Orientation (cheap, always first)
```bash
nool status --compact && nool outline <file> && nool show <file>::<Symbol>
nool context "<task>" --budget 2500 --cite      # matching entities+blast+findings
nool ground "<routing question>"                # + causal chain of the last knot
nool workspace search "<keywords>"              # org-wide, per-project index
nool discover shortcuts                         # explicit shortcut markers
```

Independent parallel work
```bash
nool fleet advise                                # Single/Parallel/CentralizedQuorum
nool fleet plan --thread <epic> [--isolation worktree]
nool fleet start --builder b.yaml --reviewer r.yaml --isolation worktree --nodes <paths>
nool workspace changeset open ...                # multi-repo, ledgers pinned
```

Ad-hoc sabotage/experiments that must not land
```bash
nool try new perf-1 --worktree --nodes src/      # lease + isolated checkout atomically
nool try impact perf-1 && nool try rebase perf-1 && nool try promote perf-1
```

Learning + handoff
```bash
nool learn --about "driver" --content "..."      # signed finding, replicated
nool findings digest --scope '<topic>'
nool thread handoff --name <epic> --to <key> -s  # offer to another agent
nool msg send --to agent-a "..." && nool msg inbox # local bus for overlap
```

Policy/audit plumbing (policy owners)
```bash
nool config init-governance --write
nool admin team add --id <key> --role editor --scope "devices/**"
nool attest record --checker github-actions --assertion e2e-tests --verdict pass --push origin
nool audit verify --since 30d; nool audit export --framework SOC2
nool policy stats --since 30d
```

Release-safe deployment pipeline (the honest short line)
```
work -> task rails honored -> propose --solidify (ghost-run) -> validate --all
     -> doctor --strict (blocks with exits 2) -> workspace verify/changeset show
     -> checkin 0.2.0 --channel prod -> push -> nool audit verify for the auditors
```

Blocking / breaking the rules, visible:
- gate says `exit 2` → fix content/configuration or route around; retry-or-relo
  identical changes → `nool steer --cascade` for parents/children approvals;
- `exit 3` → back off/lease-wait; `exit 4` → higher edition needed — in this
  workspace, the Enterprise plan is active and sessions use `--pool`/`session
  start` for non-colliding identity.

## 7. Worth adding in production

- `[attestation.obligations]` per path: e.g. `devices/**` requires `cold-run`
  and `svc-*` requires appsec-scan before push.
- `[publication].destination` grants for release channel gating (internal
  signing key pinned per repo).
- `nool audit packs` — align each firmware release's audit with the factory
  config baseline (see wiki).

## 8. Nool CLI v7.25.0 highlights exercised here

`changeset open/show` (6-tier 7.25 feature for multi-repo coordination),
`workspace goal/goals/status/verify/visualize`, `task` with `--rails`,
`docs --json` set output, intents with `--target-nodes` (+releases), training
budget skill plans, phantically transparent metadata, remote-signed `manifest`
pins, `verify-bundle` for offline evidence (SLSA/in-toto/DSSE), break-glass
(`--force` needs `admin`), `code` sessions with `--exec`+`--fleet` pools, and
submission with `policy stats`, `usage`, `session` for cost analysis. Quitl:

```bash
nool guide multi-agent  # fleets, reviewer pools
nool dag && nool ui     # DAG viewers
```

---

**Bottom line (observed, not asserted):** the sim covers the exact pain that
Nool formalizes: shared contract deps, firmware+cloud teams, dozens of agent
contributions. The governed path made each cross-repo change self-describing
(intent, tests, cited knots), locally provable before sealing, and
workspace-sum-able (org tree, dependency-ordered landings, rollup doctor).
The commands live in `workspace.toml` + `.nool/` in each repo; git mirrors are
in each `.git` for whichever `origin` the team wires up next.
