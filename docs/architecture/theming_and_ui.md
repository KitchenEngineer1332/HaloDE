# Theming, UI, and Configuration

## Theme Engine Design (`libtheme`)

Halo DE separates logic from presentation entirely.

### unified styling
The theme engine applies styles not only to the internal shell (panels, dashboard) but exports variables for GTK, Qt, and flatpaks. 
A single `theme.conf` file controls the entire desktop color palette. The daemon translates this into:
- XResources (for XWayland apps)
- `~/.config/gtk-3.0/settings.ini`
- `~/.config/gtk-4.0/gtk.css`
- Qt style sheets.
- `xdg-desktop-portal` color scheme preferences (Dark/Light mode).

### Native UI Toolkit (`libui`)
To avoid bringing in massive dependencies like GTK or Qt into the compositor and lightweight shell components, Halo uses a custom immediate-mode/retained-mode hybrid UI library (`libui`) backed by Skia or Cairo.
This ensures widgets look native, launch instantly, and consume minimal memory.

## Animation Engine Design (`libanimation`)

The animation engine is a mathematical curve evaluator decoupled from the render loop.

### Bezier Curves & Springs
Animations are defined by either cubic bezier curves or spring physics (mass, stiffness, damping). 

### Timelines
When a window opens, a timeline is created. 
`halo_animator_start(&anim_ctx, start_val, end_val, duration, HALO_EASE_OUT_EXPO);`

During the render loop, the compositor passes the current hardware presentation timestamp to the animator, which returns the interpolated value. This ensures animations remain perfectly smooth regardless of the refresh rate (60Hz, 144Hz, 240Hz).

## Config System Design (`libconfig`)

### Memory-Mapped IPC
Parsing text files inside the compositor on hot-reload is slow and unsafe. 
Halo DE uses a two-process config system:
1. **Config Compiler (CLI)**: Reads `~/.config/halo/compositor.conf`, validates the syntax, and compiles it into a binary struct representation.
2. **Settings Daemon**: Maps this binary struct into shared memory (shm) and notifies the compositor via IPC.
3. **Compositor**: Instantly reads the updated C struct from shared memory without parsing strings or allocating heap memory. This allows real-time, zero-latency configuration reloads.
