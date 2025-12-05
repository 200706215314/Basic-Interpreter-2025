#include "VarState.hpp"

#include <algorithm>

#include "utils/Error.hpp"

VarState::VarState() {
  scopes_.push_back(std::unordered_map<std::string, int>());
}

void VarState::setValue(const std::string& name, int value) {
  setInCurrentScope(name, value);
}

int VarState::getValue(const std::string& name) const {
  const int* value = findVariable(name);
  if (value == nullptr) {
    throw BasicError("VARIABLE NOT DEFINED");
  }
  return *value;
}
void VarState::indent() {
  scopes_.push_back(std::unordered_map<std::string, int>());
}

void VarState::dedent() {
  if (scopes_.size() <= 1) {
    throw BasicError("SCOPE UNDERFLOW");
  }
  scopes_.pop_back();
}

int VarState::getCurrentScopeLevel() const {
  return scopes_.size() - 1;
}

void VarState::setInCurrentScope(const std::string& name, const int value) {
  if (scopes_.empty()) {
    scopes_.push_back(std::unordered_map<std::string, int>());
  }
  scopes_.back()[name] = value;
}


const int* VarState::findVariable(const std::string& name) const {
  // 从当前作用域向外查找变量 - 与之前的 findVariable 逻辑相同
  for (int i = scopes_.size() - 1; i >= 0; --i) {
    auto it = scopes_[i].find(name);
    if (it != scopes_[i].end()) {
      return &(it->second);
    }
  }
  return nullptr;
}

void VarState::clear() { scopes_.clear(); }
