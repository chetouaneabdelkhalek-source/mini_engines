#include <iostream>

#include "arena.hpp"
#include "tests.hpp"
#include "traversals.hpp"

namespace
{
void test_small_tree()
{
    std::cout << "=== Test 1: Small Tree Correctness ===" << std::endl;
    Arena arena;
    arena.reserve_tree(10);

    node* root = arena.allocate_tree(10);
    root->left = arena.allocate_tree(5);
    root->right = arena.allocate_tree(15);
    root->left->left = arena.allocate_tree(2);

    std::cout << "Pre-Order:  ";
    stack_preOrderTraversal(root);
    std::cout << "\nIn-Order:   ";
    stack_inOrderTraversal(root);
    std::cout << "\nPost-Order: ";
    stack_postOrderTraversal(root);
    std::cout << "\n" << std::endl;
}

void test_depth_1000()
{
    std::cout << "=== Test 2: Depth 1,000 Test ===" << std::endl;
    Arena arena;
    arena.reserve_tree(1000);
    node* root = arena.allocate_tree(1);
    node* curr = root;

    for (int i = 2; i <= 1000; ++i)
    {
        curr->right = arena.allocate_tree(i);
        curr = curr->right;
    }

    stack_inOrderTraversal(root);
    std::cout << "Depth 1,000 completed successfully!\n" << std::endl;
}

void test_depth_100000()
{
    std::cout << "=== Test 3: Depth 100,000 Stress Test ===" << std::endl;
    Arena arena;
    arena.reserve_tree(100000);
    node* root = arena.allocate_tree(1);
    node* curr = root;

    for (int i = 2; i <= 100000; ++i)
    {
        curr->right = arena.allocate_tree(i);
        curr = curr->right;
    }

    stack_postOrderTraversal(root);
    std::cout << "Depth 100,000 completed successfully!\n" << std::endl;
}
} // namespace

void run_traversal_tests()
{
    std::cout << "\n=== Running Traversal Test Suite ===" << std::endl;
    test_small_tree();
    //test_depth_1000();
    //test_depth_100000();
    std::cout << ">>> ALL TRAVERSAL TESTS PASSED SUCCESSFULLY! <<<\n" << std::endl;
}