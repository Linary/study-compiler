//
// Created by 李凝瑞 on 2021/12/17.
//

#include "TokenType.h"

std::string TokenTypeToString(TokenType token_type) {
    switch (token_type) {
#define CASE_ENUM_TYPE_STRING(enum_type, enum_name) \
  case TokenType::enum_type:                        \
    return enum_name;
        TOKEN_TYPE_LIST(CASE_ENUM_TYPE_STRING)
#undef CASE_ENUM_TYPE_STRING
    }
}