//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_TOKENREADER_H
#define STUDY_COMPILER_TOKENREADER_H

#include "Token.h"

class TokenReader {
public:
    /**
     * 返回token流中下一个token，并从流中取出。如果流已经为空，返回null;
     */
    virtual const Token* read() = 0;

    /**
     * 返回token流中下一个token，但不从流中取出。如果流已经为空，返回null;
     */
    virtual const Token* peek() const = 0;

    /**
     * token流回退一步，恢复原来的token。
     */
    virtual void unread() = 0;

    /**
     * 获取token流当前的读取位置。
     */
    virtual int getPosition() const = 0;

    /**
     * 设置token流当前的读取位置
     */
    virtual void setPosition(int position) = 0;

    virtual ~TokenReader() = default;
};

#endif //STUDY_COMPILER_TOKENREADER_H
