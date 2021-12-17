//
// Created by 李凝瑞 on 2021/12/17.
//
#pragma once

#ifndef STUDY_COMPILER_DFASTATE_H
#define STUDY_COMPILER_DFASTATE_H

/**
 * 有限状态机的各种状态。
 */
enum DfaState {

    Initial,

    If,
    Id_if1,
    Id_if2,

    Else,
    Id_else1,
    Id_else2,
    Id_else3,
    Id_else4,

    Int,
    Id_int1,
    Id_int2,
    Id_int3,

    Id,

    GT,
    GE,

    Assignment,

    Plus, Minus, Star, Slash,

    SemiColon,
    LeftParen,
    RightParen,

    IntLiteral
};

#endif //STUDY_COMPILER_DFASTATE_H
