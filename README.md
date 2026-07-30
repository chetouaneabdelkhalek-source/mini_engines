# mini_engines

C++20 systems primitives featuring an arena-backed recursive-descent
expression parser, iterative expression evaluation, a trie, and iterative
tree traversals — built to explore the implementation techniques used in
machine learning systems.

Everything here is arena-backed (no per-node `new`, no `std::unique_ptr`
graphs) and biased toward iterative, non-recursive implementations where
recursion depth would otherwise be unbounded.

**Contents:** an index/pointer arena allocator, a recursive-descent
expression parser + tree with recursive and iterative `eval()`, a trie
(insert/search/prefix/delete with pruning), and iterative pre/in/post-order
tree traversals.

## Example

Source `3*(2+exp(x))` parses into this arena-backed AST (dependency order,
root last):

```
Input:
    3*(2+exp(x))

AST:
    MUL
    ├── 3
    └── ADD
        ├── 2
        └── EXP
            └── x

eval(0.0) = 9.0
eval(1.0) ≈ 14.1548
```

## Build & run

Requires CMake ≥ 3.10 and a C++20 compiler (GCC or Clang).

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -B build
cmake --build build
./build/mini_engines_run
```

Use `-DCMAKE_BUILD_TYPE=Release` for the optimized build (`-O3`); Debug
builds with `-O0 -g` for use with a debugger or Valgrind.

## Layout

```
include/
  arena.hpp        raw arena allocator (separate pools for tree, trie, parser nodes)
  expr_tree.hpp     ParserNode, Kind/Op enums, eval_recursive, eval_iterative, benchmark_eval
  parser.hpp        recursive-descent Parser: source string -> arena-indexed expression pool
  trie.hpp          Trie class (insert / search / startsWith / remove)
  traversals.hpp    iterative pre/in/post-order traversal declarations
  tests.hpp         test-suite entry points
src/
  expr_tree.cpp     eval_recursive / eval_iterative / benchmark_eval implementations
  parser.cpp        the parser grammar (below)
  trie.cpp          trie implementation
  traversals.cpp    traversal implementations
  test_*.cpp        one test file per module
  tests_main.cpp    runs all test suites
```

## The pieces

### Arena (`arena.hpp`)

One `Arena` owns three separate `std::vector` pools — tree nodes, trie
nodes, and parser (expression-tree) nodes — and hands out either raw
pointers (`allocate_tree`, `allocate_trie`) or pool indices
(`allocate_parser`), depending on which structure is consuming them. Call
`reserve_*` up front with an expected capacity: the arena does **not**
guard against reallocation, so exceeding a reserved capacity silently
invalidates every pointer handed out so far. This is a known, intentional
limitation — see the note in the header — not something to work around by
over-reserving in client code.

### Expression tree & parser (`expr_tree.hpp`, `parser.hpp`, `parser.cpp`)

`Parser` is a recursive-descent parser over a single free variable `x`,
producing a flat `std::vector<ParserNode>` where each node's children are
stored as indices *lower* than the node itself (dependency order). Grammar,
lowest to highest precedence:

```
expr    -> term (('+' | '-') term)*        left-assoc
term    -> unary (('*' | '/') unary)*      left-assoc
unary   -> '-' unary | power
power   -> primary ('^' power)?            right-assoc
primary -> NUMBER | 'x' | '(' expr ')' | 'exp(' expr ')'
```

Supported operators: `+ - * / ^` (power, right-associative) and unary `-`,
plus the `exp(...)` function — `ADD/SUB/MUL/DIV/NEG/EXP/POW` in the `Op`
enum (`expr_tree.hpp`).

Two evaluators over the resulting pool:
- `eval_recursive` — correctness reference and benchmark baseline. **Not**
  the shipped entry point, since it recurses per node and has no depth
  bound.
- `eval_iterative` — the shipped entry point. Single linear scan over the
  pool plus an explicit value stack; relies on the dependency-order
  invariant the parser guarantees.

`benchmark_eval` times both over `iterations` calls on the same `(pool, x)`
and reports average per-call latency in nanoseconds. Sample output on the
`3*(2+exp(x))` tree, Release build, 100,000 iterations:

```
=== eval() benchmark (100000 iterations) ===
recursive: 18.27 ns/call
iterative: 67.55 ns/call
```

For this tiny six-node tree, recursive evaluation is faster because the
compiler can inline the calls and the explicit stack introduces extra
overhead. The iterative evaluator remains the primary implementation
because it avoids recursion-depth limits on large trees.

### Trie (`trie.hpp`, `trie.cpp`)

Fixed 26-way branching (lowercase `a`–`z`), arena-backed nodes. Supports
`insert`, `search`, `startsWith`, and `remove` — removal prunes now-dead
nodes back up the path rather than just clearing the `is_end` flag.

### Traversals (`traversals.hpp`, `traversals.cpp`)

Iterative (explicit-stack, non-recursive) pre-, in-, and post-order
traversals over the plain binary `node` type from `arena.hpp`.

## Tests

Each module has its own `test_*.cpp` with a `run_*_tests()` entry point
declared in `tests.hpp`; `tests_main.cpp` runs them all. Building and
running `mini_engines_run` executes the full suite — there's no separate
test target or filtering flag yet.

Verified with Valgrind: zero memory leaks and zero invalid reads/writes.

```bash
valgrind --leak-check=full ./build/mini_engines_run
```

## Known limitations

- Arena pools are fixed-capacity: reserve before you allocate, or pointers
  returned earlier become dangling on reallocation.
- Parser only recognizes the single variable `x` and the single function
  `exp`; no multi-variable expressions, no other transcendental functions.
- Trie is lowercase-only (`a`–`z`); no Unicode, digits, or uppercase.