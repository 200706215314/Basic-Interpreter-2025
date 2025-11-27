#include <iostream>
#include <memory>
#include <string>

#include "../include/Lexer.hpp"
#include "../include/Parser.hpp"
#include "../include/Program.hpp"
#include "../include/Token.hpp"
#include "../include/utils/Error.hpp"

int main() {
  Lexer lexer;
  Parser parser;
  Program program;
  std::string line;
  ParsedLine parsed_line;
  while (std::getline(std::cin, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      // TODO: The main function.
      TokenStream tokens = lexer.tokenize(line);
      // 检查第一个 token 的类型
      const Token* firstToken = tokens.peek();
      if (!firstToken) {
        continue; // 空行，跳过
      }

      // 如果是标识符，可能是解释器命令`

        std::string command = firstToken->text;

        if (command == "RUN") {
          program.run();
          continue;
        } else if (command == "LIST") {
          // std::cout <<"in list" << std::endl;
          program.list();
          continue;
        } else if (command == "CLEAR") {
          program.clear();
          continue;
        } else if (command == "QUIT") {
          // std::cout << "in quit" << std::endl;
          break;
        }

      parsed_line = parser.parseLine(tokens, line);

      if (!parsed_line.getLine().has_value()) {
        //没行号，直接执行
        if (parsed_line.getStatement()) {
          // std::cout << "no num has content" << std::endl;
          try {
            program.execute(parsed_line.getStatement());
            // std::cout << "pass" << std::endl;
            delete parsed_line.fetchStatement();
          }
          catch (const BasicError& e) {
            delete parsed_line.fetchStatement();
            throw BasicError(e);
          }

        }
      }
      else {
        // std::cout << "has linenum" << std::endl;
        int line_number = parsed_line.getLine().value();
        // std::cout << line_number << std::endl;
        //行号不规范
        if (line_number <= 0) {
          throw BasicError("INVALID LINE NUMBER");
        }
        //有行号，没内容
        if (!parsed_line.getStatement()) {
          // std::cout << "no content" << std::endl;
          program.removeStmt(line_number);
        }
        else {  // 添加或覆盖
          // std::cout << "is added" << std::endl;
          program.addStmt(line_number, parsed_line.getStatement());
        }
      }
    } catch (const BasicError& e) {
      // std::cout << "in catch" << std::endl;
      std::cout << e.message() << "\n";
    }
  }
  return 0;
}