#include "Statement.hpp"

#include <iostream>
#include <limits>
#include <sstream>
#include <utility>

#include "Program.hpp"
#include "VarState.hpp"
#include "utils/Error.hpp"

Statement::Statement(std::string source) : source_(std::move(source)) {}

const std::string& Statement::text() const noexcept { return source_; }

// TODO: Imply interfaces declared in the Statement.hpp.

LetStatement::LetStatement(std::string varName, Expression* expr, std::string source)
    : Statement(std::move(source)), varName_(std::move(varName)), expr_(expr) {}

LetStatement::~LetStatement() {
  delete expr_;
}

void LetStatement::execute(VarState& state, Program& program) const {
  state.setValue(varName_, expr_->evaluate(state));
}

PrintStatement::PrintStatement(std::string source, Expression* expr):Statement(std::move(source)) {
  this->expr_ = expr;
}

PrintStatement::~PrintStatement() {
  delete expr_;
}

void PrintStatement::execute(VarState& state, Program& program) const {
  int value = expr_->evaluate(state);
  std::cout << value << '\n';
}

 InputStatement::InputStatement(std::string varName, std::string source) : Statement(std::move(source)), varName_(std::move(varName)) { }

 InputStatement::~InputStatement() = default;

void InputStatement::execute(VarState& state, Program& program) const {
  int value = 0;
  std::string s;
  std::cout << " ? ";
  getline(std::cin, s);
  // std::cout << s;
  // std::cin >> value;
  //没判断输入是否合法
  // if (std::cin.fail()) {
  //   std::cin.clear();
  //   std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  //   throw BasicError("INVALID NUMBER");
  //   std::cout << std::endl;
  //   std::cout << " ? ";
  // }
  while (!InputStatement::IsValid(s, value)) {
    value = 0;
    s.clear();
    std::cout << "INVALID NUMBER" << std::endl;
    std::cout << " ? ";
    getline(std::cin, s);
  }
  // std::cout << value;
  state.setValue(varName_, value);
}

 GotoStatement::GotoStatement(const int line, std::string source) : Statement(std::move(source)), line_(line) {};

 GotoStatement::~GotoStatement() = default ;

void GotoStatement::execute(VarState& state, Program& program) const {
  //此语句将无条件地将控制权转移至程序中的第 n 行。如果第 n 行不存在，基本解释程序应生成一条错误消息，通知用户该行不存在
  // std::cout << "change PC";
  program.changePC(line_);
}

IFStatement::IFStatement(const int line, std::string source, Expression* expr1,
                         Expression* expr2, const char op) : Statement(std::move(source)), line_(line), left_(expr1), right_(expr2), op_(op) {}

 IFStatement::~IFStatement() {
  delete left_;
  delete right_;
}


void IFStatement::execute(VarState& state, Program& program) const {
  const int leftVal = left_->evaluate(state);
  const int rightVal = right_->evaluate(state);

  bool condition = false;
  switch (op_) {
    case '=':
      condition = (leftVal == rightVal);
      break;
    case '>':
      condition = (leftVal > rightVal);
      break;
    case '<':
      condition = (leftVal < rightVal);
      break;
    default:
      throw BasicError("UNSUPPORTED OPERATOR");     //文件与expression中对不上
  }

  if (condition) {
    program.changePC(line_);
  }
}

 RemStatement::RemStatement(std::string source, std::string comment) : Statement(std::move(source)), comment_(std::move(comment)){};

 RemStatement::~RemStatement() = default;

void RemStatement::execute(VarState& state, Program& program) const {
  //只起到注释作用，不执行任何操作
}

 EndStatement::EndStatement(std::string source) :Statement(std::move(source)){  }


 EndStatement::~EndStatement() = default;

void EndStatement::execute(VarState& state, Program& program) const {
  program.programEnd();
}



