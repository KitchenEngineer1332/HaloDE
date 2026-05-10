# Halo DE

Halo DE is a modern, production-grade Linux desktop environment built on Wayland. It is designed to be highly modular, scalable, secure, and optimized. The project is primarily written in C17, avoiding C++ where possible, ensuring a clean, stable ABI and a lightweight core.

## Features

- **Wayland Native:** Built on a custom backend abstraction (initially wrapping wlroots).
- **High Performance:** OpenGL/Vulkan rendering, damage tracking, explicit sync, DMA-BUF, and triple buffering.
- **Secure by Design:** Sandbox-aware, secure IPC, permission management, and plugin sandboxing.
- **Highly Modular:** IPC-driven architecture with stable plugin APIs.
- **Advanced UI/UX:** Theming engine, animation engine, blur, shadows, and rounded corners.

## Components

- **Halo WM:** The core Wayland compositor and window manager (Tiling + Floating).
- **libs/**: Core system libraries (`libcore`, `libwayland`, `librender`, `libipc`, `libplugin`, etc.).
- **shell/**: Desktop shell components (panels, app launchers, dashboard).
- **session/** & **services/**: Session management, power, network, and audio daemons.

## Building

Halo DE supports both Meson and CMake, though Meson is the primary recommended build system for Wayland-native projects.

### Dependencies
- Wayland, wayland-protocols
- wlroots (>= 0.17)
- libinput, libxkbcommon
- OpenGL (EGL, GLESv2) / Vulkan
- pixman, libdrm, libgbm
- systemd / elogind
- dbus

### Using Meson
```bash
meson setup build/release --buildtype=release
ninja -C build/release
```

## Documentation

Comprehensive architecture documentation can be found in the `docs/` directory.

- [Architecture Overview](docs/architecture/overview.md)
- [Developer Guide](docs/developer-guide/README.md)
- [API Reference](docs/api/README.md)

## License
MIT (or GPL-3.0, TBD)