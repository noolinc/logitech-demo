# mouse-fw

Logi Wheel wireless mouse (simulated Logitech MX-class device).

-   Superloop firmware for an NRF-class MCU: sensor sampling, button
    debounce, LED, radio TX of `LOGI` frames (protocol from
    `platform/mcu-protocol`, contracts from `platform/contracts`).
-   Sim build (Makefile default `test`, then `sim`) links the mocked
    `hal-core` sources so CI can exercise the same task code on host.

Depends on: `platform/hal-core` (strict), `platform/mcu-protocol` (strict),
`platform/contracts` (contract).  Not for direct edits without release:
changing `include/logi/mouse.h` must also record a bump in the milestone
timeline if the wire contract changes.

Nool notes:
-   ghost-run + integrity driver validate commits before landing.
-   `nool debug blast-radius` against `include/logi/mouse.h` maps consumers
    (tests, `motion.c`, `buttons.c`).
-   Try branches for firmware experiments: `nool try new exp-* --worktree`.
