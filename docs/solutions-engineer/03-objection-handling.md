# 03 — Objection Handling

Every objection below came up either publicly (Logitech sources) or live in
this workspace. Prepared answer first, receipt within the kit second.

## "We already have git flow / GitLab"

**Answer:** "Git flow governs branches. It has no object for intent, no
contention primitive before merge, no multi-repo change, and it assumes
review throughput scales with PR creation. Keep your GitLab — Nool mirrors to
git; your repos stay clones. What you gain is the layer git never had: fences
before work starts, proofs before land, signatures after."

**If pressed on review workflow:** "Your PR review doesn't disappear; it
moves. `propose` seals what already passed validation, so the PR becomes the
*last* mile, not the first. That's how review survives 100x PR volume
without becoming rubber stamps."

## "Your demo compiles with host clang; we build with STM32/NRF toolchains"

**Answer:** "Correct, and the workspace is honest about it — see the
'integrity driver' concept: the point is the *contract*, and drivers are
pluggable. Wire your cross-compilers and unit runners ([code] checks) and the
gate stack is unchanged. What we demoed today is the governance web, not the
oscilloscope."

**If they raise HIL/bench tests:** "Those stay manual and SHOULD — nool
never claims to flash hardware. But `nool attest record --checker
bench-lab` — signed verdicts from your rack — closes the loop between the
ledger and the bench."

## "The C parser false-positived on extern C — that's a bug, why glamorize it?"

**Answer (give the finding id, don't dodge):** "Yes — finding f1feacce,
assertion 94195abf. Two things matter more than the bug itself: you can
*decide* org policy a tool can't parse and record it durably (that's what we
did — headers went extern-C-free, recorded, revocable), and the same gate
caught it before it ever reached CI. We'd rather show you a real wart with a
recovery path than a spotless toy."

## "Single agent + reviewer is fine; why fleets?"

**Answer:** "Because the cost appears the day you run two. In this workspace
a single propose was refused because ANOTHER identity held the lease — even
the same human machine needs identity plumbing, and git has no primitive for
it at all. The moment Logitech runs parallel NPI workstreams (every job post
describes multi-track NPIs), contention becomes daily, and 'merge at the end'
is where the damage is found."

## "Token costs — how is this not runaway spend?"

**Answer:** "Three controls, all shipped here: per-task attribution
(`nool usage thread`), workforce budgets (`fleet capacity` binds on
`budget_usd_per_day` vs CPU vs memory, and tells you WHICH bound), and
playbooks so runs are reproducible (`nool code --playbook bug-fix`). The
number for your CFO: spend per verified goal, promoted-rate trend —
`nool code metrics`."

## "Where do markdown-style agent graphs / shared scratchpads fit?"

**Answer:** "They're the meeting, not the ledger. Useful for planning;
useless for contention — two workers reach for the same file and the
markdown never notices. In nool the plan can still BE markdown (playbooks,
personas, thread chat), but the *effect* of the plan is enforced:
promises become leases, rails and gate receipts."

## "What does this cost in process overhead?"

**Answer:** "Roughly the overhead of your current hybrid model, but made
 executable and portable. The dial is validation depth per landing: `--fast`
 when you trust the change, full-mode when you don't, with debt tracked by
 `validate --all`. Defaults are warn-first (we lived that in the demo),
 block-second once the team is confident."

## "Security: a signed ledger is one key away from abuse"

**Answer with their world:** "Grants are scoped keys, not shared passwords:
`nool admin team add --role editor --scope devices/**`. Device slots are
metered; compromised keys die on `admin revoke`. There's a break-glass path,
but it's one-ref, short-lived, admin-signed, and shows in `audit verify`.
'Who could land what when a leaked key existed' is a five-minute question in
front of your auditor instead of a week of archaeology."

---

## The 3-minute rebuttal round (verbal rehearsal cut)

For when the executive pushback arrives all at once — the full evidence for
every beat is generated and on file in
[`06-exec-rebuttals-evidence.md`](06-exec-rebuttals-evidence.md). Spoken
language, ~450 words, each beat timed.

**[0:00 — setup, 15 s]**
"You asked what happens if someone just works around this. Fair. During this
build, that exact question got answered three different ways, and I'd rather
show you the receipts than argue the theory."

**[0:15 — 'they'll git commit around it', 45 s]**
"Say a dev types `git commit` on main, pushes, bypasses everything. Nothing
blocks their keystroke — we don't stealth-patch your git. What happens is
that landing is *history without a signature*. One command — `bridge adopt` —
and it enters the ledger tagged `nool:foreign`, so when you audit later, the
question 'who landed around the process?' is answered by the DAG itself,
permanently. And for agents, that lane doesn't exist: the hook renews a live
fence on every file the agent touches. No session, no landing, no debate."

**[1:00 — 'we could build this ourselves', 45 s]**
"You could build the demo. Here's what you'd be signing up to replicate —
this week, this tool's audit **refused its own history**. Twice. Contracts
had a broken chain: the export said FAIL, the fix took one command,
twenty-three issues healed. Mouse firmware: eighty-two knots checked,
thirty-eight fixed on the spot. A lock service you code in a sprint won't
fail closed on its own integrity — it will happily keep serving a corrupted
history. The product's most embarrassing moments are its best reference."

**[1:45 — 'PCI for a mouse?', 30 s]**
"The frameworks aren't baked in — they're a loadout. We run SLSA source
because that's what fits a firmware supply chain, and the only controls that
get assessed are the ones a landed policy claims. Drop the claim, the control
leaves the audit. It's config, not architecture."

**[2:15 — 'it's a demo toy', 30 s]**
"On portability: every node in this workspace replayed its full chain of
custody through a round trip — push, fresh clone, pull, verify — nine out of
nine, zero integrity failures, all signed. Your evidence is reproducible from
a clone, on someone else's machine, a year from now. That's not a laptop demo
anymore."
*⚠ If pressed on the caveat: "One honest wart — that result needs one origin
per repo, which is how production runs. Jamming nine repos as branches onto
one remote trips a bug we've filed. Finding 5501aba4, receipt in
`artifacts/fresh-clone-replay.txt`."*

**[2:45 — 'token math', 15 s]**
"And on spend: no providers, no fake numbers. What's live is the refusal
meter — one proposal physically stopped by another agent's fence.
Milliseconds, zero tokens burned, receipt in the ledger. The trend comes from
your pilot, not my slides."

**[3:00 — the close]**
"That's the rebuttal round. Now the question back — the one from the start:
your last cross-repo protocol change. Who tracked the ripple, and how long
until they were sure?"
*…and be quiet.*

**Three moments sell this script and none of them are features** — the audit
refusing to lie (twice), the bypass being tagged instead of blocked, and the
silence after your question. If you only land one, land the audit story; it's
the only one where the product is the hero without you defending it.

---

## Annex — Incumbent landscape & the composition posture

For the hardest version of "why buy this": aren't GitHub, GitLab, Sigstore,
OPA, Drata, Polarion already selling pieces of this? They are. Lead by
conceding that out loud — then show every incumbent holds **one primitive**,
and the gaps between them are exactly where agent-era failures land.

### The primitive map

| Primitive | Incumbent that owns it | What's left uncovered |
|---|---|---|
| Signed append-only log (artifact provenance) | Sigstore Rekor / in-toto / GitHub attestations | Build artifacts only — no intent, identity, leases, or "why"; never paces a work force |
| Policy-as-code refuse-at-push | OPA / Gatekeeper / Sentinel · GitLab compliance pipelines | Evaluates ON the PR, after work; single-repo worldview |
| Contention control (after work) | GitHub/GitLab merge queues; Trunk | Serialization happens at merge; the work is already spent |
| Contention control (before work) | Perforce exclusive checkouts (decades old) | The industry walked away because human rebase cost was cheap; with agents the cost is a burned session — the trade inverted, and the incumbent slept on it |
| Cross-repo change object | — none — | Monorepos dodge it; Jira epics are to-do lists, not landing state machines with pins |
| Process evidence / RTM | Polarion / DOORS / Jama | System of record upstream; documentary, retrospective, human-curated — nothing at the change edge |
| Continuous compliance evidence | Drata / Vanta | Collects from existing tool logs; assumes change records already exist |
| Agent guardrails | Claude Code hooks · Copilot Control System · sandboxes (E2B…) | Per-tool, per-sandbox; identity of the TOOL, not the actor; no cross-tool ledger or spend attribution |

### Concessions to volunteer (say these before they do)

- "If your org is human-paced and single-repo, don't buy this — merge queues,
  branch protection, and a compliance subscription already close the loop."
- "Sigstore proved the model; the ledger applies it to the change layer."
- "Perforce solved checkout locking decades ago — and it under-served the
  market because human rebase was cheap. Agents changed the economics."

### The composition posture

Nool replaces none of it:

| Layer | Owner | Unchanged? |
|---|---|---|
| Requirements system of record | Polarion / DOORS / Jama | upstream, untouched |
| Certified analyzers / scanners | Polyspace · QAC · SAST | the instrument layer, untouched |
| Provenance / transparency logs | Sigstore Rekor | can mirror or coexist |
| Policy-as-code at PR | OPA | still runs; nool adds the pre-work fence |
| The connective tissue between them | **Nool** | actor identity, cross-repo leases, intent binding, per-actor attestations, spend attribution, pin discharge |

Positioning line: "We don't replace your CI, your Sigstore, or your Polarion.
We're the layer between the actor that starts work and the system of record
that swallows it. Every incumbent above answers 'did this artifact pass?' We
answer 'who did this, to what purpose, under what fence, at what cost — and
can you prove it in two years?'"

### The question that ends the debate

"**How does any of your incumbent tooling know that the Claude session editing
`proto.h` is the same actor as the commit authored into main?**"

Answer everywhere except nool: it doesn't. Incumbents trust merge-time
identity; agent workflows break that trust (shared machines, keys, service
accounts). The hole never existed in their stack until now — which is why the
layer doesn't exist as a product yet. Deliver this question last, quietly,
and let it sit on the table.

---

**Delivery rule:** every objection gets a receipt (knot id, demo minute, or
artifact file) inside 60 seconds, then one question back. Objection-handling
without return-fire becomes a debate instead of a discovery.
