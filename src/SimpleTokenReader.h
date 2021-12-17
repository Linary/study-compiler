//
// Created by 李凝瑞 on 2021/12/17.
//

#ifndef STUDY_COMPILER_SIMPLETOKENREADER_H
#define STUDY_COMPILER_SIMPLETOKENREADER_H

#include "TokenReader.h"

class SimpleTokenReader : public TokenReader {
public:
    explicit SimpleTokenReader(std::vector<std::unique_ptr<Token>>& tokens) {
        for (auto& token : tokens) {
            this->tokens_.emplace_back(std::move(token));
        }
    }

    const Token * read() override {
        if (isValidPosition(this->pos_)) {
            return this->tokens_[pos_++].get();
        }
        return nullptr;
    }

    const Token * peek() const override {
        if (isValidPosition(this->pos_)) {
            return this->tokens_[pos_].get();
        }
        return nullptr;
    }

    void unread() override {
        if (this->pos_ > 0) {
            this->pos_--;
        }
    }

    int getPosition() const override {
        return this->pos_;
    }

    void setPosition(int position) override {
        if (isValidPosition(position)) {
            this->pos_ = position;
        }
    }
private:
    // 判断 position 是否合法
    bool isValidPosition(int position) const {
        return position >= 0 && position < this->tokens_.size();
    }

    // 全部的 token 集合
    std::vector<std::unique_ptr<Token>> tokens_;
    // 当前读到的token的位置
    int pos_ = 0;
};


#endif //STUDY_COMPILER_SIMPLETOKENREADER_H
