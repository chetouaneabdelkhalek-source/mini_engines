#ifndef ARENA_HPP
#define ARENA_HPP

#include <cstdint>
#include <vector>

#include "expr_tree.hpp" // for ParserNode, used by parser_pool below

struct node
{
    int data;
    node *left = nullptr;
    node *right = nullptr;
};

struct TrieNode
{
    bool is_end = false;
    uint8_t child_count = 0;
    TrieNode *children[26] = {nullptr};
};

class Arena
{
private:
    std::vector<node> tree_pool;
    std::vector<TrieNode> trie_pool;
    std::vector<ParserNode> parser_pool;

public:
    // NOTE:
    // This arena assumes the reserved capacity is never exceeded.
    // If more objects are allocated than the reserved capacity,
    // std::vector may reallocate, invalidating all previously
    // returned pointers and causing bugs.
    // To support arbitrary growth, implement capacity management

    Arena() {}

    void reserve_tree(int capacity = 100000)
    {
        tree_pool.reserve(capacity);
    }

    void reserve_trie(int capacity = 100000)
    {
        trie_pool.reserve(capacity);
    }

    void reserve_parser(int capacity = 100000)
    {
        parser_pool.reserve(capacity);
    }

    node* allocate_tree(int val)
    {
        tree_pool.push_back(node{val});
        return &tree_pool.back();
    }

    TrieNode* allocate_trie()
    {
        trie_pool.push_back(TrieNode{});
        return &trie_pool.back();
    }

    // Appends a hand-built or parser-built node to parser_pool and returns
    // its slot index (NOT a pointer — callers store this index in left/right
    // fields elsewhere, per the arena's index-based design).
    int allocate_parser(ParserNode node)
    {
        parser_pool.push_back(node);
        return static_cast<int>(parser_pool.size()) - 1;
    }

    const std::vector<ParserNode>& parser_nodes() const
    {
        return parser_pool;
    }
};

#endif