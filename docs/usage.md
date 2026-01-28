# Usage

## Basic concept

You choose a number of notes between **1 and 12**.

The program generates **all ordered permutations** of those notes and writes the results to disk.

## Running

Depending on platform, launch via:

- Packaged app (double-click)
- Command-line binary:

```sh
note_permutations
```

(Exact invocation may differ depending on platform packaging.)

## Output volume warning

The number of permutations grows factorially:

- 5 notes → 120 permutations
- 8 notes → 40,320 permutations
- 12 notes → 479,001,600 permutations

Large note counts will generate **very large output files** and may take significant time.
