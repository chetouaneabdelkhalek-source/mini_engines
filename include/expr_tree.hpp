#ifndef EXPR_TREE_HPP
#define EXPR_TREE_HPP

#include <cstdint>
#include <vector>

enum class Kind : uint8_t { CONST, VAR, UNARY, BINARY };
enum class Op : uint8_t { ADD, SUB, MUL, DIV, NEG, EXP, POW };

struct ParserNode
{
    double data;
    int left = -1;
    int right = -1;
    Kind kind;
    Op op;
};

// Recursive evaluator — kept for correctness reference/benchmarking against
// the iterative version. Not the shipped entry point (recursion depth is
// unbounded, which violates the project's no-unbounded-recursion constraint).
double eval_recursive(const ParserNode& root, const std::vector<ParserNode>& pool, const double& x);

// Iterative evaluator (single linear scan + explicit value stack) — the
// shipped entry point. Requires pool to already be in dependency order
// (every node's children stored at lower indices than the node itself).
double eval_iterative(const std::vector<ParserNode>& pool, const double& x);

// Times eval_recursive vs eval_iterative over `iterations` calls on the same
// (pool, x) and prints the average per-call duration for each, in nanoseconds.
void benchmark_eval(const std::vector<ParserNode>& pool, double x, int iterations = 100000);

#endif