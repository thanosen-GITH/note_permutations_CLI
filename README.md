# Note Permutations

Generate ordered permutations of musical notes and optionally export MIDI (chordal, melodic, or both).

## Quick Start

From the project root:

```sh
sh macOS_build.sh
sh create_dist.sh
```

This builds and packages:
- macOS binary: `build/note_permutations`
- `dist/macos/`

## Cross-platform builds (optional)

### Windows (cross-compile from macOS/Linux)

```sh
sh Windows_build.sh
```

### Linux (Docker)

```sh
sh Linux_build.sh
```

## Run the app

### macOS
- Double-click: `dist/macos/Note Permutations MacOs.app`
- Or run directly: `dist/macos/bin/note_permutations`

### Windows
- Double-click: `dist/windows/Note Permutations WIN.bat`

### Linux
- Double-click: `dist/linux/Note Permutations Linux/Note Permutations.desktop`
- Or run directly: `dist/linux/Note Permutations Linux/bin/note_permutations`

## Manual build steps (optional)

### macOS (native)

```sh
cmake -S . -B build
cmake --build build
```

### Windows (cross-compile from macOS/Linux)

```sh
sh Windows_build.sh
```

### Linux (Docker)

```sh
sh Linux_build.sh
```

### Package into `dist/`

```sh
sh create_dist.sh
```

## License status (important)

This project is shared publicly for viewing and feedback, but **no license is granted at this time**.  
All rights are reserved by the author as of January 25, 2026.

## macOS security warning (unsigned app)

This app is **not notarized** by Apple (paid Developer ID required), so macOS may block it on first run.
Do **not** disable security system‑wide. Instead use one of these safe, per‑app options:

1. **System Settings override**
   - Try to open the app once (it will be blocked).
   - Go to **System Settings → Privacy & Security** and click **Open Anyway**.

2. **Remove the quarantine flag (Terminal)**

```sh
xattr -dr com.apple.quarantine "dist/macos/Note Permutations MacOs.app"
```

## Notes

- The Linux build uses Docker and installs build tools inside the container.
- If a launcher cannot find the binary, build first and re-run `sh create_dist.sh`.
