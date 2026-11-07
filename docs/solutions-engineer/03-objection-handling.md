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

**Delivery rule:** every objection gets a receipt (knot id, demo minute, or
artifact file) inside 60 seconds, then one question back. Objection-handling
without return-fire becomes a debate instead of a discovery.
