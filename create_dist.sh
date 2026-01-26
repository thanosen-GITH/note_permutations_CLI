#!/bin/sh
set -e

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
DIST_DIR="$ROOT_DIR/dist"
MAC_DIR="$DIST_DIR/macos"
WIN_DIR="$DIST_DIR/windows"
LINUX_DIR="$DIST_DIR/linux"

mkdir -p "$MAC_DIR/bin" "$WIN_DIR/bin" "$LINUX_DIR"

# Build with CMake if not already built
if [ ! -f "$ROOT_DIR/build/note_permutations" ] && [ ! -f "$ROOT_DIR/build-win/note_permutations.exe" ] && [ ! -f "$ROOT_DIR/build-linux/note_permutations" ]; then
  echo "No build output found. Build first with CMake (see PACKAGING.md)."
fi

# Copy binaries if present
if [ -f "$ROOT_DIR/build/note_permutations" ]; then
  cp "$ROOT_DIR/build/note_permutations" "$MAC_DIR/bin/"
fi
if [ -f "$ROOT_DIR/build-linux/note_permutations" ]; then
  :
fi
if [ -f "$ROOT_DIR/build-win/note_permutations.exe" ]; then
  cp "$ROOT_DIR/build-win/note_permutations.exe" "$WIN_DIR/bin/"
fi

# Windows launcher
cp "$ROOT_DIR/packaging/win/Note Permutations WIN.bat" "$WIN_DIR/"

# macOS app wrapper
MAC_APP_SRC="$ROOT_DIR/packaging/macos/Note Permutations MacOs.app"
MAC_APP_DST="$MAC_DIR/Note Permutations MacOs.app"
if [ -d "$MAC_APP_SRC" ]; then
  rm -rf "$MAC_APP_DST"
  cp -R "$MAC_APP_SRC" "$MAC_APP_DST"
  if [ -f "$MAC_DIR/bin/note_permutations" ]; then
    mkdir "$MAC_APP_DST/Contents/Resources"
    cp "$MAC_DIR/bin/note_permutations" "$MAC_APP_DST/Contents/Resources"
  fi
fi

# Linux launcher bundle
LINUX_BUNDLE_DIR="$LINUX_DIR/Note Permutations Linux"
rm -rf "$LINUX_BUNDLE_DIR"
mkdir -p "$LINUX_BUNDLE_DIR/bin"
if [ -f "$ROOT_DIR/build-linux/note_permutations" ]; then
  cp "$ROOT_DIR/build-linux/note_permutations" "$LINUX_BUNDLE_DIR/bin/"
fi
cat > "$LINUX_BUNDLE_DIR/run.sh" <<'EOF'
#!/bin/sh
DIR="$(cd "$(dirname "$0")" && pwd)"
"$DIR/bin/note_permutations"
EOF
chmod 755 "$LINUX_BUNDLE_DIR/run.sh"
cat > "$LINUX_BUNDLE_DIR/Note Permutations.desktop" <<'EOF'
[Desktop Entry]
Type=Application
Name=Note Permutations
Exec=sh -c '"$(dirname "%k")/run.sh"'
Terminal=true
Categories=Utility;Music;
EOF

echo "dist/ prepared under dist/macos, dist/windows, and dist/linux."
