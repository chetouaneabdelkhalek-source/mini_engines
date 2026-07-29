#include <cassert>
#include <iostream>

#include "arena.hpp"
#include "tests.hpp"
#include "trie.hpp"

void run_trie_tests()
{
    std::cout << "\n=== Running Trie Test Suite ===" << std::endl;
    Arena arena;
    arena.reserve_trie(1000);
    Trie trie(arena);

    trie.insert("cat");
    trie.insert("car");
    trie.insert("cats");

    assert(trie.search("cat") == true);
    assert(trie.search("car") == true);
    assert(trie.search("cats") == true);
    assert(trie.search("ca") == false);
    assert(trie.search("dog") == false);
    std::cout << "[PASS] Insertion & Search Tests" << std::endl;

    assert(trie.startsWith("ca") == true);
    assert(trie.startsWith("cat") == true);
    assert(trie.startsWith("do") == false);
    std::cout << "[PASS] Prefix Matching Tests" << std::endl;

    trie.remove("cats");
    assert(trie.search("cats") == false);
    assert(trie.search("cat") == true);
    assert(trie.search("car") == true);
    std::cout << "[PASS] Pruning Deletion Tests" << std::endl;

    trie.remove("cat");
    assert(trie.search("cat") == false);
    assert(trie.search("car") == true);
    assert(trie.startsWith("ca") == true);
    std::cout << "[PASS] Overlapping Removal Tests" << std::endl;

    std::cout << ">>> ALL TRIE TESTS PASSED SUCCESSFULLY! <<<\n" << std::endl;
}