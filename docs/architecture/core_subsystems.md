# Core Subsystems Architecture

## Threading Model

Halo WM operates primarily on a **Single-Threaded Event Loop** model for the core compositor. This eliminates locking overhead and race conditions for the scene graph and Wayland object state.

However, specific intensive tasks are offloaded to worker threads:
1. **Main Thread (Event Loop)**: Handles Wayland socket polling, input events, damage tracking, and scheduling rendering.
2. **KMS/Render Thread (Optional per output)**: In advanced configurations, rendering and DRM page flips can be offloaded to a dedicated thread per GPU/Output to prevent blocking the main thread during VBLANK waits.
3. **Async I/O Threads**: Disk I/O (loading images, parsing large configs) and expensive IPC serialization are pushed to a thread pool via `libcore` async tasks.

## Memory Management Model

Memory allocation in a compositor must be extremely fast to avoid frame drops. Halo DE employs a tiered memory model:

1. **Arena Allocators**: For transient, per-frame data (e.g., render pass descriptors, temporary lists of damaged rects), an arena allocator is used. It resets at the end of the frame (VBLANK), providing `O(1)` allocation and preventing fragmentation.
2. **Object Pools**: High-frequency objects (Wayland client structs, surface structs) are allocated from fixed-size object pools initialized at startup.
3. **Explicit Ownership**: Every struct has a clear owner. We do not use garbage collection or reference counting in the core path. Objects are explicitly destroyed when their Wayland resource is destroyed.

Example of ownership: A `halo_surface` is owned by a `halo_client`. A `halo_view` (mapped window) holds a weak pointer to `halo_surface`. If the client disconnects, `halo_client` frees `halo_surface`, which triggers an event notifying the scene graph to unmap the `halo_view`.

## Event Loop Architecture

The event loop is powered by `wl_event_loop` (provided by `libwayland-server`).
It multiplexes:
- Wayland client sockets.
- DRM fd (for VBLANK events).
- libinput fd (for mouse/keyboard events).
- IPC sockets.
- Timer fds (for animations).

### Animation Engine & Event Loop
Animations are driven by a generic timeline system. A timer is NOT used for actual frame driving. Instead, the animation engine calculates interpolated values based on the time elapsed since the *last presentation timestamp* received from the DRM backend.

## IPC Architecture

To communicate between the compositor, shell, and services, Halo DE uses a custom IPC library (`libipc`).
- **Transport**: UNIX Domain Sockets (`SOCK_STREAM` or `SOCK_DGRAM`).
- **Protocol**: Binary, TLV (Type-Length-Value) encoded for speed. Avoids JSON overhead.
- **Capabilities**: Can pass file descriptors (FDs) via `SCM_RIGHTS`. This is crucial for sending DMA-BUFs from the compositor to the shell for background blurring, or sharing memory-mapped configuration files.

## Plugin System Design

Halo DE features a dynamically loaded plugin system (`dlopen`).

### Plugin ABI (`halo_plugin_v1`)
Plugins are compiled against a stable C ABI. 
- The compositor exposes a vtable of allowed functions (e.g., `halo_wm_add_keybind`, `halo_scene_add_node`).
- The plugin exports a single entry point: `bool halo_plugin_init(struct halo_plugin_api *api)`.

### Sandboxing & Security
Plugins run in the same memory space as the compositor, making them inherently unsafe if malicious. Therefore, plugins must be explicitly enabled by the user and are intended for trusted extensions. For untrusted code, external Wayland clients communicating via custom protocols are preferred over in-process plugins.
