# Halo DE Developer Guide

This directory contains crucial information for developers contributing to Halo DE.

## Build Strategy

Halo DE uses **Meson** as its primary build system, as it integrates flawlessly with the Wayland ecosystem (wlroots, wayland-scanner, systemd). CMake is maintained as a secondary option for specific packaging scenarios.

- **Out-of-tree builds**: Always build in a separate directory (`build/debug` or `build/release`).
- **Wayland Protocols**: The build system automatically invokes `wayland-scanner` to generate C headers from XML protocols located in `protocols/`.

## Testing Strategy

- **Unit Tests**: Located in `testing/unit/`. We use `cmocka` for C unit testing. Every data structure in `libcore` must have 100% coverage.
- **Integration Tests**: We use a headless Wayland backend (wlroots headless backend) to spin up a virtual compositor, connect a virtual client, and verify scene graph mutations.
- **Fuzzing**: `libipc` and protocol handlers are fuzzed using `AFL++` and `libFuzzer` to ensure malformed data does not crash the compositor.

## CI/CD Pipeline Design

GitHub Actions is used for CI/CD.
1. **Linting**: Every PR triggers `clang-format` and `clang-tidy`.
2. **Build Matrix**: Builds are tested on Alpine (musl), Arch (gcc), and Ubuntu (clang).
3. **Leak Checking**: Tests are run under AddressSanitizer (ASan) and UndefinedBehaviorSanitizer (UBSan).

## Packaging Strategy

- **Arch Linux**: Maintained in AUR initially (`halo-de-git`), aiming for `[community]`.
- **Debian/Ubuntu**: PPA providing latest builds.
- **NixOS**: Flake available in the root repository.
- **Flatpak**: The shell and core apps are distributed via Flatpak, utilizing portals for system access.

## Git Workflow Strategy

- **Branching**: `main` is the unstable development branch. Release branches are cut as `release/1.x`.
- **Commits**: Follow Conventional Commits (`feat:`, `fix:`, `refactor:`).
- **PRs**: Require 1 approving review from a core maintainer and passing CI.

## Coding Standards

- **C17 Standard**: Pure C.
- **Naming**: `snake_case` for variables/functions. `HALO_UPPER_SNAKE` for macros.
- **Opaque Pointers**: Header files should expose typedefs (e.g., `typedef struct halo_surface halo_surface_t;`) and keep the actual struct definition in the `.c` file to maintain a stable ABI.
- **Error Handling**: Functions return `int` (0 on success, negative error code on failure) or `bool`. Do not use `goto` except for cleanup blocks at the end of a function.

## Release Engineering Plan

Semantic Versioning (SemVer) is strictly enforced.
- **Alpha**: Internal API is volatile.
- **Beta**: Feature freeze. Focus on bug fixes.
- **Stable (1.0)**: Plugin ABI and custom Wayland protocols are frozen. Any breaking change bumps the major version.

## Roadmap from Alpha → Stable

1. **v0.1.0 (Alpha)**: Core compositor runs, can launch Wayland clients, basic tiling.
2. **v0.5.0 (Beta)**: XWayland support, theming engine, basic shell (panel).
3. **v0.9.0 (RC)**: Stable IPC, plugin system, security sandboxing complete.
4. **v1.0.0 (Stable)**: Full release, robust multi-monitor support, complete documentation.
