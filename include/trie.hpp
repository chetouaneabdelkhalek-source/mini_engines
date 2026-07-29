#ifndef TRIE_HPP
#define TRIE_HPP
#include <string> 
#include "arena.hpp"
class Trie
{

public:
    Trie(Arena &arena_ref) : arena(arena_ref)
    {
        root = arena.allocate_trie();
    }
    void insert(const std::string &word);
    bool search(const std::string &word);
    bool startsWith(const std::string &prefix);
    void remove(const std::string &word);

private:
    TrieNode *root;
    Arena &arena;
};

#endif