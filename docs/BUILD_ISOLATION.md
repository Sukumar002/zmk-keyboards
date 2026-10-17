# Board-isolated firmware builds

This repository keeps shared ZMK infrastructure in one place while building each keyboard independently.

## Hana
- Matrix: `build-hana.yaml`
- Workflow: `.github/workflows/build.yml`
- Board files: `boards/shields/hana/**`

## Chocofi
- Matrix: `build-chocofi.yaml`
- Workflow: `.github/workflows/chocofi.yml`
- Board files: `boards/shields/chocofi/**`

Board-specific commits only trigger that board's workflow. Changes to shared firmware infrastructure such as `config/**`, `src/**`, `snippets/**`, `dts/**`, or module/Kconfig files intentionally trigger both so shared compatibility is verified.

The original `build.yaml` is retained temporarily as a compatibility/reference file; active workflows use the per-board matrices.
