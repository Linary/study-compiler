//
// Created by 李凝瑞 on 2021/12/17.
//

#include "SimpleLexer.h"

#include <memory>

namespace {

    // 是否为字母
    bool isAlpha(char ch) {
        return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == '_');
    }

    // 是否为数字
    bool isDigit(char ch) { return ch >= '0' && ch <= '9'; }

    //是否是空白字符
    bool isBlank(char ch) { return ch == ' ' || ch == '\t' || ch == '\n'; }

}  // namespace

/**
 * 解析字符串，形成 token
 * 这是一个有限状态自动机，在不同的状态中迁移。
 */
std::unique_ptr<SimpleTokenReader> SimpleLexer::tokenize(const std::string& script) {
    // std::vector<std::unique_ptr<Token>> tokens;
    this->tokens.clear();
    this->tokenText.clear();
    this->token = std::make_unique<SimpleToken>();

    char ch = 0;
    int idx = 0;
    size_t length = script.length();
    DfaState state = DfaState::Initial;
    while (idx < length) {
        ch = script.at(idx);
        switch (state) {
            case DfaState::Initial:
                // 重新确定后续状态
                state = this->initToken(ch);
                break;
            case DfaState::Id:
                if (isAlpha(ch) || isDigit(ch)) {
                    // 保持标识符状态
                    this->tokenText += ch;
                } else {
                    // 退出标识符状态，并保存 token
                    state = this->initToken(ch);
                }
                break;
            case DfaState::GT:
                if (ch == '=') {
                    // 转换成GE
                    this->token->type() = TokenType::GE;
                    state = DfaState::GE;
                    this->tokenText += ch;
                } else {
                    // 退出GT状态，并保存Token
                    state = this->initToken(ch);
                }
                break;
            case DfaState::GE:
            case DfaState::Assignment:
            case DfaState::Plus:
            case DfaState::Minus:
            case DfaState::Star:
            case DfaState::Slash:
            case DfaState::SemiColon:
            case DfaState::LeftParen:
            case DfaState::RightParen:
                // 退出当前状态，并保存Token
                state = this->initToken(ch);
                break;
            case DfaState::IntLiteral:
                if (isDigit(ch)) {
                    // 继续保持在数字字面量状态
                    this->tokenText += ch;
                } else {
                    // 退出当前状态，并保存Token
                    state = this->initToken(ch);
                }
                break;
            case DfaState::Id_int1:
                if (ch == 'n') {
                    state = DfaState::Id_int2;
                    this->tokenText += ch;
                } else if (isDigit(ch) || isAlpha(ch)) {
                    // 切换回Id状态
                    state = DfaState::Id;
                    this->tokenText += ch;
                } else {
                    state = this->initToken(ch);
                }
                break;
            case DfaState::Id_int2:
                if (ch == 't') {
                    state = DfaState::Id_int3;
                    this->tokenText += ch;
                } else if (isDigit(ch) || isAlpha(ch)) {
                    // 切换回id状态
                    state = DfaState::Id;
                    this->tokenText += ch;
                } else {
                    state = this->initToken(ch);
                }
                break;
            case DfaState::Id_int3:
                if (isBlank(ch)) {
                    this->token->type() = TokenType::Int;
                    state = this->initToken(ch);
                } else {
                    // 切换回Id状态
                    state = DfaState::Id;
                    this->tokenText += ch;
                }
                break;
            default:
                break;
        }
        ++idx;
    }
    // 把最后一个 token 送进去
    if (this->tokenText.length() > 0) {
        this->initToken(ch);
    }
    auto rtn = std::make_unique<SimpleTokenReader>(this->tokens);
    return std::move(rtn);
}

/**
 * 打印所有的 token
 */
void SimpleLexer::dump(SimpleTokenReader& tokenReader) {
    std::cout << "text\t\ttype" << std::endl;
    const Token* vToken = tokenReader.read();
    while (vToken != nullptr) {
        std::cout << vToken->text() + "\t\t\t" << vToken->type() << std::endl;
        vToken = tokenReader.read();
    }
    std::cout << "=========================" << std::endl;
}

/**
 * 有限状态机进入初始状态。这个初始状态其实并不做停留，它马上进入其他状态。
 * 开始解析的时候，进入初始状态；某个Token解析完毕，也进入初始状态，在这里把Token记下来，然后建立一个新的Token。
 */
DfaState SimpleLexer::initToken(char ch) {
    if (this->tokenText.length() > 0) {
        // 保存当前的 token
        this->token->text() = this->tokenText;
        // 为什么这里必须用std::move呢？不用连编译都过不去
        this->tokens.emplace_back(std::move(this->token));

        this->tokenText.clear();
        this->token = std::make_unique<SimpleToken>();
    }

    DfaState newState;
    if (isAlpha(ch)) {
        // 第一个字符是字母
        if (ch == 'i') {
            // 处于 int 的第一阶段
            newState = DfaState::Id_int1;
        } else {
            // 进入标识符状态
            newState = DfaState::Id;
        }
        // 注意：这里 tokenType 先记录为标识符了，但是 newState 有可能仍然只是 Id_int1
        this->token->type() = TokenType::Identifier;
        this->tokenText += ch;
    } else if (isDigit(ch)) {
        // 第一个字符是数字
        newState = DfaState::IntLiteral;
        this->token->type() = TokenType::IntLiteral;
        this->tokenText += ch;
    } else if (ch == '>') {
        // 第一个字符是 >
        newState = DfaState::GT;
        this->token->type() = TokenType::GT;
        this->tokenText += ch;
    } else if (ch == '+') {
        newState = DfaState::Plus;
        this->token->type() = TokenType::Plus;
        this->tokenText += ch;
    } else if (ch == '-') {
        newState = DfaState::Minus;
        this->token->type() = TokenType::Minus;
        this->tokenText += ch;
    } else if (ch == '*') {
        newState = DfaState::Star;
        this->token->type() = TokenType::Star;
        this->tokenText += ch;
    } else if (ch == '/') {
        newState = DfaState::Slash;
        this->token->type() = TokenType::Slash;
        this->tokenText += ch;
    } else if (ch == ';') {
        newState = DfaState::SemiColon;
        this->token->type() = TokenType::SemiColon;
        this->tokenText += ch;
    } else if (ch == '(') {
        newState = DfaState::LeftParen;
        this->token->type() = TokenType::LeftParen;
        this->tokenText += ch;
    } else if (ch == ')') {
        newState = DfaState::RightParen;
        this->token->type() = TokenType::RightParen;
        this->tokenText += ch;
    } else if (ch == '=') {
        newState = DfaState::Assignment;
        this->token->type() = TokenType::Assignment;
        this->tokenText += ch;
    } else {
        // skip all unknown patterns
        newState = DfaState::Initial;
    }
    return newState;
}
