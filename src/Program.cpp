// TODO: Imply interfaces declared in the Program.hpp.
#include "Program.hpp"

#include <algorithm>
#include <iostream>

#include "utils/Error.hpp"

Program::Program():programCounter_(-1),programEnd_(false) {}

void Program::addStmt(int line, Statement* stmt) {
   recorder_.add(line, stmt);
 }


void Program::changePC(int line) {
   // 强制改变 PC，用于 GOTO/IF
   // auto lines = recorder_.getAllLines();
   // std::sort(lines.begin(), lines.end());
   // auto it = std::find(lines.begin(), lines.end(), line);
   // if (it != lines.end()) {
   //   programCounter_ = std::distance(lines.begin(), it);
   // }
   // else {
   //   throw BasicError("LINE NUMBER ERROR");
   // }
  auto lines = recorder_.getAllLines();
  std::sort(lines.begin(), lines.end());  // 基于排序后的行号查找
  auto it = std::find(lines.begin(), lines.end(), line);
  if (it != lines.end()) {
    programCounter_ = std::distance(lines.begin(), it);  // 定位到目标行索引
  } else {
    throw BasicError("LINE NUMBER ERROR");
  }
 }

void Program::clear() {
   recorder_.clear();
   vars_.clear();
 }

void Program::execute(const Statement* stmt) {
  if (stmt == nullptr) return;
   stmt->execute(vars_, *this);
 }

int Program::getPC() const noexcept {
  if (programCounter_ < 0 || programCounter_ >= static_cast<int>(recorder_.getAllLines().size())) {
    return -1;
  }
  auto lines = recorder_.getAllLines();
  std::sort(lines.begin(), lines.end());
  return lines[programCounter_];  // 返回实际行号
 }

void Program::list() const {
   // auto lines = recorder_.getAllLines();
   // std::sort(lines.begin(), lines.end());

   // for (auto i : lines) {
   //   auto it = recorder_.get(i);
   //   std::cout << it->text() << '\n';             //格式待定
   // }
  recorder_.printLines();
 }

void Program::programEnd() {
   programEnd_ = true;
 }

void Program::removeStmt(int line) {
   recorder_.remove(line);
 }

void Program::resetAfterRun() noexcept {
  programCounter_ = -1;
  programEnd_ = false;
 }

void Program::run() {
   // 按行号升序执行程序，支持 GOTO/IF 改变 PC。
  programCounter_ = 0;
  auto lines = recorder_.getAllLines();
   std::sort(lines.begin(), lines.end());
  try {
    while (!programEnd_ && programCounter_ < static_cast<int>(lines.size())) {
      const int currentLine = lines[programCounter_];
      if (const auto stmt = recorder_.get(currentLine)) {
        execute(stmt);
        // std::cout << "execute";
        if (!programEnd_ && new_line) {
          // std::cout << "plus" << std::endl;
          programCounter_++;
        }
        new_line = true;
      }

    }
  } catch (const BasicError& error) {
    std::cout << error.what() << std::endl;
    resetAfterRun();
    // throw;
  } catch (const std::exception& e) {
    std::cout << "RUNTIME ERROR: " << e.what() << std::endl;
    resetAfterRun();
    // throw BasicError("RUNTIME ERROR");
  }

  resetAfterRun();
 }