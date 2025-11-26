#pragma once

#include <memory>
#include <string>

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
};

class IfStatement: public Statement {
public:
  IfStatement(std::string source, Expression* expr);
  ~IfStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
};

class GotoStatement: public Statement {
public:
  GotoStatement(int line, std::string source);
  ~GotoStatement() override;
  void execute(VarState& state, Program& program) const override;

private:
  int line_;
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
  RemStatement(std::string source, std::string  comment);
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
