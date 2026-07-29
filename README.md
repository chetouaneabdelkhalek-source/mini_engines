# mini_engines

C++20 systems primitives: an arena allocator (index-based, no per-node
`new`), an arena-backed expression tree with both recursive and iterative
(stack-based, non-recursive) `eval()`, a trie, and iterative
pre/in/post-order tree traversals.

Built as curriculum/scaffolding work — expression-tree evaluation here is
direct preparation for a future autograd engine's forward pass.

## Build

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build
./build/mini_engines_run
```

## Contents

- `include/arena.hpp`, — raw arena allocator (three pools: tree nodes, trie nodes, expression-tree nodes)
- `include/expr_tree.hpp` / `src/expr_tree.cpp` — expression tree node types, recursive + iterative `eval()`, recursive-vs-iterative benchmark
- `include/trie.hpp` / `src/trie.cpp` — trie (insert/search/startsWith/remove with pruning)
- `include/traversals.hpp` / `src/traversals.cpp` — iterative (non-recursive) pre/in/post-order traversals
- `src/test_*.cpp` — per-module test suites, run via `src/tests_main.cpp`

Exit criterion for the expression tree: parses/evaluates `3*(2+exp(x))` as a
hand-built arena tree, `eval(1.0) ≈ 14.15`, `eval(0.0) = 9.0`, Valgrind clean.
