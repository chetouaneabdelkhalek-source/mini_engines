#include "parser.hpp"
#include <stdexcept>

// expr    -> term (('+' | '-') term)*
// term    -> unary (('*' | '/') unary)*
// unary   -> '-' unary | power
// power   -> primary ('^' power)?
// primary -> NUMBER | IDENT | '(' expr ')' | 'exp(' expr ')'
int Parser::parse() {
  int root = expr();

  if (peek() != '\0')
    throw std::runtime_error(std::string("Parsing error: unexpected token '") +
                             peek() + "'");

  return root;
}

char Parser::peek() {
  while (pos < src.size() && std::isspace(src[pos])) {
    ++pos;
  }
  if (pos >= src.size()) {
    return '\0';
  }
  return src[pos];
}

double Parser::parse_number() {
  size_t start = pos;
  bool seen_dot = false;
  while (pos < src.size() &&
         (std::isdigit(src[pos]) || (src[pos] == '.' && !seen_dot))) {
    if (src[pos] == '.')
      seen_dot = true;
    ++pos;
  }
  std::string num_str = src.substr(start, pos - start);
  return std::stod(num_str);
}
std::string Parser::parse_ident() {
  size_t start = pos;
  while (pos < src.size() && std::isalpha(src[pos])) {
    ++pos;
  }
  return src.substr(start, pos - start);
}
// + / -, left-associative
int Parser::expr() {
  int left = term();
  char c ; 
  while ((c = peek()) == '+' || c == '-') {
    Op op = (c == '+') ? Op::ADD : Op::SUB;
    ++pos;
    int right = term();
    pool.push_back(
        {.left = left, .right = right, .kind = Kind::BINARY, .op = op});
    left = (int)pool.size() - 1;
  }
  return left;
}
int Parser::term() {
    int left = unary();
    char c ; 
    while ((c = peek()) == '*' || c == '/') {
        Op op = (c == '*') ? Op::MUL : Op::DIV;
        ++pos;
        int right = unary();

        pool.push_back(
            {.left = left, .right = right, .kind = Kind::BINARY, .op = op});

        left = (int)pool.size() - 1;
    }

    return left;
}
int Parser::unary() {
    if (peek() == '-') {
        ++pos;

        int child = unary();

        pool.push_back({
            .left = child,
            .kind = Kind::UNARY,
            .op = Op::NEG
        });

        return (int)pool.size() - 1;
    }

    return power();
}
// ^, right-associative
int Parser::power() {
  int left = primary();
  if (peek() == '^') {

    ++pos;
    int right = power();
    pool.push_back(
        {.left = left, .right = right, .kind = Kind::BINARY, .op = Op::POW});
    left = (int)pool.size() - 1;
  }
  return left;
}

int Parser::primary() {
  int idx ;

  if (std::isdigit(peek())) {

    double v = parse_number();
    pool.push_back({.data = v, .kind = Kind::CONST});
    idx = (int)pool.size() - 1;

  } else if (std::isalpha(peek())) {

    std::string name = parse_ident();

    if (name == "exp") {

      if (peek() != '(')
        throw std::runtime_error(
            "Parsing error: expected parentheses after exponential function.");

      ++pos;
      int left = expr();

      if (peek() != ')')
        throw std::runtime_error(
            "Parsing error: expected ')' after exponential expression.");

      ++pos;

      pool.push_back({.left = left, .kind = Kind::UNARY, .op = Op::EXP});
      idx = (int)pool.size() - 1;

    } else if (name == "x") {

      pool.push_back({.kind = Kind::VAR});
      idx = (int)pool.size() - 1;

    } else {

      throw std::runtime_error("Parsing error: cannot parse \"" + name + "\"");
    }

  } else if (peek() == '(') {

    ++pos;
    idx = expr();

    if (peek() != ')')
      throw std::runtime_error("Parsing error: expected ')'.");

    ++pos;

  } else {

    char c = peek();

    if (c == '\0')
      throw std::runtime_error("Parsing error: unexpected end of input.");

    throw std::runtime_error(std::string("Parsing error: unexpected token '") +
                             c + "'");
  }

  return idx;
}