# 02 — Audience Playbook (Logitech-specific)

Built from public sources: the Stack Overflow podcast with Laurent Gillet
(head of embedded software) and Tanvi Shah (head of software development),
the PC Gamer interview with senior director Maxime Marini, the MX Keypad
blog (developer-tools PM), 2026 job postings, and github.com/Logitech.

Use these personas to pick which 5 minutes of the demo you spend on whom.

## 1. Embedded software leadership (the "Gillet" persona)

**What they've said publicly:** firmware is C/C++/asm; 1 ms wireless protocol
"as good as corded"; waterfall toward a hard mass-production date, fixed
scope; half the team is EE and sits with oscilloscopes; firmware must be
complete when the device ships — "you can't say, let's ship right-click later."

**Their fear:** quality at the factory date, and any tooling that turns
agents into cowboys on factory-bound code.

**Your opening:** "This isn't 'move fast and break things.' It's enforce the
constraints you already believe in — fixed scope becomes task rails, the hard
date becomes doctor --strict and a checkpoint — so agent throughput doesn't
soften your process."

**Demo minutes that matter:** §4 (task rails enforced), §5 (the two live
catches), §6 (doctor/audit). De-emphasize the cloud half.

## 2. Software/platform leadership (the "Shah" persona)

**What they've said publicly:** Agile Release Trains, CI/CD, continuous
delivery, "Hybrid Agile" for combined firmware+software experiences.

**Their fear:** shipping faster without re-platforming everything; also the
review bottleneck when PR volume becomes agent-shaped.

**Your opening:** "You keep the agile cadence. What changes is that review
attention gets spent only on work the machines already fenced, tested, and
couldn't game — and the cross-repo coordination you do in standups becomes a
changeset object with pins instead."

**Demo minutes:** §5–6 (changeset), blast radius §3, workspace goal spine §7.

## 3. Firmware managers / NPI program leads (the job-post persona)

The 2026 postings describe EVT/DVT/PVT gates, cross-functional coordination
with EE/QA/suppliers, and — notably — **required "expertise in using AI tools
and creating tools using AI."**

**What they actually need:** status that doesn't rot, scope that doesn't
drift, evidence that EVT/DVT builds contain only what program management
approved.

**Your opening:** "Your program gates become commands: `doctor --strict`,
`checkpoint X.Y.Z`, task-bound landings — and the burn-down your execs ask
for is `nool workspace goal-status`, computed."

## 4. Quality / compliance / release engineering

**Context you can cite:** they already publish firmware artifacts to the
Linux Vendor Firmware Service (`github.com/Logitech/fw_updates`, co-maintained
with fwupd's Richard Hughes), and the Select-tier enterprise ecosystem sells
SLAs. Both need reproducible release evidence.

**Your opening:** "Every landing is signed (ed25519), gate receipts,
attestations, pinch-quotable in one export: `nool audit verify --framework
sox-itgc` or SLSA source — replayable from a fresh clone of the mirror."

**Ask them:** "Who assembles that evidence today, and how long does it take?"
(The changelog from contracts is the live answer: receipts happen per-landing,
not per-audit-season.)

## 5. Developer-tools / partnerships (MX Keypad persona)

The MX Keypad launch was explicitly about an agent-and-plugin ecosystem —
2,500 developers, marketplaces, "AI-assisted documentation." Their public
protocol (cpg-docs) feeds hundreds of third-party tools (logiops et al.) that
survive on reverse engineering.

**Your opening on the community angle:** "One semantic contract repo mirror
per product family; a change checked against every dependent is `doctor`
output. Your external tools ecosystem stops discovering protocol drift the
hard way — through bug reports."

## Meeting-structure suggestion

Don't spread evenly. Get leadership buy-in on the **three claims**
(`00-read-me-first.md`), then give the firmware bench a hands-on: the rest of
the kit is exactly this workspace — let them run `nool task list`,
`nool show`, and the fencing collision themselves. The close is never a
slide; it's their question from §6 of the demo script: "how many people
tracked the ripple on your last protocol change?"
