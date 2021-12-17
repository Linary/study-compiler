//
// Created by 李凝瑞 on 2021/12/17.
//

#ifndef STUDY_COMPILER_SIMPLETOKEN_H
#define STUDY_COMPILER_SIMPLETOKEN_H

#include "Token.h"
#include "TokenType.h"

/**
 * Token的一个简单实现，只有类型和文本值两个属性。
 */
class SimpleToken : public Token {
public:
    TokenType& type() final {
        return this->type_;
    }

    const TokenType& type() const final {
        return this->type_;
    }

    std::string& text() final {
        return this->text_;
    };

    const std::string& text() const final {
        return this->text_;
    };

private:
    // 注意：字段名不能跟方法名重复
    TokenType type_ = TokenType::Invalid;
    std::string text_;
};

#endif //STUDY_COMPILER_SIMPLETOKEN_H
