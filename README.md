# contracts

Wire-protocol contracts shared by every Logitech firmware MCU repo and the
cloud microservices (research-simulated Logitech org).

- `include/logi/proto.h` — C header: frame layout, TLV ids, thresholds.
- `schemas/device-event.schema.json` — cloud-facing JSON schema.

This is a **dependency root**: `mcu-protocol`, `hal-core` consumers,
all three firmware repos, and `svc-gateway` mirror these constants.
A change here is a workspace-wide event: the Firmware/Hardware group and the
cloud services depend on it, so a `contract` flavor landing must carry:
breaking-change notes, version bump policy, and a notice round-trip.

Policy (enforced by Nool, Aram + obligations):
- Do not change the pipeline rules mid-task: `policy:contracts-only`.
- Cold-start batch mode for CI verification on the platform group.
- Two-eyes for cross-repo changes; `nool attest` records rebase-proof.
