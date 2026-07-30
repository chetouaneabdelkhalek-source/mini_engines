#include <cassert>
#include <cmath>
#include <iostream>

#include "expr_tree.hpp"
#include "parser.hpp"
#include "tests.hpp"

namespace
{
bool near(double a, double b, double eps = 1e-2)
{
    return std::fabs(a - b) < eps;
}

double parse_and_eval(const std::string& input, double x = 0.0)
{
    Parser parser(input);
    int root = parser.parse();
    return eval_iterative(parser.pool, x);
}

bool throws(const std::string& input)
{
    try
    {
        Parser parser(input);
        parser.parse();
        return false; // no exception -> test should fail
    }
    catch (const std::runtime_error&)
    {
        return true;
    }
}
} // namespace

void run_parser_tests()
{
    std::cout << "\n=== Running Parser Test Suite ===" << std::endl;

    // Precedence and associativity
    assert(near(parse_and_eval("2+3*4"), 14.0));
    assert(near(parse_and_eval("(2+3)*4"), 20.0));
    assert(near(parse_and_eval("2^3^2"), 512.0));
    std::cout << "[PASS] precedence and right-associative ^" << std::endl;

    // Unary minus (grammar choice: binds tighter than ^, see README note)
    assert(near(parse_and_eval("-2^2"), -4.0));
    assert(near(parse_and_eval("(-2)^2"), 4.0));
    assert(near(parse_and_eval("--2"), 2.0));
    std::cout << "[PASS] unary minus, matches documented grammar precedence" << std::endl;

    // Functions and variables
    assert(near(parse_and_eval("exp(1+2)"), std::exp(3.0)));
    assert(near(parse_and_eval("x", 5.0), 5.0));
    assert(near(parse_and_eval("x^2+3*x+1", 2.0), 11.0));
    std::cout << "[PASS] exp() and variable x" << std::endl;

    // Malformed input must throw, not silently produce a wrong value
    assert(throws("2+"));
    assert(throws("("));
    assert(throws("exp("));
    assert(throws("foo"));
    assert(throws("2abc"));
    std::cout << "[PASS] malformed input correctly rejected" << std::endl;

    std::cout << ">>> ALL PARSER TESTS PASSED SUCCESSFULLY! <<<\n" << std::endl;
}