# Note Permutations Packaging

## Build macOS (native)

```sh
cmake -S . -B build
cmake --build build
```

## Build Windows (cross-compile on macOS/Linux)

```sh
sh Windows_build
```

The executable will be at `build-win/note_permutations.exe`.

## Build Linux (Docker)

```sh
sh Linux_build.sh
```

This expects a Docker image that already has `cmake` and build tools.

## Create dist folder

```sh
sh packaging/chmod_all.sh
sh create_dist.sh
```

This creates:
- `dist/bin/` with the binary (if found)
- `dist/bin/` with detected binaries
- `dist/Note Permutations WIN.bat` (Windows launcher)
- `dist/Note Permutations MacOs.app` (macOS app wrapper)
- `dist/Note Permutations Linux/` (Linux launcher bundle)

## Using the launchers

- **Windows**: double‑click `dist/Note Permutations WIN.bat`
- **Linux**: double‑click `dist/Note Permutations Linux/Note Permutations.desktop`
- **macOS**: double‑click `dist/Note Permutations MacOs.app`

If a launcher can’t find the binary, copy it into the expected `bin/` folder and re-run `sh create_dist.sh`.
