# Halo DE Wrapper Makefile
# This allows users to use standard 'make' and 'make install' commands.
# Under the hood, it can wrap Meson, but for now, we use it to install the session files.

PREFIX ?= /usr/local
BINDIR ?= $(PREFIX)/bin

# Display Managers (GDM/SDDM/LightDM) usually only look in /usr/share/wayland-sessions
# so we hardcode the system path for the .desktop file, respecting DESTDIR for packagers.
WAYLAND_SESSIONS ?= /usr/share/wayland-sessions

all:
	@echo "Halo DE is currently in skeleton mode."
	@echo "Once the C code is fully wired up, this will run: meson setup build && ninja -C build"

install:
	@echo "Installing Halo DE session files..."
	
	# Create target directories
	install -d $(DESTDIR)$(WAYLAND_SESSIONS)
	install -d $(DESTDIR)$(BINDIR)

	# Install the Wayland session file so Display Managers (GDM/SDDM) can see it
	install -m 644 assets/halo.desktop $(DESTDIR)$(WAYLAND_SESSIONS)/halo.desktop

	# Install the session launcher script
	install -m 755 scripts/halo-session $(DESTDIR)$(BINDIR)/halo-session

	# Install a dummy executable for halo-wm just so the session doesn't instantly fail
	# (Remove this once compositor/main.c is compiling into halo-wm)
	echo '#!/bin/sh\necho "Halo WM starting..."\nsleep 5' > $(DESTDIR)$(BINDIR)/halo-wm
	chmod +x $(DESTDIR)$(BINDIR)/halo-wm

	@echo ""
	@echo "Install complete!"
	@echo "You can now log out, and 'Halo' should appear in your display manager's session list."

uninstall:
	rm -f $(DESTDIR)$(WAYLAND_SESSIONS)/halo.desktop
	rm -f $(DESTDIR)$(BINDIR)/halo-session
	rm -f $(DESTDIR)$(BINDIR)/halo-wm

.PHONY: all install uninstall
