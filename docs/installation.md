# Installation & Builds

## Quick start (macOS)

From the project root:

```sh
sh macOS_build.sh
sh create_dist.sh
```

This produces:
- Binary: `build/note_permutations`
- Packaged app: `dist/macos/`

## Windows (cross-compile from macOS/Linux)

```sh
sh Windows_build.sh
sh create_dist.sh
```

## Linux

### Docker-based build

```sh
sh Linux_build.sh
sh create_dist.sh
```

### Native build

```sh
cmake -S . -B build-linux
cmake --build build-linux
sh create_dist.sh
```

## Running the app

### macOS / Linux

```sh
sh chmod_all.sh
```

Then:
- Run the packaged app from `dist/`
- Or execute the binary directly from `dist/**/bin/`
