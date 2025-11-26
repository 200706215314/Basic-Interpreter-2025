#include <iostream>
#include <memory>
#include <string>

#include "Lexer.hpp"
#include "Parser.hpp"
#include "Program.hpp"
#include "Token.hpp"
#include "utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;

  std::string line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // TODO: The main function.
      TokenStream tokens = lexer.tokenize(line);
      const Token* start = tokens.peek();
      if (start && start->type == TokenType::IDENTIFIER) {
        std::string command = start->text;

        if (command == "RUN") {
          program.run();
        }
        if (command == "LIST") {
          program.list();
        }
        if (command == "CLEAR") {
          program.clear();
        }
        if (command == "QUIT") {
          break;
        }
        if (command == "HELP") {  //?格式
          std::cout << "BASIC Interpreter Commands:\n";
          std::cout << "RUN    - Execute the program\n";
          std::cout << "LIST   - List all program lines\n";
          std::cout << "CLEAR  - Clear program and variables\n";
          std::cout << "QUIT   - Exit the interpreter\n";
          std::cout << "HELP   - Show this help message\n";
          std::cout << "Or enter BASIC statements with/without line numbers\n";
        }
      }
      else {

      }
    } catch (const BasicError& e) {
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}