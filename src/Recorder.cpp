// TODO: Imply interfaces declared in the Recorder.hpp.
#include "Recorder.hpp"
#include <iostream>

 Recorder::~Recorder() {
   clear();
 }

void Recorder::add(int line, Statement* stmt) {
   if (hasLine(line)) {
     remove(line);
   }
   statements_[line] = stmt;
 }

void Recorder::remove(int line) {
   auto it = statements_.find(line);
   if (it != statements_.end()) {
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
   for (auto it : statements_) {
     std::cout << it.first << " " << it.second->text() << '\n';                   //还没找到输出的格式要求
   }
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







