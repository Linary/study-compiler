//
// Created by 李凝瑞 on 2021/12/17.
//

#include "SimpleAST.h"
#include "absl/memory/memory.h"
#include "glog/logging.h"
#include "ASTNode.h"

ASTNode* SimpleAST::Builder::addASTNode(std::unique_ptr<ASTNode> astNode) {
    if (this->builderASTNodes_.empty()) {
        this->firstAddedNode_ = astNode.get();
    }
    this->builderASTNodes_.emplace_back(std::move(astNode));
    return this->builderASTNodes_.back().get();
}

std::unique_ptr<SimpleAST> SimpleAST::Builder::build(ASTNode* rootASTNode) {
    ASTNode* root = rootASTNode ? rootASTNode : this->firstAddedNode_;
    CHECK(root) << "ast root should not be valid";
    // 注意：这里SimpleAST的构造方法是私有的，用make_unique是没有访问权限的
    return absl::WrapUnique(new SimpleAST(&this->builderASTNodes_, root));
}

SimpleAST::SimpleAST(std::vector<std::unique_ptr<ASTNode>>* astNodes, ASTNode* rootASTNode)
    : rootNode_(rootASTNode) {
    bool rootFound = false;
    for (auto& node : *astNodes) {
        rootFound |= node.get() == this->rootNode_;
        this->astNodes_.emplace_back(std::move(node));
    }
    CHECK(rootFound) << "Error: root is not found in the AST.";
}

std::vector<ASTNode*> SimpleAST::getASTNodes() {
    std::vector<ASTNode*> ret;
    for (auto& astNode : this->astNodes_) {
        ret.emplace_back(astNode.get());
    }
    return std::move(ret);
}

std::vector<const ASTNode*> SimpleAST::getASTNodes() const {
    std::vector<const ASTNode*> ret;
    for (auto& astNode : this->astNodes_) {
        ret.emplace_back(astNode.get());
    }
    return std::move(ret);
}

