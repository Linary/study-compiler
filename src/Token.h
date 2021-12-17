//
// Created by 李凝瑞 on 2021/12/17.
//

#ifndef STUDY_COMPILER_TOKEN_H
#define STUDY_COMPILER_TOKEN_H

#include <string>
#include "TokenType.h"

class Token {
public:
    // 获取 token 的类型
    virtual TokenType& type() = 0;
    virtual const TokenType& type() const = 0;

    // 获取 token 的文本值
    virtual std::string& text() = 0;
    virtual const std::string& text() const = 0;
};

#endif //STUDY_COMPILER_TOKEN_H
