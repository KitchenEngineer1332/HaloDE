# Security, Session, and Services

## Security Architecture

Halo DE is designed with modern Linux sandboxing (Flatpak) in mind.

### Wayland Protocol Security
Unlike X11, Wayland clients cannot read other clients' inputs or buffers by default. Halo WM enforces strict protocol security:
- **screencopy / wlr-export-dmabuf**: Restricted protocols. By default, clients cannot take screenshots or record screens. A permission manager acts as a middleman. When a client requests screencopy, the compositor blocks the request and triggers a DBus prompt via `xdg-desktop-portal-halo`. Only if the user approves is the Wayland protocol bound to the client.
- **layer-shell**: Only authenticated shell components (launched by the session manager) can bind to `zwlr_layer_shell_v1`.

### Privilege Separation
The compositor runs as the logged-in user, but acquires necessary DRM/input capabilities via `logind` or `seatd`. It never runs as root.

## Session / Login Architecture

The lifecycle of a Halo DE session is managed by `halo-session`.

1. **Greeter (Display Manager)**: A specialized, locked-down instance of Halo WM starts. It runs `halo-greeter` (the login UI).
2. **PAM Authentication**: The greeter authenticates the user via `PAM`.
3. **Session Handoff**: Upon success, `halo-session` spawns a new TTY, launches the user's `halo-wm` instance, and populates the environment variables (`WAYLAND_DISPLAY`, `XDG_RUNTIME_DIR`).

## Service Daemon Architecture

Background tasks are split into isolated daemons that communicate with the shell via DBus or Halo IPC.

- **halo-polkit**: An authentication agent for Polkit.
- **halo-power**: Interfaces with `upower` to suspend the system, manage battery alerts, and dim the screen via IPC commands to the compositor.
- **halo-portal**: Implementation of `xdg-desktop-portal` for file dialogs, screencasting, and global shortcuts.
