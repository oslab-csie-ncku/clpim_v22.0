# vmlinux Binaries (gem5 / QEMU)

This directory stores multiple `vmlinux` kernel binaries built from
Linux **4.18.0 + NOVA filesystem**, used for **gem5 simulation** and
**QEMU execution** under different PIM / DPU / copy-path configurations.

All kernels are based on the same baseline source tree, with variations
in data-movement paths and synchronization logic.

---

## Naming Convention
x86_64-vmlinux-4.18.0-nova-pohao-baseline-<STACK>-<MODE>-<PLATFORM>

### Fields

- **STACK**
  - `1`, `4`, ... : Number of PIM stacks / zones enabled

- **MODE**
  - `baseline` : No special copy optimization
  - `CrDw`     : CPU read → DPU write
  - `DrCw`     : DPU read → CPU write
  - `DrDw`     : DPU read → DPU write
  - `DrDwDPUtaskwaitall` : DrDw with `nova_flush_buffer` in `dpu_task_wait_all()` enabled
  - `_v7`, `_v8`, `_v9`  : Different internal revisions / experiments

- **PLATFORM**
  - `gem5` : Built for gem5 full-system simulation
  - `qemu` : Built for QEMU execution

---

## Kernel List

### Baseline Kernels

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-1-gem5`  
  Baseline kernel for gem5, no PIM-specific optimizations.

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-4-gem5`  
  Baseline kernel with 4 PIM stacks enabled.

---

### DPU / PIM Data-Movement Variants (gem5)

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-1-CrDw-gem5`  
  CPU-read → DPU-write path.

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-1-DrCw_v8-gem5`  
  DPU-read → CPU-write path (revision v8).

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-1-DrDw_v7-gem5`  
  DPU-read → DPU-write path (revision v7).

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-1-DrDw_v9-gem5`  
  DPU-read → DPU-write path (revision v9).

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-1-DrDwDPUtaskwaitall-gem5`  
  DrDw variant with explicit `dpu_task_wait_all()` synchronization. => Actually, it didn't help, delete it later.

---

### QEMU Kernel

- `x86_64-vmlinux-4.18.0-nova-pohao-baseline-1-qemu`  
  Kernel built for QEMU (not gem5).  
  Used mainly for functional debugging and faster iteration.
  - All cpu read/write.

---

## Notes

- All `gem5` kernels must match the **memmap configuration** passed via
  kernel command line (e.g. `memmap=16G!17G`).
- These kernels are **not interchangeable** between gem5 and QEMU.
- Revision suffixes (`_v7`, `_v8`, `_v9`) indicate experimental changes
  in copy path, cache flushing, or synchronization logic.

---

## Recommended Usage

- Use **baseline** kernels for correctness comparison.
- Use **DrDw / CrDw / DrCw** variants when evaluating PIM data paths.
- Prefer **QEMU kernel** for fast debugging, **gem5 kernel** for timing
  and memory-system analysis.
