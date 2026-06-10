# Changelog

## Unreleased — GSoC 2026 libcint Integration (Issue #229)

### Added

- **Build system**: Migrated from `setuptools` to `scikit-build-core` + CMake for C extension support
- **CMakeLists.txt**: Automatic download and compilation of `libcint` v6.1.2 via FetchContent
- **Platform detection**: Automatically uses `qcint` (AVX2/x86) or `libcint` (ARM/generic)
- **C extension**: `libcint_wrap.c` — Python/C API bindings for 1-electron and 2-electron integrals
- **Platform-aware loading**: `.so` (Linux), `.dylib` (macOS), `.dll` (Windows) support
- **CI/CD**: `cibuildwheel` workflow for automated wheel builds on Linux and macOS
- **Gradient integrals**: `overlap_gradient_integral`, `kinetic_gradient_integral`, `nuclear_gradient_integral`
- **3-center integrals**: `three_center_repulsion_integral`
- **Angular momentum fix**: Uses GBasis Python implementation (see Issue #149)
- **Moment integral fix**: Updated function naming for libcint v6 compatibility
- **Benchmark notebook**: `notebooks/tutorial/Benchmark_libcint.ipynb`
- **Tutorial notebook**: `notebooks/tutorial/Tutorial_libcint.ipynb`
- **Documentation**: `docs/libcint_integration.rst`

### Fixed

- Moment integral naming: `int1e_x/xx/zzz` → `int1e_r/rr/rrr` for libcint v6 compatibility
- Platform-aware shared library loading in `libcint.py`
