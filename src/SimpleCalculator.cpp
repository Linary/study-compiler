//
// Created by 李凝瑞 on 2021/12/17.
//

#include <iostream>
#include "SimpleCalculator.h"
#include "SimpleLexer.h"

namespace {
    inline bool checkTokenType(const Token* ptoken, const TokenType type) {
        return (ptoken != nullptr) && (ptoken->type() == type);
    }

    template <typename T>
    T* logIfNullAndReturn(T* ptr, const std::string& msg) {
        if (ptr == nullptr) {
            VLOG(1) << "NOT found: " << msg;
        } else {
            VLOG(1) << "Found: " << msg;
        }
        return ptr;
    }
}  // namespace

/**
 * 执行脚本，并打印输出AST和求值过程。
 */
void SimpleCalculator::evaluate(const std::string& script) {
    try {
        std::unique_ptr<SimpleAST> tree = this->parse(script);
        this->dumpAST(tree->rootNode(), "");
        this->evaluate(tree->rootNode(), "");
    } catch (std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
        return;
    }
}

/**
 * 解析脚本，并返回根节点
 */
std::unique_ptr<SimpleAST> SimpleCalculator::parse(const std::string& script) {
    std::unique_ptr<SimpleLexer> lexer = std::make_unique<SimpleLexer>();
    auto tokenReaderPtr = lexer->tokenize(script);
    TokenReader* tokenReader = tokenReaderPtr.get();
    ASTNode* rootNode = this->parseProgram(tokenReader);
    return this->astBuilder_.build(rootNode);
}

/**
 * 对某个AST节点求值，并打印求值过程。
 * @param indent  打印输出时的缩进量，用tab控制
 */
int SimpleCalculator::evaluate(ASTNode* node, const std::string& indent) {
    int result = 0;
    std::cout << indent + "calculating: " + ASTNodeTypeToString(node->getType()) << std::endl;
    ASTNode* child1;
    ASTNode* child2;
    switch (node->getType()) {
        case ASTNodeType::Program:
            for (auto child : node->getChildren()) {
                result = this->evaluate(child, indent + "\t");
            }
            break;
        case ASTNodeType::Additive: {
            child1 = node->getChildren()[0];
            int value1 = this->evaluate(child1, indent + "\t");
            child2 = node->getChildren()[1];
            int value2 = this->evaluate(child2, indent + "\t");
            if (node->getText() == "+") {
                result = value1 + value2;
            } else {
                result = value1 - value2;
            }
            break;
        }
        case ASTNodeType::Multiplicative: {
            child1 = node->getChildren()[0];
            int value1 = this->evaluate(child1, indent + "\t");
            child2 = node->getChildren()[1];
            int value2 = this->evaluate(child2, indent + "\t");
            if (node->getText() == "*") {
                result = value1 * value2;
            } else {
                result = value1 / value2;
            }
            break;
        }
        case ASTNodeType::IntLiteral:
            result = std::stoi(node->getText());
            break;
        default:
            break;
    }
    std::cout << indent + "result: " << result << std::endl;
    return result;
}

/**
 * 语法解析：根节点
 */
SimpleASTNode* SimpleCalculator::parseProgram(TokenReader* reader) {
    std::unique_ptr<SimpleASTNode> node = std::make_unique<SimpleASTNode>(ASTNodeType::Program, "Calculator");
    SimpleASTNode* child = this->parseIntDeclare(reader);
    if (child == nullptr) {
        child = this->parseAdditive(reader);
    }
    if (child != nullptr) {
        node->addChild(child);
    } else {
        throw std::logic_error("unknown statement");
    }
    //
    auto* ret = dynamic_cast<SimpleASTNode*>(this->astBuilder_.addASTNode(std::move(node)));
    return ret;
}

/**
 * 整型变量声明语句，如：
 * int a;
 * int b = 2*3;
 */
SimpleASTNode* SimpleCalculator::parseIntDeclare(TokenReader* reader) {
    VLOG(1) << "parsing IntDeclare...";
    std::unique_ptr<SimpleASTNode> node;
    const Token* ptoken = reader->peek();
    if (checkTokenType(ptoken, TokenType::Int)) {
        VLOG(2) << "  see 'int' ...";
        // get int
        reader->read();
        if (reader->peek()->type() == TokenType::Identifier) {
            ptoken = reader->read();
            VLOG(2) << "  see 'identifier': " << ptoken->text();
            node = absl::WrapUnique(new SimpleASTNode(ASTNodeType::IntDeclaration, ptoken->text()));
            ptoken = reader->peek();
            if (checkTokenType(ptoken, TokenType::Assignment)) {
                // get the "="
                reader->read();
                VLOG(2) << "  see '=' ...";
                SimpleASTNode* child = this->parseAdditive(reader);
                if (child == nullptr) {
                    throw std::logic_error("invalide variable initialization, expecting an expression");
                }
                node->addChild(child);
            }
        } else {
            throw std::logic_error("variable name expected");
        }

        if (node) {
            ptoken = reader->peek();
            if (checkTokenType(ptoken, TokenType::SemiColon)) {
                reader->read();
            } else {
                throw std::logic_error("invalid statement, expecting semicolon");
            }
        }
    }
    auto ret = dynamic_cast<SimpleASTNode*>(this->astBuilder_.addASTNode(std::move(node)));
    return ret;
}

/**
 * 语法解析：加法表达式
 */
SimpleASTNode* SimpleCalculator::parseAdditive(TokenReader* reader) {
    VLOG(1) << "parsing Additive...";
    SimpleASTNode* child1 = this->parseMultiplicative(reader);
    // SimpleASTnode* node = child1;
    const Token* ptoken = reader->peek();
    if (child1 != nullptr && ptoken != nullptr) {
        if (checkTokenType(ptoken, TokenType::Plus) || checkTokenType(ptoken, TokenType::Minus)) {
            // get the "+" or "-"
            ptoken = reader->read();
            SimpleASTNode* child2 = this->parseAdditive(reader);
            if (child2 == nullptr) {
                throw std::logic_error("invalid additive expression, expecting the right part.");
            }
            std::unique_ptr<SimpleASTNode> add_node = std::make_unique<SimpleASTNode>(
                                                      ASTNodeType::Additive, ptoken->text());
            // Attention, new ast node on the top to ensure the correctness
            add_node->addChild(child1);
            add_node->addChild(child2);
            child1 = dynamic_cast<SimpleASTNode*>(this->astBuilder_.addASTNode(std::move(add_node)));
        }
    }
    return logIfNullAndReturn(child1, "Additive");
}

/**
 * 语法解析：乘法表达式
 */
SimpleASTNode* SimpleCalculator::parseMultiplicative(TokenReader* reader) {
    VLOG(1) << "parsing Multiplicative...";
    SimpleASTNode* child1 = this->parsePrimitive(reader);
    if (child1 != nullptr) {
        const Token* ptoken = reader->peek();
        if (checkTokenType(ptoken, TokenType::Star) ||
            checkTokenType(ptoken, TokenType::Slash)) {
            // take the "*" or "/"
            ptoken = reader->read();
            VLOG(2) << "  got " << ptoken->text();
            SimpleASTNode* child2 = this->parsePrimitive(reader);
            if (child2 == nullptr) {
                throw std::logic_error(
                        "invalid multiplicative expression, expecting the right part.");
            }
            std::unique_ptr<SimpleASTNode> mul_node = std::make_unique<SimpleASTNode>(
                                                      ASTNodeType::Multiplicative, ptoken->text());
            mul_node->addChild(child1);
            mul_node->addChild(child2);
            child1 = dynamic_cast<SimpleASTNode*>(this->astBuilder_.addASTNode(std::move(mul_node)));
        }
    }
    return logIfNullAndReturn(child1, "Multiplicative");
}

/**
 * 语法解析：基础表达式
 */
SimpleASTNode* SimpleCalculator::parsePrimitive(TokenReader* reader) {
    VLOG(1) << "parsing Primary...";
    SimpleASTNode* node = nullptr;
    // std::unique_ptr<SimpleASTNode> node;
    const Token* ptoken = reader->peek();
    if (checkTokenType(ptoken, TokenType::IntLiteral)) {
        ptoken = reader->read();
        VLOG(2) << "  got IntLiteral: " << ptoken->text();
        auto pri_node = absl::WrapUnique(new SimpleASTNode(ASTNodeType::IntLiteral, ptoken->text()));
        node = dynamic_cast<SimpleASTNode*>(this->astBuilder_.addASTNode(std::move(pri_node)));
    } else if (checkTokenType(ptoken, TokenType::Identifier)) {
        ptoken = reader->read();
        VLOG(2) << "  got Identifier: " << ptoken->text();
        auto pri_node = absl::WrapUnique(new SimpleASTNode(ASTNodeType::Identifier, ptoken->text()));
        node = dynamic_cast<SimpleASTNode*>(this->astBuilder_.addASTNode(std::move(pri_node)));
    } else if (checkTokenType(ptoken, TokenType::LeftParen)) {
        // eat the "("
        ptoken = reader->read();
        VLOG(2) << "  got LeftParen: " << ptoken->text();
        node = this->parseAdditive(reader);
        if (node == nullptr) {
            throw std::logic_error("expecting an additive expression inside parenthesis");
        }
        ptoken = reader->peek();
        if (!checkTokenType(ptoken, TokenType::RightParen)) {
            throw std::logic_error("expecting right parenthesis");
        }
        // eat the ")"
        ptoken = reader->read();
        VLOG(2) << "  got RightParen: " << ptoken->text();
    }
    return logIfNullAndReturn(node, "Primary");
}

void SimpleCalculator::dumpAST(const ASTNode* node, const std::string& indent) const {
    std::cout << indent
              << ASTNodeTypeToString(node->getType()) + " " + node->getText()
              << std::endl;
    for (auto& child : node->getChildren()) {
        this->dumpAST(child, indent + "\t");
    }
}
