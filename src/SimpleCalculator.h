//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_SIMPLECALCULATOR_H
#define STUDY_COMPILER_SIMPLECALCULATOR_H

#include <string>
#include "TokenReader.h"
#include "ASTNode.h"
#include "SimpleAST.h"
#include "SimpleASTNode.h"

/**
 * 实现一个计算器，但计算的结合性是有问题的。因为它使用了下面的语法规则（为了避免左递归）：
 *
 * additive -> multiplicative | parseMultiplicative + parseAdditive
 * multiplicative -> primary | primary * parseMultiplicative
 *
 * 递归项在右边，会自然的对应右结合。我们真正需要的是左结合。
 */
class SimpleCalculator {
public:

    /**
     * 执行脚本，并打印输出AST和求值过程。
     */
    void evaluate(const std::string& script);

    /**
     * 解析脚本，并返回根节点
     */
    std::unique_ptr<SimpleAST> parse(const std::string& script);

    /**
     * 对某个AST节点求值，并打印求值过程
     */
    int evaluate(ASTNode* node, const std::string& indent);

    /**
     * 语法解析，根节点
     */
    SimpleASTNode* parseProgram(TokenReader* reader);

    /**
     * 语法解析：整型变量声明语句，如：
     * int a;
     * int b = 2 * 3;
     */
    SimpleASTNode* parseIntDeclare(TokenReader* reader);

    /**
     * 语法解析：加法表达式
     */
    SimpleASTNode* parseAdditive(TokenReader* reader);

    /**
     * 语法解析：乘法表达式
     */
    SimpleASTNode* parseMultiplicative(TokenReader* reader);

    /**
     * 语法解析：基础表达式
     */
    SimpleASTNode* parsePrimitive(TokenReader* reader);

    /**
     * 打印抽象语法树
     */
    void dumpAST(const ASTNode* node, const std::string& indent) const;

private:
    SimpleAST::Builder astBuilder_;
};

#endif //STUDY_COMPILER_SIMPLECALCULATOR_H
