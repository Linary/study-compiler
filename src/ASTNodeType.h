//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_ASTNODETYPE_H
#define STUDY_COMPILER_ASTNODETYPE_H

#include <iostream>
#include <string>

#define ASTNODE_TYPE_LIST(MACRO)            \
    MACRO(Program, "Program")               \
    MACRO(IntDeclaration, "IntDeclaration") \
    MACRO(ExpressionStmt, "ExpressionStmt") \
    MACRO(AssignmentStmt, "AssignmentStmt") \
    MACRO(Primary, "Primary")               \
    MACRO(Multiplicative, "Multiplicative") \
    MACRO(Additive, "Additive")             \
    MACRO(Identifier, "Identifier")         \
    MACRO(IntLiteral, "IntLiteral")

enum class ASTNodeType {
#define DECLARE_ENUM(enum_type, enum_string) enum_type,
    ASTNODE_TYPE_LIST(DECLARE_ENUM)
#undef DECLARE_ENUM
};

std::string ASTNodeTypeToString(ASTNodeType ast_node_type);

inline std::ostream& operator<<(std::ostream& os, ASTNodeType ast_node_type) {
    return os << ASTNodeTypeToString(ast_node_type);
}

inline std::string operator+(std::string& str, ASTNodeType ast_node_type) {
    return str + ASTNodeTypeToString(ast_node_type);
}

#endif //STUDY_COMPILER_ASTNODETYPE_H
