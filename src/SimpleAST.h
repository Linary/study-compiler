//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_SIMPLEAST_H
#define STUDY_COMPILER_SIMPLEAST_H

#include <memory>
#include <string>
#include <vector>
#include "ASTNode.h"

/**
 * 一个抽象语法树，持有着全部的 ASTNode
 */
class SimpleAST {
public:
    class Builder {
    public:
        explicit Builder() : builderASTNodes_(), firstAddedNode_(nullptr) {}

        std::unique_ptr<SimpleAST> build(ASTNode* rootASTNode = nullptr);

        ASTNode* addASTNode(std::unique_ptr<ASTNode> astNode);
    private:
        std::vector<std::unique_ptr<ASTNode>> builderASTNodes_;
        ASTNode* firstAddedNode_;
    };

    std::vector<ASTNode*> getASTNodes();

    std::vector<const ASTNode*> getASTNodes() const;

    ASTNode* rootNode() {
        return this->rootNode_;
    }

    const ASTNode* rootNode() const {
        return this->rootNode_;
    }

private:
    SimpleAST(std::vector<std::unique_ptr<ASTNode>>* astNodes, ASTNode* rootASTNode);

    // 保存全部的 AST Node
    std::vector<std::unique_ptr<ASTNode>> astNodes_;
    // 根 AST Node
    ASTNode* rootNode_;
};

#endif //STUDY_COMPILER_SIMPLEAST_H
