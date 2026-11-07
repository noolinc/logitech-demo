# 04 — Environment Prep (run the morning of the demo)

**Machine:** the workspace lives at
`/Users/arunbalakrishnan/Research/logitech` with 9 governed repos and a
workspace root ledger. All commands are local; the Enterprise lease was
activated (`nool admin account billing status` → ENTERPRISE, expiry 2027).
Run this checklist from the workspace root.

## T-60 minutes

```bash
cd /Users/arunbalakrishnan/Research/logitech
# 1. Edition alive?
nool admin account billing status
# 2. Test matrix (all green expected — was verified 2026-11-07)
for r in platform/contracts platform/mcu-protocol platform/hal-core \
         devices/mouse-fw devices/keyboard-fw devices/receiver-fw; do
  (cd "$r" && make -s test)
done
for s in svc-gateway svc-ota svc-insights; do
  (cd "cloud/$s" && go test ./...)
done
# 3. Ledger health rollup
nool workspace doctor --recursive --compact
```

Expected: all greens; workspace verdict RELEASABLE_WITH_WARNINGS or better;
audit in contracts reads INCONCLUSIVE (0 fails) after the 2026-11-07 ledger
repair — see artifacts `contracts-audit-pci.before/after-repair.txt` for the
story if asked ("the ledger heals itself; here's the receipt").

## T-30: key hygiene

```bash
nool status --compact            # in each of the 3 demo repos: no pending candidates
nool task list --active          # pre-existing tasks stay; never cancel on stage
nool admin team list             # your key's grants visible, scope devices/**, cloud/**
```

Identity for the live collision demo: always set
`NOOL_AGENT_ID=se-demo-A` / `se-demo-B` in the two panes; the earlier
confusion with key-level identity (agent-firmware-1) is exactly the story
from §5 of the demo script — keep the cheeky version out of the room.

## T-15: the pre-flight you actually rehearse

```bash
# the ONLY live-collision commands used on stage (non-destructive):
# pane A
NOOL_AGENT_ID=se-demo-A nool announce intent -k src/motion.c --intent "hold"   # in mouse-fw
# pane B
NOOL_AGENT_ID=se-demo-B nool propose --path src/motion.c --intent "collide" --solidify
# expect exit 3 + "Coordination conflict" — then release:
NOOL_AGENT_ID=se-demo-A nool announce release <id from status>
```

## Known gaps (announce them BEFORE they surface, see §5 of demo script)

1. **No git remotes.** Notice delivery, changeset compatibility-pin discharge,
   and hub rollups want per-repo `origin`s or the hub. Mention once: "in your
   estate every repo already has origin; the workspace syncs in dependency
   order (`nool workspace sync --once`)."
2. **No model providers configured** for `nool code`/fleet runs on this
   machine — that segment is intentionally replayed, not live (kit §5 metrics
   covers the numbers instead).
3. **Knot ids referenced in the script are from this build**. If the workspace
   was rebuilt, regenerate `artifacts/` (commands are one-liners in each file)
   — never quote stale ids on stage.

## Fallback plan (network dies, machine dies)

Every live command has its output cast in `artifacts/` — same terminal
emulator, `less -R artifacts/<file>.txt`. The narrative never depends on a
keystroke; only the collision demo does, and it has a swap-out.
