// Header file defining the token type for Vidi programming language

// Header guard
#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <string>

// enumeration for all token types
enum token_type {
  tok_Bool,
  tok_Int,
  tok_ID,
  tok_Str,
  tok_Float,
  tok_Declare,
  tok_Assign,
  tok_Print,
  tok_If,
  tok_Then,
  tok_Else,
  tok_EndIf,
  tok_While,
  tok_Do,
  tok_EndWhile,
  tok_Plus,
  tok_Minus,
  tok_Mult,
  tok_Div,
  tok_Mod,
  tok_And,
  tok_Or,
  tok_Not,
  tok_Equal,
  tok_NotEqual,
  tok_Less,
  tok_Greater,
  tok_LessEqual,
  tok_GreaterEqual,
  tok_EOF,
  tok_LeftBracket,
  tok_RightBracket,
  tok_Comma,
  tok_Length,
  tok_IntCast,
  tok_FloatCast,
  tok_StringCast,
  tok_Return,
  tok_LeftParen,
  tok_RightParen,
  tok_EndDeclare,
  tok_Call,
  tok_IntPred,
  tok_FloatPred,
  tok_StringPred,
  tok_BoolPred
};

// Type definitions for tokens
struct Token {
  token_type type;
  union {
    bool b;
    int i;
    float f;
  };
  std::string str;
};

// Function prototype for tokenize
std::vector<Token> tokenize (char * input);

#endif
