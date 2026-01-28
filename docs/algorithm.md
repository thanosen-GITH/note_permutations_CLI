# Algorithm Notes

## Non-recursive permutation generation

This project uses a **custom non-recursive algorithm** to generate the i-th permutation directly.

### Why this matters

- Avoids deep recursion
- Enables random access to permutations
- Designed with future GUI/query-based exploration in mind

### Implementation idea

- Permutations are derived using sub-tables
- The algorithm computes the permutation index mathematically rather than iterating from the start

This design allows future extensions such as:
- Jumping directly to a permutation by index
- Interactive browsing of the permutation space
