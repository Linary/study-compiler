//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#include <string>
#include <vector>

#include "ASTNodeType.h"

/**
 * AST的节点，属性包括：
 * AST的类型、文本值、下级子节点和父节点
 */
class ASTNode {
public:
    // 父节点
    virtual const ASTNode* getParent() const = 0;

    // 子节点
    virtual const std::vector<ASTNode*> getChildren() const = 0;

    // 类型
    virtual ASTNodeType getType() const = 0;

    // 文本值
    virtual const std::string& getText() const = 0;

    virtual ~ASTNode() = default;
};