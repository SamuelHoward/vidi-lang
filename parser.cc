// Functions necessary for parsing Vidi code

#include "parser.h"   // Include necessary parsing definitions
#include "lexer.h"    // Include necessary Token definitions
#include <iostream>   // Include IO stream library

// Function for parsing any expression
ParseExprRetVal parse_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;  
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Check if list is empty
  if (toks.size() == 0) {
    std::cout << "End of input";
  } else {
    
    // Check if token begins an expression or list of expression
    switch(toks[0].type) {
    case tok_LeftBracket :
      
      // Check if next token ends list
      switch(toks[1].type) {
      case tok_RightBracket :     // List is empty
	ret.exp.type = expr_array;
	ret.exp.operands = emptyExprTreeVector;
	toks.erase(toks.begin(), toks.begin() + 2);
	ret.v = toks;
	return ret;
	break;
      default :                   // List is nonempty
	toks.erase(toks.begin(), toks.begin() + 1);
	tmp1 = parse_comma_expr(toks);
	toks = tmp1.v;
	toks.erase(toks.begin());
	
	// Check for operations on arrays
	switch(tmp1.v[0].type) {
	case tok_Plus :                // Array appending
	  ret.exp.type = expr_sum;
	  tmp2 = parse_expr(toks);
	  emptyExprTreeVector.push_back(tmp1.exp);
	  emptyExprTreeVector.push_back(tmp2.exp);
	  ret.exp.operands = emptyExprTreeVector;
	  ret.v = tmp2.v;
	  break;
	case tok_Mult :                // Array initialization using *
	  ret.exp.type = expr_mult;
	  tmp2 = parse_expr(toks);
	  emptyExprTreeVector.push_back(tmp1.exp);
	  emptyExprTreeVector.push_back(tmp2.exp);
	  ret.exp.operands = emptyExprTreeVector;
	  ret.v = tmp2.v;
	  break;
	case tok_Equal :               // Array comparison using ==
	  tmp2 = parse_expr(toks);
	  emptyExprTreeVector.push_back(tmp1.exp);
	  emptyExprTreeVector.push_back(tmp2.exp);
	  ret.exp.operands = emptyExprTreeVector;
	  ret.v = tmp2.v;
	  ret.exp.type = expr_equal;
	  break;
	case tok_NotEqual :            // Array comparison using !=
	  tmp2 = parse_expr(toks);
	  emptyExprTreeVector.push_back(tmp1.exp);
	  emptyExprTreeVector.push_back(tmp2.exp);
	  ret.exp.operands = emptyExprTreeVector;
	  ret.v = tmp2.v;
	  ret.exp.type = expr_notEqual;
	  break;
	default :	  // Return array value if no operation is present
	  ret = tmp1;
	}
	return ret;
      }
      break;
    default :    // If value is not an array, parse it as an or expression
      return parse_or_expr(toks);
    }
  }
}

// Function for parsing comma expressions
ParseExprRetVal parse_comma_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Preliminary parsing
  tmp1 = parse_expr(toks);
  ret.exp.type = expr_array;
  
  // Check if there is another element in the array
  switch(tmp1.v[0].type) {
  case tok_Comma :              // There is at least one more element
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_comma_expr(tmp1.v);
    tmp2.exp.operands.insert(tmp2.exp.operands.begin(), tmp1.exp);
    ret.exp.operands = tmp2.exp.operands;
    ret.v = tmp2.v;
    break;
  case tok_RightBracket :       // There is no more elements
    tmp1.v.erase(tmp1.v.begin());
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                     
    ret = ret;
  }
  return ret;
}

// Function for parsing or expressions
ParseExprRetVal parse_or_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Preliminary parsing
  tmp1 = parse_and_expr(toks);
  
  // Check if expression utilizes `||`
  switch(tmp1.v[0].type) {
  case tok_Or :                  // expression uses `||` 
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_or_expr(tmp1.v);
    ret.exp.type = expr_or;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                      // expression does not use `||`
    ret = tmp1;
  }
  return ret;
}

// Function for parsing and expressions
ParseExprRetVal parse_and_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Preliminary parsing
  tmp1 = parse_equal_expr(toks);
  
  // Check if expressions uses `&&`
  switch(tmp1.v[0].type) {
  case tok_And :                  // Expression uses `&&`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_and_expr(tmp1.v);
    ret.exp.type = expr_and;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                       // Expression does not use `&&`
    ret = tmp1;
  }
  return ret;
}

// Function for parsing equality expressions
ParseExprRetVal parse_equal_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Preliminary parsing
  tmp1 = parse_rel_expr(toks);
  
  // Check if expression uses `==` or `!=`
  switch(tmp1.v[0].type) {
  case tok_Equal :                // Expression uses `==`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_equal_expr(tmp1.v);
    ret.exp.type = expr_equal;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_NotEqual :             // Expression uses `!=`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_equal_expr(tmp1.v);
    ret.exp.type = expr_notEqual;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                       // Expression does not use `==` or `!=`
    ret = tmp1;
  }
  return ret;
}

// Function for parsing relational expressions
ParseExprRetVal parse_rel_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Preliminary parsing
  tmp1 = parse_add_expr(toks);
  
  // Check if expression uses `<`, `>`, `<=`, or `>=`
  switch(tmp1.v[0].type) {
  case tok_Less :                 // Expression uses `<`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_rel_expr(tmp1.v);
    ret.exp.type = expr_less;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_Greater :              // Expression uses `>`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_rel_expr(tmp1.v);
    ret.exp.type = expr_greater;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_LessEqual :            // Expression uses `<=`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_rel_expr(tmp1.v);
    ret.exp.type = expr_lessEqual;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_GreaterEqual :         // Expression uses `>=`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_rel_expr(tmp1.v);
    ret.exp.type = expr_greaterEqual;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                      // Expression does not use those operators
    ret = tmp1;
  }
  return ret;
}

// Function for parsing additive expressions
ParseExprRetVal parse_add_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Preliminary parsing
  tmp1 = parse_mult_expr(toks);
  
  // Check if expression uses `+` or `-`
  switch(tmp1.v[0].type) {
  case tok_Plus :                  // Expression uses `+`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_add_expr(tmp1.v);
    ret.exp.type = expr_sum;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_Minus :                 // Expression uses `-`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_add_expr(tmp1.v);
    ret.exp.type = expr_diff;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                        // Expression does not use `+` or `-`
    ret = tmp1;
  }
  return ret;
}

// Function for parsing multiplicative expressions
ParseExprRetVal parse_mult_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Preliminary parsing
  tmp1 = parse_un_expr(toks);
  
  // Check if expression uses `*`, `/`, or `%`
  switch(tmp1.v[0].type) {
  case tok_Mult :                   // Expression uses `*`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_mult_expr(tmp1.v);
    ret.exp.type = expr_mult;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_Div :                    // Expression uses `/`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_rel_expr(tmp1.v);
    ret.exp.type = expr_div;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_Mod :                    // Expression uses `%`
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_rel_expr(tmp1.v);
    ret.exp.type = expr_mod;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    emptyExprTreeVector.push_back(tmp2.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                         // Expression does not uses `*`, `/`, or `%`
    ret = tmp1;
  }
  return ret;
}

// Function for parsing unary expressions
ParseExprRetVal parse_un_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Check if expression uses unary operators
  switch(toks[0].type) {
  case tok_Not :                    // Expression uses `!`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_not;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_Length :                 // Expression uses `LENGTH`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_length;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_IntCast :                // Expression uses `INT`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_intCast;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_FloatCast :              // Expression uses `FLOAT`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_floatCast;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_StringCast :             // Expression uses `STRING`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_stringCast;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_IntPred :                // Expression uses `INT?`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_intPred;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_FloatPred :              // Expression uses `FLOAT?`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_floatPred;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_StringPred :             // Expression uses `STRING?`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_stringPred;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_BoolPred :               // Expression uses `BOOL?`
    toks.erase(toks.begin());
    tmp1 = parse_un_expr(toks);
    ret.exp.type = expr_boolPred;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  case tok_Int :                    // Expression has an integer
    ret.exp.i = toks[0].i;
    toks.erase(toks.begin());
    ret.exp.type = expr_int;
    ret.v = toks;
    break;
  case tok_Bool :                   // Expression has a boolean
    ret.exp.b = toks[0].b;
    toks.erase(toks.begin());
    ret.exp.type = expr_bool;
    ret.v = toks;
    break;
  case tok_ID :                     // Expression has an ID
    switch(toks[1].type) {
    case tok_LeftBracket :                      // Expression uses array access
      tmp2.exp.str = toks[0].str;
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      tmp1 = parse_add_expr(toks);
      ret.exp.type = expr_access;
      tmp1.v.erase(tmp1.v.begin());
      ret.v = tmp1.v;
      emptyExprTreeVector.push_back(tmp2.exp);
      emptyExprTreeVector.push_back(tmp1.exp);
      ret.exp.operands = emptyExprTreeVector;
      break;
    case tok_LeftParen :                        // Expression uses function call
      ret.exp.str = toks[0].str;
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      tmp1 = parse_comma_paren_expr(toks);
      ret.exp.type = expr_funcCall;
      ret.v = tmp1.v;
      ret.exp.operands = tmp1.exp.operands;
      break;
    default :                                   // Expression is just an ID
      ret.exp.type = expr_id;
      ret.exp.str = toks[0].str;
      toks.erase(toks.begin());
      ret.v = toks;
    }
    break;
  case tok_Str :                    // Expression uses a string
    ret.exp.type = expr_str;
    ret.exp.str = toks[0].str;
    toks.erase(toks.begin());
    ret.v = toks;
    break;
  case tok_Float :                  // Expression uses a float
    ret.exp.f = toks[0].f;
    toks.erase(toks.begin());
    ret.exp.type = expr_float;
    ret.v = toks;
    break;
  case tok_LeftBracket :            // Expression has an array
    ret = parse_expr(toks);
    break;
  default :                    
    ret = ret;
  }
  return ret;
}

// Function for parsing comma (paren) expressions
ParseExprRetVal parse_comma_paren_expr (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;

  // Preliminary parsing
  tmp1 = parse_expr(toks);
  ret.exp.type = expr_array;

  // Check if function call has multiple arguments
  switch(tmp1.v[0].type) {
  case tok_Comma :                // Function call has multiple arguments
    tmp1.v.erase(tmp1.v.begin());
    tmp2 = parse_comma_paren_expr(tmp1.v);
    tmp2.exp.operands.insert(tmp2.exp.operands.begin(), tmp1.exp);
    ret.exp.operands = tmp2.exp.operands;
    ret.v = tmp2.v;
    break;
  case tok_RightParen :           // Function call has no more arguments
    tmp1.v.erase(tmp1.v.begin());
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.exp.operands = emptyExprTreeVector;
    break;
  default :                    
    ret = ret;
  }
  return ret;
}

// Function for parsing any statement
ParseStmtRetVal parse_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseStmtRetVal tmp1;
  ParseStmtRetVal tmp2;
  std::vector<StmtTree> emptyStmtTreeVector;
  
  // Check what type of statement is being parsed
  switch(toks[0].type) {
  case tok_ID :                 // Statement is for declaration or assignment

    // Statement is declarative
    if ((toks[1].type == tok_Declare) || (toks[1].type == tok_LeftParen)) {
      tmp1 = parse_dec_stmt(toks);
      tmp2 = parse_stmt(tmp1.v);
      ret.st.type = stmt_seq;
      ret.v = tmp2.v;
      emptyStmtTreeVector.push_back(tmp1.st);
      emptyStmtTreeVector.push_back(tmp2.st);
      ret.st.stmts = emptyStmtTreeVector;      
    } else {                            // Statement is an assignment statement
      tmp1 = parse_assign_stmt(toks);
      tmp2 = parse_stmt(tmp1.v);
      ret.st.type = stmt_seq;
      ret.v = tmp2.v;
      emptyStmtTreeVector.push_back(tmp1.st);
      emptyStmtTreeVector.push_back(tmp2.st);
      ret.st.stmts = emptyStmtTreeVector;
    }
    break;
  case tok_Print :              // Statement is print statement
    tmp1 = parse_print_stmt(toks);
    tmp2 = parse_stmt(tmp1.v);
    ret.st.type = stmt_seq;
    ret.v = tmp2.v;
    emptyStmtTreeVector.push_back(tmp1.st);
    emptyStmtTreeVector.push_back(tmp2.st);
    ret.st.stmts = emptyStmtTreeVector;
    break;
  case tok_If :                 // Statement is if statement
    tmp1 = parse_if_stmt(toks);
    tmp2 = parse_stmt(tmp1.v);
    ret.st.type = stmt_seq;
    ret.v = tmp2.v;
    emptyStmtTreeVector.push_back(tmp1.st);
    emptyStmtTreeVector.push_back(tmp2.st);
    ret.st.stmts = emptyStmtTreeVector;
    break;
  case tok_While :              // Statement is while statement
    tmp1 = parse_while_stmt(toks);
    tmp2 = parse_stmt(tmp1.v);
    ret.st.type = stmt_seq;
    ret.v = tmp2.v;
    emptyStmtTreeVector.push_back(tmp1.st);
    emptyStmtTreeVector.push_back(tmp2.st);
    ret.st.stmts = emptyStmtTreeVector;
    break;
  case tok_Return :             // Statement is return statement
    tmp1 = parse_return_stmt(toks);
    tmp2 = parse_stmt(tmp1.v);
    ret.st.type = stmt_seq;
    ret.v = tmp2.v;
    emptyStmtTreeVector.push_back(tmp1.st);
    emptyStmtTreeVector.push_back(tmp2.st);
    ret.st.stmts = emptyStmtTreeVector;
    break;
  case tok_Call :               // Statement is call statement
    tmp1 = parse_call_stmt(toks);
    tmp2 = parse_stmt(tmp1.v);
    ret.st.type = stmt_seq;
    ret.v = tmp2.v;
    emptyStmtTreeVector.push_back(tmp1.st);
    emptyStmtTreeVector.push_back(tmp2.st);
    ret.st.stmts = emptyStmtTreeVector;
    break;
  default :                     // There are no more statements in the program
    ret.v = toks;
    ret.st.type = stmt_none;
  }
  return ret;
}

// Function for parsing declarative statements
ParseStmtRetVal parse_dec_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseExprRetVal tmp1;
  ParseStmtRetVal tmp2;
  std::vector<StmtTree> emptyStmtTreeVector;
  std::vector<ExprTree> emptyExprTreeVector;
  std::vector<std::string> emptyStrVector;
  
  // Check what type of declarative statement this is
  switch(toks[0].type) {
  case tok_ID :

    // Check if statement is declaring function or variable
    switch(toks[1].type) {
    case tok_LeftParen :                 // Statement declares function
      emptyStrVector.push_back(toks[0].str);
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      tmp1 = parse_comma_id_stmt(toks);
      tmp1.v.erase(tmp1.v.begin());
      tmp2 = parse_stmt(tmp1.v);
      tmp2.v.erase(tmp2.v.begin());
      emptyExprTreeVector.push_back(tmp1.exp);
      emptyStmtTreeVector.push_back(tmp2.st);
      ret.v = tmp2.v;
      ret.st.type = stmt_funcDec;
      ret.st.exprs = tmp1.exp.operands;
      ret.st.stmts = emptyStmtTreeVector;
      ret.st.strs = emptyStrVector;
      break;
    default :                            // Statement declares variable
      emptyStrVector.push_back(toks[0].str);
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      tmp1 = parse_expr(toks);
      emptyExprTreeVector.push_back(tmp1.exp);
      ret.v = tmp1.v;
      ret.st.type = stmt_dec;
      ret.st.exprs = emptyExprTreeVector;
      ret.st.strs = emptyStrVector;
    }
    break;
  default :                   
    ret = ret;
  }
  return ret;
}

// Function for parsing assignment statements
ParseStmtRetVal parse_assign_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  std::vector<std::string> emptyStrVector;
  
  // Check what type of assignment statement this is
  switch(toks[0].type) {
  case tok_ID :       

    // Check if assignment statement
    switch(toks[1].type) {
    case tok_LeftBracket :              // Statement is array assignment
      emptyStrVector.push_back(toks[0].str);
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      tmp1 = parse_add_expr(toks);
      tmp1.v.erase(tmp1.v.begin());
      tmp1.v.erase(tmp1.v.begin());
      tmp2 = parse_expr(tmp1.v);
      emptyExprTreeVector.push_back(tmp1.exp);
      emptyExprTreeVector.push_back(tmp2.exp);
      ret.v = tmp2.v;
      ret.st.type = stmt_arrayAssign;
      ret.st.exprs = emptyExprTreeVector;
      ret.st.strs = emptyStrVector;
      break;
    default :                           // Statement is variable assignment
      emptyStrVector.push_back(toks[0].str);
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      tmp1 = parse_expr(toks);
      emptyExprTreeVector.push_back(tmp1.exp);
      ret.v = tmp1.v;
      ret.st.type = stmt_assign;
      ret.st.exprs = emptyExprTreeVector;
      ret.st.strs = emptyStrVector;
    }
    break;
  default :       
    ret = ret;
  }
  return ret;
}

// Function for parsing comma id statements
ParseExprRetVal parse_comma_id_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseExprRetVal ret;
  ParseExprRetVal tmp1;
  ParseExprRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  
  // Check the arguments in a function declaration
  switch(toks[0].type) {
  case tok_ID :   
    switch(toks[1].type) {
    case tok_Comma :               // Function has more arguments
      tmp2.exp.str = toks[0].str;
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      tmp1 = parse_comma_id_stmt(toks);
      tmp1.exp.operands.insert(tmp1.exp.operands.begin(), tmp2.exp);
      ret.exp.operands = tmp1.exp.operands;
      ret.v = tmp1.v;
      break;
    case tok_RightParen :          // Function has no more arguments
      tmp1.exp.str = toks[0].str;
      toks.erase(toks.begin());
      toks.erase(toks.begin());
      ret.v = toks;
      emptyExprTreeVector.push_back(tmp1.exp);
      ret.exp.operands = emptyExprTreeVector;
    default :        
      ret = ret;
    }
    break;
  default :    
    ret = ret;
  }
  return ret;
}

// Function for parsing print statements
ParseStmtRetVal parse_print_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseExprRetVal tmp1;
  std::vector<Token> emptyTokVector;
  std::vector<ExprTree> emptyExprTreeVector;

  // Ensure statement is a print statement
  switch(toks[0].type) {
  case tok_Print :             // Statement is a print statement
    toks.erase(toks.begin());
    tmp1 = parse_expr(toks);
    ret.st.type = stmt_print;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.st.exprs = emptyExprTreeVector;
    break;
  default :             
    ret = ret;
  }
  return ret;
}

// Function for parsing if statements
ParseStmtRetVal parse_if_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseExprRetVal tmp1;
  ParseStmtRetVal tmp2;
  ParseStmtRetVal tmp3;
  std::vector<ExprTree> emptyExprTreeVector;
  std::vector<StmtTree> emptyStmtTreeVector;
  
  // Preliminary parsing
  toks.erase(toks.begin());
  tmp1 = parse_expr(toks);
  tmp1.v.erase(tmp1.v.begin());
  tmp2 = parse_stmt(tmp1.v);

  // Check if if statement has an else statement
  switch(tmp2.v[0].type) {
  case tok_EndIf :                // Statement does not have else statement
    tmp3.st.type = stmt_none;
    tmp2.v.erase(tmp2.v.begin());
    ret.st.type = stmt_if;
    ret.v = tmp2.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.st.exprs = emptyExprTreeVector;
    emptyStmtTreeVector.push_back(tmp2.st);
    emptyStmtTreeVector.push_back(tmp3.st);
    ret.st.stmts = emptyStmtTreeVector;
    break;
  case tok_Else :                 // Statement has else statement
    tmp2.v.erase(tmp2.v.begin());
    tmp3 = parse_stmt(tmp2.v);
    tmp3.v.erase(tmp3.v.begin());
    ret.st.type = stmt_if;
    ret.v = tmp3.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.st.exprs = emptyExprTreeVector;
    emptyStmtTreeVector.push_back(tmp2.st);
    emptyStmtTreeVector.push_back(tmp3.st);
    ret.st.stmts = emptyStmtTreeVector;
    break;
  default :          
    ret = ret;
  }
  return ret;
}

// Function for parsing while statements
ParseStmtRetVal parse_while_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseExprRetVal tmp1;
  ParseStmtRetVal tmp2;
  std::vector<ExprTree> emptyExprTreeVector;
  std::vector<StmtTree> emptyStmtTreeVector;
  
  // Parse while statement
  toks.erase(toks.begin());
  tmp1 = parse_expr(toks);
  tmp1.v.erase(tmp1.v.begin());
  tmp2 = parse_stmt(tmp1.v);
  tmp2.v.erase(tmp2.v.begin());
  ret.v = tmp2.v;
  ret.st.type = stmt_while;
  emptyExprTreeVector.push_back(tmp1.exp);
  emptyStmtTreeVector.push_back(tmp2.st);
  ret.st.exprs = emptyExprTreeVector;
  ret.st.stmts = emptyStmtTreeVector;
  return ret;
}

// Function for parsing return statements
ParseStmtRetVal parse_return_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseExprRetVal tmp1;
  std::vector<ExprTree> emptyExprTreeVector;
    
  // Check statement is return statement
  switch(toks[0].type) {
  case tok_Return :              // Statement is return statement
    toks.erase(toks.begin());
    tmp1 = parse_expr(toks);
    ret.st.type = stmt_return;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.st.exprs = emptyExprTreeVector;
    break;
  default :       
    ret = ret;
  }
  return ret;
}

// Function for parsing call statements
ParseStmtRetVal parse_call_stmt (std::vector<Token> toks)
{
  // Initialize necessary variables
  ParseStmtRetVal ret;
  ParseExprRetVal tmp1;
  std::vector<ExprTree> emptyExprTreeVector;
    
  // Check if statement is call statement
  switch(toks[0].type) {
  case tok_Call :                // Statement is call statement
    toks.erase(toks.begin());
    tmp1 = parse_expr(toks);
    ret.st.type = stmt_call;
    ret.v = tmp1.v;
    emptyExprTreeVector.push_back(tmp1.exp);
    ret.st.exprs = emptyExprTreeVector;
    break;
  default :           
    ret = ret;
  }
  return ret;
}
