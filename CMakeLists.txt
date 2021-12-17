cmake_minimum_required(VERSION 3.20)
project(study_compiler)

set(CMAKE_CXX_STANDARD 14)

add_executable(study_compiler
        src/SimpleLexer.h
        src/SimpleLexer.cpp
        src/SimpleTokenReader.h
        src/Token.h
        src/SimpleToken.h
        src/TokenType.cpp
        src/TokenType.h
        src/DfaState.h
        src/TokenReader.h
        src/SimpleLexerTest.cpp
        )
