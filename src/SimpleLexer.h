//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_SIMPLELEXER_H
#define STUDY_COMPILER_SIMPLELEXER_H

#include <string>
#include <vector>
#include "SimpleTokenReader.h"
#include "SimpleToken.h"
#include "DfaState.h"

/**
 * 一个简单的词法分析器
 */
class SimpleLexer {
public:
    /**
     * 解析字符串，形成 token。这是词法解析的入口函数
     * 这是一个有限状态自动机，在不同的状态中迁移。
     */
    std::unique_ptr<SimpleTokenReader> tokenize(const std::string& script);

    /**
     * 打印所有的 token
     */
    void dump(SimpleTokenReader& tokenReader);

protected:
    /**
     * 有限状态机进入初始状态。这个初始状态其实并不做停留，它马上会进入其他状态。
     * 开始解析的时候，进入初始状态；某个Token解析完毕，也进入初始状态，在这里把Token记下来，然后建立一个新的Token。
     */
    DfaState initToken(char ch);

private:
    // 当前正在解析的 token
    std::unique_ptr<Token> token;

    // 临时保存 token 的文本
    std::string tokenText;

    // 保存解析出来的 token
    std::vector<std::unique_ptr<Token>> tokens;
};

#endif //STUDY_COMPILER_SIMPLELEXER_H
