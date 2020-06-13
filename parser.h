// Header file defining the types for Vidi expressions and statements

// Header guard
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "lexer.h"

// enumeration for all expression types
enum expr_type {
  expr_int,
  expr_bool,
  expr_id,
  expr_str,
  expr_float,
  expr_array,
  expr_access,
  expr_sum,
  expr_diff,
  expr_mult,
  expr_div,
  expr_mod,
  expr_and,
  expr_or,
  expr_equal,
  expr_notEqual,
  expr_greater,
  expr_less,
  expr_greaterEqual,
  expr_lessEqual,
  expr_not,
  expr_length,
  expr_intCast,
  expr_floatCast,
  expr_stringCast,
  expr_funcCall,
  expr_intPred,
  expr_floatPred,
  expr_stringPred,
  expr_boolPred
};

// Type definition for expression tree
struct ExprTree;

struct ExprTree {
  expr_type type;
  union {
    bool b;
    int i;
    float f;
  };
  std::string str;
  std::vector<ExprTree> operands;
};

// enumeration for all statement types
enum stmt_type {
  stmt_none,
  stmt_seq,
  stmt_dec,
  stmt_assign,
  stmt_arrayAssign,
  stmt_funcDec,
  stmt_if,
  stmt_while,
  stmt_print,
  stmt_return,
  stmt_call
};

// Type definition for statement tree
struct StmtTree;

struct StmtTree {
  stmt_type type;
  std::vector<StmtTree> stmts;
  std::vector<ExprTree> exprs;
  std::vector<std::string> strs;
};

// Type definition for return value from parsing expressions
struct ParseExprRetVal {
  ExprTree exp;
  std::vector<Token> v;
};

// Type definition for return value from parsing statements
struct ParseStmtRetVal {
  StmtTree st;
  std::vector<Token> v;
};

// Function prototypes

// Function prototype for parsing any expression
ParseExprRetVal parse_expr (std::vector<Token> toks);

// Function prototype for parsing comma expressions
ParseExprRetVal parse_comma_expr (std::vector<Token> toks);

// Function prototype for parsing or expressions
ParseExprRetVal parse_or_expr (std::vector<Token> toks);

// Function prototype for parsing and expressions
ParseExprRetVal parse_and_expr (std::vector<Token> toks);

// Function prototype for parsing equality expressions
ParseExprRetVal parse_equal_expr (std::vector<Token> toks);

// Function prototype for parsing relational expressions
ParseExprRetVal parse_rel_expr (std::vector<Token> toks);

// Function prototype for parsing additive expressions
ParseExprRetVal parse_add_expr (std::vector<Token> toks);

// Function prototype for parsing multiplicative expressions
ParseExprRetVal parse_mult_expr (std::vector<Token> toks);

// Function prototype for parsing unary expressions
ParseExprRetVal parse_un_expr (std::vector<Token> toks);

// Function prototype for parsing comma (paren) expressions
ParseExprRetVal parse_comma_paren_expr (std::vector<Token> toks);

// Function prototype for parsing any statement
ParseStmtRetVal parse_stmt (std::vector<Token> toks);

// Function prototype for parsing declarative statements
ParseStmtRetVal parse_dec_stmt (std::vector<Token> toks);

// Function prototype for parsing assignment statements
ParseStmtRetVal parse_assign_stmt (std::vector<Token> toks);

// Function prototype for parsing comma id statements
ParseExprRetVal parse_comma_id_stmt (std::vector<Token> toks);

// Function prototype for parsing print statements
ParseStmtRetVal parse_print_stmt (std::vector<Token> toks);

// Function prototype for parsing if statements
ParseStmtRetVal parse_if_stmt (std::vector<Token> toks);

// Function prototype for parsing while statements
ParseStmtRetVal parse_while_stmt (std::vector<Token> toks);

// Function prototype for parsing return statements
ParseStmtRetVal parse_return_stmt (std::vector<Token> toks);

// Function prototype for parsing call statements
ParseStmtRetVal parse_call_stmt (std::vector<Token> toks);

#endif
