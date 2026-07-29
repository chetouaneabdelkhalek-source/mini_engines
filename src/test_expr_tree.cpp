#include <cassert>
#include <cmath>
#include <iostream>

#include "expr_tree.hpp"
#include "tests.hpp"

namespace
{
// 3*(2+exp(x)), built by hand in dependency order:
// slot 0: 3 (CONST)      slot 3: exp(slot2)  (UNARY, EXP)
// slot 1: 2 (CONST)      slot 4: slot1+slot3 (BINARY, ADD)
// slot 2: x (VAR)        slot 5: slot0*slot4 (BINARY, MUL)
std::vector<ParserNode> build_test_tree()
{
    ParserNode idx0{.data = 3, .kind = Kind::CONST};
    ParserNode idx1{.data = 2, .kind = Kind::CONST};
    ParserNode idx2{.kind = Kind::VAR};
    ParserNode idx3{.left = 2, .kind = Kind::UNARY, .op = Op::EXP};
    ParserNode idx4{.left = 1, .right = 3, .kind = Kind::BINARY, .op = Op::ADD};
    ParserNode idx5{.left = 0, .right = 4, .kind = Kind::BINARY, .op = Op::MUL};
    return {idx0, idx1, idx2, idx3, idx4, idx5};
}

bool near(double a, double b, double eps = 1e-2)
{
    return std::fabs(a - b) < eps;
}
} // namespace

void run_expr_tree_tests()
{
    std::cout << "\n=== Running Expression Tree Test Suite ===" << std::endl;

    const std::vector<ParserNode> pool = build_test_tree();
    const int root = static_cast<int>(pool.size()) - 1;

    // Both evaluators must agree with each other and with the spec's two checkpoints.
    assert(near(eval_iterative(pool, 1.0), 14.15));
    assert(near(eval_iterative(pool, 0.0), 9.0));
    assert(near(eval_recursive(pool[root], pool, 1.0), 14.15));
    assert(near(eval_recursive(pool[root], pool, 0.0), 9.0));
    std::cout << "[PASS] eval_iterative and eval_recursive match spec checkpoints" << std::endl;

    assert(near(eval_recursive(pool[root], pool, 1.0), eval_iterative(pool, 1.0)));
    assert(near(eval_recursive(pool[root], pool, 0.0), eval_iterative(pool, 0.0)));
    std::cout << "[PASS] eval_recursive and eval_iterative agree with each other" << std::endl;

    benchmark_eval(pool, 1.0);

    std::cout << ">>> ALL EXPRESSION TREE TESTS PASSED SUCCESSFULLY! <<<\n" << std::endl;
}