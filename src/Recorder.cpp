// TODO: Imply interfaces declared in the Recorder.hpp.
#include "Recorder.hpp"
#include <iostream>
#include <algorithm>

 Recorder::~Recorder() {
   clear();
 }

void Recorder::add(int line, Statement* stmt) {
   if (hasLine(line)) {
     remove(line);
   }
   // std::cout << "root add" << std::endl;
   statements_[line] = stmt;
   // for (auto it : statements_) {
   //   std::cout  << it.second->text() << std::endl;
   // }
 }

void Recorder::remove(int line) {
   auto it = statements_.find(line);
   if (it != statements_.end()) {    //双重释放？ !=
     delete it->second;
     statements_.erase(it);
   }
 }

const Statement* Recorder::get(int line) const noexcept {
   if (hasLine(line)) {
     return statements_.find(line)->second;
   }
   return nullptr;
 }

bool Recorder::hasLine(int line) const noexcept {
   auto it = statements_.find(line);
   return (it != statements_.end());
 }

void Recorder::clear() noexcept {
   for (auto& i : statements_) {
     delete i.second;
   }
   statements_.clear();
 }

void Recorder::printLines() const {
   // std::cout << "root print" << std::endl;
   // std::cout << it.first << " " << it.second->text() << '\n';
   for (auto it : statements_) {
   // std::cout << it.first <<  '\n';
     if (it.second == nullptr) {
       // std::cout << "false" << std::endl;
       return;
     }
   std::cout  << it.second->text() << std::endl;
    // std::cout <<  it.second->text() << '\n';                   //还没找到输出的格式要求
   }
   // std::vector<int> lines;
   // for (const auto& pair : statements_) {
   //   lines.push_back(pair.first);
   // }
   // std::sort(lines.begin(), lines.end());
   //
   // // 输出每行内容：行号 + 语句文本
   // for (int line : lines) {
   //   const Statement* stmt = statements_.at(line);
   //   std::cout << line << " " << stmt->text() << std::endl;
   // }
 }

int Recorder::nextLine(int line) const noexcept {
   auto it = statements_.upper_bound(line);
   if (it != statements_.end()) {
     return it->first;
   }
   return -1;
 }

std::vector<int> Recorder::getAllLines() const {
   std::vector<int> lines;
   for (const auto& pair : statements_) {
     lines.push_back(pair.first);
   }
   return lines;
 }