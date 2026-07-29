#include "expr_tree.hpp"

#include <chrono>
#include <cmath>
#include <iostream>
#include <stack>

double eval_recursive(const ParserNode& root, const std::vector<ParserNode>& pool, const double& x)
{
    if (root.kind == Kind::CONST)
    {
        return root.data;
    }
    if (root.kind == Kind::VAR)
    {
        return x;
    }
    else if (root.kind == Kind::BINARY)
    {
        switch (root.op)
        {
        case Op::ADD:
            return eval_recursive(pool[root.left], pool, x) +
                   eval_recursive(pool[root.right], pool, x);

        case Op::SUB:
            return eval_recursive(pool[root.left], pool, x) -
                   eval_recursive(pool[root.right], pool, x);

        case Op::MUL:
            return eval_recursive(pool[root.left], pool, x) *
                   eval_recursive(pool[root.right], pool, x);

        case Op::DIV:
            return eval_recursive(pool[root.left], pool, x) /
                   eval_recursive(pool[root.right], pool, x);

        case Op::POW:
            return std::pow(
                eval_recursive(pool[root.left], pool, x),
                eval_recursive(pool[root.right], pool, x));

        default:
            break;
        }
    }
    else if (root.kind == Kind::UNARY)
    {
        switch (root.op)
        {
        case Op::NEG:
            return -eval_recursive(pool[root.left], pool, x);

        case Op::EXP:
            return std::exp(eval_recursive(pool[root.left], pool, x));

        default:
            break;
        }
    }

    return 0;
}

double eval_iterative(const std::vector<ParserNode>& pool, const double& x)
{
    std::stack<double, std::vector<double>> values;

    for (const ParserNode& node : pool)
    {
        if (node.kind == Kind::CONST)
        {
            values.push(node.data);
        }
        else if (node.kind == Kind::VAR)
        {
            values.push(x);
        }
        else if (node.kind == Kind::BINARY)
        {
            double right = values.top();
            values.pop();

            double left = values.top();
            values.pop();

            switch (node.op)
            {
            case Op::ADD:
                values.push(left + right);
                break;

            case Op::SUB:
                values.push(left - right);
                break;

            case Op::MUL:
                values.push(left * right);
                break;

            case Op::DIV:
                values.push(left / right);
                break;

            case Op::POW:
                values.push(std::pow(left, right));
                break;

            default:
                break;
            }
        }
        else if (node.kind == Kind::UNARY)
        {
            double operand = values.top();
            values.pop();

            switch (node.op)
            {
            case Op::NEG:
                values.push(-operand);
                break;

            case Op::EXP:
                values.push(std::exp(operand));
                break;

            default:
                break;
            }
        }
    }

    return values.top();
}

void benchmark_eval(const std::vector<ParserNode>& pool, double x, int iterations)
{
    volatile double sink = 0.0; // prevents the optimizer from deleting the "unused" result

    auto start_recursive = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        sink = eval_recursive(pool[pool.size() - 1], pool, x);
    }
    auto end_recursive = std::chrono::high_resolution_clock::now();

    auto start_iterative = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < iterations; ++i)
    {
        sink = eval_iterative(pool, x);
    }
    auto end_iterative = std::chrono::high_resolution_clock::now();

    (void)sink;

    auto recursive_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_recursive - start_recursive).count();
    auto iterative_ns =
        std::chrono::duration_cast<std::chrono::nanoseconds>(end_iterative - start_iterative).count();

    std::cout << "=== eval() benchmark (" << iterations << " iterations) ===\n";
    std::cout << "recursive: " << (double)recursive_ns / iterations << " ns/call\n";
    std::cout << "iterative: " << (double)iterative_ns / iterations << " ns/call\n";
}