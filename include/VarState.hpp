#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

class VarState {
 public:
  VarState();
  void setValue(const std::string& name, int value);
  int getValue(const std::string& name) const;
  void clear();
  void indent();
  void dedent();
  int getCurrentScopeLevel() const;

 private:
  std::vector<std::unordered_map<std::string, int>> scopes_;

  void setInCurrentScope(const std::string& name, int value);

  const int* findVariable(const std::string& name) const;

};
