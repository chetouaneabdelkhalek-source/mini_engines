#include "trie.hpp"
#include "arena.hpp"
#include <iostream>
#include <stack>
#include <utility>
void Trie::insert(const std::string &word)
{
    TrieNode *curr = root;
    for (const char &c : word)
    {
        if (curr->children[c - 'a'] == nullptr)
        {
            curr->children[c - 'a'] = arena.allocate_trie();
            curr->child_count++;
        }
        curr = curr->children[c - 'a'];
    }
    curr->is_end = true;
    std::cout << word << " SUCCESFULLY INSERTED" << std::endl;
}
bool Trie::search(const std::string &word)
{
    TrieNode *curr = root;
    bool failed = false;
    for (const char &c : word)
    {
        if (curr->children[c - 'a'] == nullptr)
        {
            failed = true;
            break;
        }
        curr = curr->children[c - 'a'];
    }
    if (failed ||! curr->is_end )
    {
        std::cout << word << " NOT FOUND IN TRIE" << std::endl;
        return false;
    }
    else
    {
        std::cout << word << " FOUND IN TRIE" << std::endl;
        return true;
    }
}
bool Trie::startsWith(const std::string &prefix)
{

    TrieNode *curr = root;
    for (const char &c : prefix)
    {
        if (curr->children[c - 'a'] == nullptr)
        {
            std::cout << prefix << " PREFIX NOT FOUND IN TRIE" << std::endl;
            return false;
        }
        curr = curr->children[c - 'a'];
    }
    std::cout << prefix << " PREFIX FOUND IN TRIE" << std::endl;
    return true;
}

void Trie::remove(const std::string &word)
{
   
    std::stack<std::pair<TrieNode *, int>> word_path;

    TrieNode *curr = root;
    // storing word path logic
    for (const char &c : word)

    {
        int idx = c - 'a';
        if (curr->children[idx] == nullptr)
        { 
            
            return;
        }
        word_path.push(std::make_pair(curr, idx));
        curr = curr->children[idx];
    }
    if (curr->is_end)
    {
        curr->is_end = false;
    }
    else
    {
        return;
    }
    // deleting logic
    while (!word_path.empty())
    {
        auto [parent, idx] = word_path.top();
        TrieNode *child = parent->children[idx];
        if (child->child_count == 0 && ! child->is_end)
        {
            parent->children[idx] = nullptr;
            parent->child_count--;
            word_path.pop();
        }
        else
        {
            return;
        }
    }
}
