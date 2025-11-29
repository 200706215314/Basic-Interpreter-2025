#pragma once

#include <memory>
#include <string>
#include <iostream>
#include "Expression.hpp"

class Program;
class VarState;

class Statement {
 public:
  explicit Statement(std::string source);
  virtual ~Statement() = default;

  virtual void execute(VarState& state, Program& program) const = 0;

  const std::string& text() const noexcept;

 private:
  std::string source_;
};

// TODO: Other statement types derived from Statement, e.g., GOTOStatement,
// LetStatement, etc.
class LetStatement : public Statement {
public:
  LetStatement(std::string varName, Expression* expr, std::string source);
  ~LetStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
  std::string varName_;
  Expression* expr_;
};

class PrintStatement: public Statement {
public:
  PrintStatement(std::string source, Expression* expr);
  ~PrintStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
  Expression* expr_;
};

class InputStatement: public Statement {
public:
  InputStatement(std::string varName, std::string source);
  ~InputStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
  std::string varName_;
  static bool IsValid(std::string s, int &ans) {
    if (s.empty()) {
      return false;
    }

    bool flag = false;
    int len = s.length(), index = 0;
    char ch = s[index];
    // std::cout << ch;
    while (ch == ' ') {
      index++;
      ch = s[index];
    }
    if (ch == '-') {
      index++;
      ch = s[index];
      flag = true;
    }
    while (ch != ' ') {
      if (isdigit(ch)) {
        // std::cout << "isdigit";
        // std::cout << ch;
        ans = (ans * 10 + (ch - '0'));
        index++;
        if (index == len) {
          if (flag) {
            ans *= -1;
          }
          return true;
        }
        ch = s[index];
      }
      else {
        return false;
      }
    }
  return false;
  }
};

class GotoStatement: public Statement {
public:
  GotoStatement(int line, std::string source);
  ~GotoStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
  int line_;
  bool newline;
};

class IFStatement: public Statement {
public:
  IFStatement(int line, std::string source, Expression* expr1, Expression* expr2, char op);
  ~IFStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
  int line_;
  Expression* left_;
  Expression* right_;
  char op_;
};

class RemStatement: public Statement {
public:
  RemStatement(std::string source, std::string comment);
  ~RemStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
  std::string comment_;
};

class EndStatement: public Statement {
public:
  explicit EndStatement(std::string source);
  ~EndStatement() override;
  void execute(VarState& state, Program& program) const override;

};


class IndentStatement: public Statement {
public:
  explicit IndentStatement(std::string source);
  ~IndentStatement()override;
  void execute(VarState& state, Program& program) const override;

};

class DedentStatement: public Statement {
public:
  explicit DedentStatement(std::string source);
  ~DedentStatement()override;
  void execute(VarState& state, Program& program) const override;

};