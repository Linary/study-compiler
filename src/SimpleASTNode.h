//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_SIMPLEASTNODE_H
#define STUDY_COMPILER_SIMPLEASTNODE_H

#include "absl/container/flat_hash_set.h"
#include "glog/logging.h"
#include "ASTNode.h"
#include "util.h"

/**
 * 一个简单的AST节点。
 * 属性包括：类型、文本值、父节点、子节点。
 */
class SimpleASTNode : public ASTNode {
public:
    SimpleASTNode(ASTNodeType node_type, const std::string& text): node_type_(node_type), text_(text) {}

    // 父节点
    const ASTNode* getParent() const override { return parent_; }

    // 子节点
    const std::vector<ASTNode*> getChildren() const override {
        return children_;
    }

    virtual ASTNode* getChildren(int child_idx) {
        CHECK_GE(child_idx, 0);
        CHECK_LT(child_idx, children_.size());
        return children_[child_idx];
    }

    // AST类型
    ASTNodeType getType() const override { return node_type_; }

    // 文本值
    const std::string& getText() const override { return text_; }

    void addChild(SimpleASTNode* child) {
        if (!containsKey(children_set_, child)) {
            // 避免添加重复的节点
            children_.emplace_back(child);
            children_set_.insert(child);
            child->parent_ = this;
        }
    }

private:
    ASTNode* parent_;
    std::vector<ASTNode*> children_;
    absl::flat_hash_set<const ASTNode*> children_set_;
    ASTNodeType node_type_;
    std::string text_;
};

#endif //STUDY_COMPILER_SIMPLEASTNODE_H
