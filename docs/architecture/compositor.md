# Compositor Architecture

## Wayland Backend Architecture (`libwayland`)

Halo DE does not hardcode its dependency on `wlroots` in the core window management logic. Instead, `libwayland` acts as a facade. Currently, this facade is implemented using `wlroots` to handle the heavy lifting of DRM/KMS, libinput, and Wayland protocols. 

The backend handles:
- Establishing the Wayland display.
- Starting the DRM backend and enumerating CRTCs/Connectors.
- Initializing the EGL/Vulkan context for rendering.

## Rendering Architecture (`librender`)

The rendering pipeline is designed for low latency and high throughput.

### Scene Graph
Rendering is driven by a Scene Graph. 
- **Nodes**: A node can be a window, a shadow, a blurred background rect, or a custom UI element.
- **Damage Tracking**: When a client commits a buffer with a damage region, the scene graph calculates the bounding box of the damage, intersects it with opaque regions above it, and passes the minimal necessary clipping rects to the renderer.

### Triple Buffering & explicit Sync
Halo DE supports explicit synchronization (via the linux-drm-syncobj-v1 protocol). The compositor does not block waiting for the GPU to finish rendering the client buffer. Instead, it accepts a timeline semaphore from the client and submits it to the DRM API.

### Shaders & Effects
Effects like blur, rounded corners, and shadows are rendered via single-pass fragment shaders where possible to reduce bandwidth.

## Input Subsystem Design (`libinput` wrapper)

Input is handled via a unified routing system:
1. `libinput` reports raw events (pointer motion, key press, touch).
2. The compositor applies constraints (e.g., pointer lock for games, relative pointer).
3. The event is mapped to the output geometry.
4. The Seat Manager determines focus and sends the `wl_keyboard` or `wl_pointer` events to the focused client surface.

### Keybinds
Keybinds are intercepted *before* they are sent to the Wayland client. The `libconfig` module populates a hash map of keybinds. When a key press matches a combination (e.g., `SUPER + ENTER`), the compositor executes the bound action internally.

## Window Management Strategy

Halo WM implements a hybrid Tiling and Floating model, similar to BSPWM or Hyprland.

- **Workspaces**: Dynamic. Workspaces only exist if they contain windows or are explicitly focused.
- **Tiling Engine**: Binary Space Partitioning (BSP) tree. A workspace has a root node. Splitting a window creates two child nodes.
- **Floating**: Windows can detach from the tree. They are stored in a separate `halo_list` per workspace, rendered *above* tiled windows.
- **Layers**: Strictly adheres to the `wlr-layer-shell` protocol. Panels are `LAYER_TOP`, wallpapers are `LAYER_BACKGROUND`.
