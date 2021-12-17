//
// Created by 李凝瑞 on 2021/12/17.
//

#include "SimpleLexer.h"

inline void testLexer(const std::string& script) {
    std::cout << "parse: " << script << std::endl;
    std::unique_ptr<SimpleLexer> lexer = std::make_unique<SimpleLexer>();
    lexer->dump(*(lexer->tokenize(script)));
}

int main(int argc, char *argv[]) {
    testLexer("int age = 45;");
    testLexer("intx age = 45;");
    testLexer("in age = 45;");
    testLexer("age >= 45;");
    testLexer("age > 45;");
}