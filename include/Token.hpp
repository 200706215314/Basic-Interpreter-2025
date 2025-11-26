#pragma once

#include <string>
#include <vector>

enum class TokenType {
  // Keywords
  LET,
  PRINT,
  INPUT,
  END,
  REM,
  GOTO,
  IF,
  THEN,
  RUN,
  LIST,
  CLEAR,
  QUIT,
  HELP,
  // Basic lexical units
  IDENTIFIER,
  NUMBER,
  REMINFO,
  // Operators and symbols
  PLUS,
  MINUS,
  MUL,
  DIV,
  EQUAL,
  GREATER,
  LESS,
  LEFT_PAREN,
  RIGHT_PAREN,
  COMMA,
  // Reserved placeholder
  UNKNOWN
};

struct Token {
  TokenType type{TokenType::UNKNOWN};
  std::string text{};
  int column{0};
};

class TokenStream {
 public:
  TokenStream() = default;
  explicit TokenStream(std::vector<Token>&& tokens);

  const Token* peek() const;        //返回当前cursor所在位置的指针  if not ->nullptr
  const Token* get();               //返回当前所在位置指针，并且将光标++
  bool empty() const;
  void reset();

  int position() const;
  int size() const;

  void push(Token&& token);
  const std::vector<Token>& data() const;

 private:
  std::vector<Token> tokens_{};     //切割后用于储存一个个词的vector
  int cursor_{0};
};
