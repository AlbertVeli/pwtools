#!/bin/sh
# Keep screensaver from activating by simulating user activity via D-Bus.
# Works on both X11 and Wayland. No root required.

INTERVAL=30

echo "Keeping screen awake (every ${INTERVAL}s). Press Ctrl+C to stop."

while true; do
    dbus-send --session \
              --dest=org.freedesktop.ScreenSaver \
              --type=method_call \
              /org/freedesktop/ScreenSaver \
              org.freedesktop.ScreenSaver.SimulateUserActivity 2>/dev/null || \
    dbus-send --session \
              --dest=org.gnome.ScreenSaver \
              --type=method_call \
              /org/gnome/ScreenSaver \
              org.gnome.ScreenSaver.SimulateUserActivity 2>/dev/null
    sleep "$INTERVAL"
done
