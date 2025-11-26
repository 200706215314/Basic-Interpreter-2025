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

      // 检查第一个 token 的类型
      const Token* firstToken = tokens.peek();
      if (!firstToken) {
        continue;
      }

      // 如果是标识符，可能是解释器命令`
      if (firstToken->type == TokenType::IDENTIFIER) {
        std::string command = firstToken->text;

        if (command == "RUN") {
          program.run();
        } else if (command == "LIST") {
          program.list();
        } else if (command == "CLEAR") {
          program.clear();
        } else if (command == "QUIT") {
          break;
        } else if (command == "HELP") {
          std::cout << "BASIC Interpreter Commands:\n";
          std::cout << "RUN    - Execute the program\n";
          std::cout << "LIST   - List all program lines\n";
          std::cout << "CLEAR  - Clear program and variables\n";
          std::cout << "QUIT   - Exit the interpreter\n";
          std::cout << "HELP   - Show this help message\n";
          std::cout << "Or enter BASIC statements with/without line numbers\n";
        } else {
          // 不是命令，尝试解析为立即执行语句
          ParsedLine parsed = parser.parseLine(tokens, line);
          if (parsed.getStatement()) {
            // 立即执行语句
            program.execute(parsed.getStatement());
            // 立即执行语句不需要保存，所以删除它
            delete parsed.fetchStatement();
          }
        }
      } else if (firstToken->type == TokenType::NUMBER ||
                 firstToken->type == TokenType::REMINFO ||
                 firstToken->type == TokenType::LET ||
                 firstToken->type == TokenType::PRINT ||
                 firstToken->type == TokenType::INPUT ||
                 firstToken->type == TokenType::END ||
                 firstToken->type == TokenType::GOTO ||
                 firstToken->type == TokenType::IF ||
                 firstToken->type == TokenType::THEN) {
        // 这些可能是程序语句或立即执行语句
        ParsedLine parsed = parser.parseLine(tokens, line);

        if (parsed.getLine().has_value()) {
          // 有行号的情况
          int lineNumber = parsed.getLine().value();

          if (parsed.getStatement() == nullptr) {
            // 只有行号，没有语句 -> 删除该行
            program.removeStmt(lineNumber);
          } else {
            // 有行号和语句 -> 添加或替换该行
            program.addStmt(lineNumber, parsed.fetchStatement());
          }
        } else {
          // 没有行号，立即执行语句
          if (parsed.getStatement()) {
            program.execute(parsed.getStatement());
            // 立即执行语句不需要保存，所以删除它
            delete parsed.fetchStatement();
          }
        }
      } else {
        // 其他情况，尝试解析
        ParsedLine parsed = parser.parseLine(tokens, line);
        if (parsed.getStatement()) {
          if (parsed.getLine().has_value()) {
            // 有行号，添加到程序
            program.addStmt(parsed.getLine().value(), parsed.fetchStatement());
          } else {
            // 没有行号，立即执行
            program.execute(parsed.getStatement());
            delete parsed.fetchStatement();
          }
        }
      }
    } catch (const BasicError& e) {
      std::cout << e.what() << "\n";
    } catch (const std::exception& e) {
      std::cout << "ERROR: " << e.what() << "\n";
    }
  }
  return 0;
}