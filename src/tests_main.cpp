#include <iostream>

#include "tests.hpp"

int main()
{
    run_expr_tree_tests();
    run_trie_tests();
    run_traversal_tests();
    run_parser_tests();

    std::cout << "ALL TESTS PASSED!" << std::endl;
    return 0;
}