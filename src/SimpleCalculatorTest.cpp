//
// Created by 李凝瑞 on 2021/12/17.
//

#include "SimpleLexer.h"
#include "SimpleCalculator.h"

int main(int argc, char *argv[]) {
    std::unique_ptr<SimpleCalculator> calculator = std::make_unique<SimpleCalculator>();

    // 测试变量声明语句的解析
    std::string script = "int a = b+3;";
    std::cout << "parsing variable declaration statement: " << script << std::endl;
    try {
        auto ast = calculator->parse(script);
        calculator->dumpAST(ast->rootNode(), std::string(""));
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }

    // 测试表达式
    script = "2+3*5";
    std::cout << "\n计算: " + script + "，看上去一切正常。" << std::endl;
    calculator->evaluate(script);

    // 测试语法错误
    script = "2+";
    std::cout << "\n: " + script + "，应该有语法错误。" << std::endl;
    calculator->evaluate(script);

    script = "2+3+4";
    std::cout << "\n计算: " + script + "，结合性出现错误。" << std::endl;
    calculator->evaluate(script);

    return 0;
}