#ifndef PARSER_HPP
#define PARSER_HPP
#include <cstddef>
#include <string>
#include <string>
#include <vector>
#include "expr_tree.hpp"

struct Parser {
    const std::string& src;
    size_t pos = 0;
    std::vector<ParserNode> pool;

    explicit Parser(const std::string& source) : src(source) {}

    // entry point — parses src, returns root index into pool
    int parse();

    char peek();
    bool match(char c);
    double parse_number();
    std::string parse_ident();

    int expr();
    int term();
    int unary();
    int power();
    int primary();
};

#endif