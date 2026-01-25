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

## Notes

- The Linux build uses Docker and installs build tools inside the container.
- If a launcher cannot find the binary, build first and re-run `sh create_dist.sh`.
