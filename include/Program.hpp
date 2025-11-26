#pragma once

#include <memory>

#include "Recorder.hpp"
#include "VarState.hpp"

class Statement;

class Program {
 public:
  Program();

  void addStmt(int line, Statement* stmt);
  void removeStmt(int line);

  void run();
  void list() const;
  void clear();

  void execute(const Statement* stmt);

  int getPC() const noexcept;
  void changePC(int line);
  void programEnd();

 private:
  Recorder recorder_;
  VarState vars_;
  int programCounter_;   //程序运行到第几行
  bool programEnd_;      //判断程序是否结束

  void resetAfterRun() noexcept;
};
