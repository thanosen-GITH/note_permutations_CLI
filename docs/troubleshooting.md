# Troubleshooting

## App does not open on macOS

The app is not notarized.

Use one of the following safe options:

```sh
xattr -dr com.apple.quarantine "dist/macos/Note Permutations MacOs.app"
```

Or allow it via:
System Settings → Privacy & Security → Open Anyway

## Launcher cannot find binary

Build the project first, then re-run:

```sh
sh create_dist.sh
```

## Program appears frozen

For large note counts, the program may still be running.

Check CPU and disk activity before force-quitting.
