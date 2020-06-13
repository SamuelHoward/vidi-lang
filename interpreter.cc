// Functions necessary for evaluating Vidi codc

#include "interpreter.h"   // Include definitions for interpreting
#include "parser.h"        // Include definitions for parsing
#include <iostream>        // Include IO stream library
#include <algorithm>       // Include algorithm library

// Function for finding ID value in environment
Val find_id(Environment env, std::string str)
{
  // Initialize necessary variables
  Val ret;

  // Iterate through environment and look for ID
  for (int i = 0; i < env.size(); i++) {
    if ((std::get<0>(env[i])) == str) {
      return std::get<1>(env[i]);
    }
  }
  
  return ret;
}

// Function for evaluating a Vidi array
std::vector<Val> eval_array(Environment env, std::vector<ExprTree> elements)
{
  // Initialize necessary variables
  std::vector<Val> ret;

  // Iterate through array and evaluate element's expressions
  for (int i = 0; i < elements.size(); i++) {
    ret.insert(ret.begin(), eval_expr(env, elements[i]));
  }

  // Reverse the array so that values are where they are expected to be
  std::reverse(ret.begin(), ret.end());
  return ret;
}

// Function for converting Vidi expressions to Vidi strings
std::vector<std::string> exprs_to_strings(std::vector<ExprTree> exprs)
{
  // Initialize necessary variables
  std::vector<std::string> ret;

  // Iterate through expressions, storing the internal strings
  for (int i = 0; i < exprs.size(); i++) {
    ret.insert(ret.begin(), exprs[i].str);
  }

  // Reverse array so strings are where they are expected to be
  std::reverse(ret.begin(), ret.end());
  return ret;
}

// Function for printing an array
Environment print_array(Environment env, std::vector<Val> elements)
{
  // Initialize necessary variables
  Environment ret;

  // Iterate through elements and evaluate them as expressions
  for (int i = 0; i < elements.size(); i++) {

    // Check what the expression type is and evaluate it as such
    switch(elements[i].type) {
    case val_int :                        // Print integer value
      std::cout << elements[i].i << " ";
      break;
    case val_bool :                       // Print boolean value
      if (elements[i].b) {
	std::cout << "TRUE ";
      } else {
	std::cout << "FALSE ";
      }
      break;
    case val_str :                        // Print string value
      std::cout << elements[i].str << " ";
      break;
    case val_float :
      //
      std::cout << elements[i].f << " ";
      break;
    case val_array :                      // Print array (recursively)
      //
      ret = print_array(env, elements[i].elements);
      break;
    }
    ret = env;
    
  }

  std::cout << "\n";
  return ret;
}

// Function for zipping together vector of strings and expressions and adding
// them to the environment
Environment zip (Environment env, std::vector<std::string> names,
		 std::vector<ExprTree> input)
{
  // Initialize necessary variables
  Environment ret;

  // Iterate through both vectors and add elements together as tuples
  // to environment
  for (int i = 0; i < names.size(); i++) {
    ret.insert(ret.begin(),
	       std::make_tuple(names[i], (eval_expr(env, input[i]))));
  }
  
  return ret;
}

// Function for determining if two Vidi values are equal
bool val_equal (Val val1, Val val2) {

  // Depending on the type of the values, return whether they are equal
  if ((val1.type == val_int) && (val2.type == val_int)) {
    return val1.i == val2.i;
  } else if ((val1.type == val_float) && (val2.type == val_float)) {
    return val1.f == val2.f;
  } else if ((val1.type == val_bool) && (val2.type == val_bool)) {
    return val1.f == val2.f;
  } else if ((val1.type == val_array) && (val2.type == val_array)) {
    return array_equal(val1.elements, val2.elements);
  } else {
    return false;
  }
  
}

// Function for determining if two Vidi arrays are equal
bool array_equal (std::vector<Val> arr1, std::vector<Val> arr2) {

  // If arrays are different sizes, they are unequal
  if (arr1.size() != arr2.size()) {
    return false;
  } else {  // If arrays are same size,
            // iterate through them to determine inequality
  
    for (int i = 0; i < arr1.size(); i++) {
      if (!val_equal(arr1[i], arr2[i])) {
	return false;
      }
    }
  }
  
  return true;
}

// Function for evaluating any expression
Val eval_expr (Environment env, ExprTree expr)
{
  // Initialize necessary variables
  Val ret;
  Val tmpVal1;
  Val tmpVal2;
  Environment tmpEnv0;
  Environment tmpEnv1;
  Environment tmpEnv2;
  std::vector<Val> tmpValVector;
  std::vector<Val> mtVal;
  
  // Check what type of expression this is
  switch(expr.type) {
  case expr_id :                      // Expression is an ID 
    ret = find_id(env, expr.str);
    break;
  case expr_int :                     // Expression is an integer
    ret.type = val_int;
    ret.i = expr.i;
    break;
  case expr_bool :                    // Expression is a boolean
    ret.type = val_bool;
    ret.b = expr.b;
    break;
  case expr_str :                     // Expression is a string
    ret.type = val_str;
    ret.str = expr.str;
    break;
  case expr_float :                   // Expression is a float
    ret.type = val_float;
    ret.f = expr.f;
    break;
  case expr_array :                   // Expression is an array
    ret.type = val_array;
    ret.elements = eval_array(env, expr.operands);
    break;
  case expr_access :                  // Expression is an array access
    ret =
      find_id(env, expr.operands[0].str)
      .elements[eval_expr(env, expr.operands[1]).i];
    ret = ret;
    break;
  case expr_sum :                     // Expression is a sum
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);

    // If sum has all floats or mixture of floats/ints, do float addition.
    // If sum is all ints, do integer addition
    if (tmpVal1.type == val_int) {
      if (tmpVal2.type == val_int) {
	ret.type = val_int;
	ret.i = tmpVal1.i + tmpVal2.i;
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = ((float) tmpVal1.i) + tmpVal2.f;
      }
    } else if (tmpVal1.type == val_float) {
      if (tmpVal2.type == val_int) {
	ret.type = val_float;
	ret.f = tmpVal1.f + ((float) tmpVal2.i);
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = tmpVal1.f + tmpVal2.f;
      }
      
    // If sum is all strings, concatenate strings
    } else if (tmpVal1.type == val_str) {
      ret.type = val_str;
      ret.str = tmpVal1.str.append(tmpVal2.str);

    // If sum is all arrays, concatenate arrays
    } else if (tmpVal1.type == val_array) {
      ret.type = val_array;
      tmpVal2.elements.insert(tmpVal2.elements.begin(),
			      tmpVal1.elements.begin(),
			      tmpVal1.elements.end());
      ret.elements = tmpVal2.elements;
    }
    
    break;
  case expr_diff :                    // Expression is a subtraction
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);

    // If subtraction has all floats or mixture of floats/ints,
    // do float subtraction.
    // If subtraction is all ints, do integer subtraction
    if (tmpVal1.type == val_int) {
      if (tmpVal2.type == val_int) {
	ret.type = val_int;
	ret.i = tmpVal1.i - tmpVal2.i;
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = ((float) tmpVal1.i) - tmpVal2.f;
      }
    } else if (tmpVal1.type == val_float) {
      if (tmpVal2.type == val_int) {
	ret.type = val_float;
	ret.f = tmpVal1.f - ((float) tmpVal2.i);
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = tmpVal1.f - tmpVal2.f;
      }
    }
    break;
    
  case expr_mult :                    // Expression is a multiplication
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);

    // If multiplication has all floats or mixture of floats/ints,
    // do float multiplication
    // If multiplication is all ints, do integer multiplication
    if (tmpVal1.type == val_int) {
      if (tmpVal2.type == val_int) {
	ret.type = val_int;
	ret.i = tmpVal1.i * tmpVal2.i;
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = ((float) tmpVal1.i) * tmpVal2.f;

	// If multiplication is all strings, duplicate strings
      } else if (tmpVal2.type == val_str) {
	ret.type = val_str;
	if (tmpVal1.i == 0) {
	  ret.str = "";
	} else {
	  ret.str = tmpVal2.str;
	  for(int i = 1; i < tmpVal1.i; i++) {
	    ret.str = ret.str.append(tmpVal2.str);
	  }
	}

	// If multiplication is all arrays, duplicate arrays
      } else if (tmpVal2.type == val_array) {
	ret.type = val_array;
	if (tmpVal1.i == 0) {
	  ret.elements = mtVal;
	} else {
	  tmpValVector = tmpVal2.elements;
	  ret.elements = tmpVal2.elements;
	  for(int i = 1; i < tmpVal1.i; i++) {
	    ret.elements.insert(ret.elements.begin(),
				tmpValVector.begin(), tmpValVector.end());
	  }
	}
      }
    } else if (tmpVal1.type == val_float) {
      if (tmpVal2.type == val_int) {
	ret.type = val_float;
	ret.f = tmpVal1.f * ((float) tmpVal2.i);
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = tmpVal1.f * tmpVal2.f;
      }
    } else if (tmpVal1.type == val_str) {
      ret.type = val_str;
      if (tmpVal2.i == 0) {
	ret.str = "";
      } else {
	ret.str = tmpVal1.str;
	for(int i = 1; i < tmpVal2.i; i++) {
	  ret.str = ret.str.append(tmpVal1.str);
	}
      }
    } else if (tmpVal1.type == val_array) {
      ret.type = val_array;
      if (tmpVal2.i == 0) {
	ret.elements = mtVal;
      } else {
	tmpValVector = tmpVal1.elements;
	ret.elements = tmpVal1.elements;
	for(int i = 1; i < tmpVal2.i; i++) {
	  ret.elements.insert(ret.elements.begin(),
			      tmpValVector.begin(), tmpValVector.end());
	} 
      }
    }
     break;
     
  case expr_div :                     // Expression is division
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);

    // If division has all floats or mixture of floats/ints,
    // do float division
    // If division is all ints, do integer division
    if (tmpVal1.type == val_int) {
      if (tmpVal2.type == val_int) {
	ret.type = val_int;
	ret.i = tmpVal1.i / tmpVal2.i;
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = ((float) tmpVal1.i) / tmpVal2.f;
      }
    } else if (tmpVal1.type == val_float) {
      if (tmpVal2.type == val_int) {
	ret.type = val_float;
	ret.f = tmpVal1.f / ((float) tmpVal2.i);
      } else if (tmpVal2.type == val_float) {
	ret.type = val_float;
	ret.f = tmpVal1.f / tmpVal2.f;
      }
    }
    break;
    
  case expr_mod :                     // Expression is a modulo expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
   
    if ((tmpVal1.type == val_int) && (tmpVal2.type == val_int)) {
      ret.type = val_int;
      ret.i = tmpVal1.i % tmpVal2.i;
    }
    break;
    
  case expr_greater :                 // Expression is a `>` expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_int) && (tmpVal2.type == val_int)) {
      ret.type = val_bool;
      ret.b = tmpVal1.i > tmpVal2.i;
    } else if ((tmpVal1.type == val_float) && (tmpVal2.type == val_float)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f > tmpVal2.f;
    }
    break;
    
  case expr_less :                    // Expression is a `<` expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_int) && (tmpVal2.type == val_int)) {
      ret.type = val_bool;
      ret.b = tmpVal1.i < tmpVal2.i;
    } else if ((tmpVal1.type == val_float) && (tmpVal2.type == val_float)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f < tmpVal2.f;
    }
    break;
    
  case expr_greaterEqual :            // Expression is a `>=` expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_int) && (tmpVal2.type == val_int)) {
      ret.type = val_bool;
      ret.b = tmpVal1.i >= tmpVal2.i;
    } else if ((tmpVal1.type == val_float) && (tmpVal2.type == val_float)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f >= tmpVal2.f;
    }
    break;
    
  case expr_lessEqual :               // Expression is a `<=` expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_int) && (tmpVal2.type == val_int)) {
      ret.type = val_bool;
      ret.b = tmpVal1.i <= tmpVal2.i;
    } else if ((tmpVal1.type == val_float) && (tmpVal2.type == val_float)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f <= tmpVal2.f;
    }
    break;

  case expr_equal :                   // Expression is a `==` expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_int) && (tmpVal2.type == val_int)) {
      ret.type = val_bool;
      ret.b = tmpVal1.i == tmpVal2.i;
    } else if ((tmpVal1.type == val_float) && (tmpVal2.type == val_float)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f == tmpVal2.f;
    } else if ((tmpVal1.type == val_bool) && (tmpVal2.type == val_bool)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f == tmpVal2.f;
    } else if ((tmpVal1.type == val_array) && (tmpVal2.type == val_array)) {
      ret.type = val_bool;
      ret.b = array_equal(tmpVal1.elements, tmpVal2.elements);
    }
    break;

  case expr_notEqual :                // Expression is a `!=` expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_int) && (tmpVal2.type == val_int)) {
      ret.type = val_bool;
      ret.b = tmpVal1.i != tmpVal2.i;
    } else if ((tmpVal1.type == val_float) && (tmpVal2.type == val_float)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f != tmpVal2.f;
    } else if ((tmpVal1.type == val_bool) && (tmpVal2.type == val_bool)) {
      ret.type = val_bool;
      ret.b = tmpVal1.f != tmpVal2.f;
    } else if ((tmpVal1.type == val_array) && (tmpVal2.type == val_array)) {
      ret.type = val_bool;
      ret.b = !array_equal(tmpVal1.elements, tmpVal2.elements);
    }
    break;

  case expr_or :                      // Expression is an or expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_bool) && (tmpVal2.type == val_bool)) {
      ret.type = val_bool;
      ret.b = tmpVal1.b || tmpVal2.b;
    }
    break;

  case expr_and :                     // Expression is an and expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    tmpVal2 = eval_expr(env, expr.operands[1]);
    
    if ((tmpVal1.type == val_bool) && (tmpVal2.type == val_bool)) {
      ret.type = val_bool;
      ret.b = tmpVal1.b && tmpVal2.b;
    }
    break;

  case expr_not :                     // Expression is a not expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_bool) {
      ret.type = val_bool;
      ret.b = ! tmpVal1.b;
    }
    break;
    
  case expr_length :                  // Expression is a Length expression 
    ret.type = val_int;
    tmpVal1 = eval_expr(env, expr.operands[0]);
    ret.i = tmpVal1.elements.size();
    break;
    
  case expr_intCast :                 // Expression is an int cast expression
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_int) {
      ret.type = val_int;
      ret.i = tmpVal1.i;
    } else if (tmpVal1.type == val_str) {
      ret.type = val_int;
      ret.i = std::stoi(tmpVal1.str);
    } else if (tmpVal1.type == val_float) {
      ret.type = val_int;
      ret.i = (int) tmpVal1.f;
    } else {
      std::cout << "other\n";
    }
    break;

  case expr_floatCast :               // Expression is a float cast expression 
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_int) {
      ret.type = val_float;
      ret.f = (float) tmpVal1.i;
    } else if (tmpVal1.type == val_str) {
      ret.type = val_float;
      ret.f = std::stof(tmpVal1.str);
    } else if (tmpVal1.type == val_float) {
      ret.type = val_float;
      ret.f = tmpVal1.f;
    } else {
      std::cout << "other\n";
    }
    break;

  case expr_stringCast :              // Expression is a string cast expression 
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_int) {
      ret.type = val_str;
      ret.str = std::to_string(tmpVal1.i);
    } else if (tmpVal1.type == val_str) {
      ret.type = val_str;
      ret.str = tmpVal1.str;
    } else if (tmpVal1.type == val_float) {
      ret.type = val_str;
      ret.str = std::to_string(tmpVal1.f);
    } else {
      std::cout << "other\n";
    }
    break;
    
  case expr_intPred :                 // Expression is an int predicate 
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_int) {
      ret.type = val_bool;
      ret.b = true;
    } else {
      ret.type = val_bool;
      ret.b = false;
    }
    break;
    
  case expr_floatPred :               // Expression is a float predicate
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_float) {
      ret.type = val_bool;
      ret.b = true;
    } else {
      ret.type = val_bool;
      ret.b = false;
    }
    break;
    
  case expr_boolPred :                // Expression is a bool predicate
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_bool) {
      ret.type = val_bool;
      ret.b = true;
    } else {
      ret.type = val_bool;
      ret.b = false;
    }
    break;
    
  case expr_stringPred :              // Expression is an string predicate
    tmpVal1 = eval_expr(env, expr.operands[0]);
    
    if (tmpVal1.type == val_str) {
      ret.type = val_bool;
      ret.b = true;
    } else {
      ret.type = val_bool;
      ret.b = false;
    }
    break;
    
  case expr_funcCall :                // Expression is a function call
    tmpVal1 = find_id(env, expr.str);
    tmpEnv0 = zip(env,tmpVal1.args,expr.operands);
    tmpEnv1 = env;
    tmpEnv1.insert(tmpEnv1.begin(), tmpEnv0.begin(), tmpEnv0.end());
    tmpEnv2 = eval_stmt(expr.str, tmpEnv1, tmpVal1.funcBody);
    ret = find_id(tmpEnv2, expr.str.append("_"));
    break;
  }
  return ret;
}

// Function for evaluating any statement
Environment eval_stmt (std::string sc, Environment env, StmtTree stmt)
{
  // Initialize necessary variables
  Environment ret = env;
  Val tmpVal;

  // Check what type of statement this is
  switch(stmt.type) {
    
  case stmt_none :                  // No statement here
    ret = env;
    break;
    
  case stmt_seq :                   // Statement is a sequence
    ret = eval_stmt(sc, (eval_stmt(sc, env, (stmt.stmts[0]))), (stmt.stmts[1]));
    break;
    
  case stmt_dec :                   // Statement is a declaration
    ret.insert(ret.begin(),
	       std::make_tuple(stmt.strs[0], eval_expr(env, stmt.exprs[0])));
    break;
    
  case stmt_assign :                // Statement is an assignment
    ret.insert(ret.begin(),
	       std::make_tuple(stmt.strs[0], eval_expr(env, stmt.exprs[0])));
    break;
    
  case stmt_arrayAssign :           // Statement is an array assignment
    tmpVal = find_id(env, stmt.strs[0]);
    tmpVal.elements.erase
      (tmpVal.elements.begin() + (eval_expr(env, stmt.exprs[0])).i);
    tmpVal.elements.insert
      (tmpVal.elements.begin() + (eval_expr(env, stmt.exprs[0])).i,
       eval_expr(env, stmt.exprs[1]));
    ret.insert(ret.begin(), std::make_tuple(stmt.strs[0], tmpVal));
    break;
    
  case stmt_if :                    // Statement is an if
    if (eval_expr(env, stmt.exprs[0]).b) {
      ret = eval_stmt(sc, env, (stmt.stmts[0]));
    } else {
      ret = eval_stmt(sc, env, (stmt.stmts[1]));
    }
    break;
    
  case stmt_while :                 // Expression is a while
    if (eval_expr(env, stmt.exprs[0]).b) {
      ret = eval_stmt(sc, (eval_stmt(sc, env, stmt.stmts[0])), stmt);
    } else {
      ret = env;
    }
    break;
    
  case stmt_print :                 // Expression is a print
    tmpVal = eval_expr(env, (stmt.exprs[0]));
    switch(tmpVal.type) {
    case val_int :
      std::cout << tmpVal.i << "\n";
      break;
    case val_bool :
      if (tmpVal.b) {
	std::cout << "TRUE" << "\n";
      } else {
	std::cout << "FALSE" << "\n";
      }
      break;
    case val_str :
      std::cout << tmpVal.str << "\n";
      break;
    case val_float :
      std::cout << tmpVal.f << "\n";
      break;
    case val_array :
      ret = print_array(env, tmpVal.elements);
      break;
    default :
      std::cout << tmpVal.type;
    }
    ret = env;
    break;
    
  case stmt_funcDec :               // Statement is a function
                                    // declaration
    tmpVal.type = val_func;
    tmpVal.str = stmt.strs[0];
    tmpVal.funcBody = stmt.stmts[0];
    tmpVal.args = exprs_to_strings(stmt.exprs);
    ret.insert(ret.begin(), std::make_tuple(stmt.strs[0], tmpVal));
    break;
    
  case stmt_return :                // Statement is a return
    tmpVal = eval_expr(env, stmt.exprs[0]);
    ret.insert(ret.begin(), std::make_tuple(sc.append("_"), tmpVal));
    break;
    
  case stmt_call :                  // Statement is a call
    eval_expr(env, stmt.exprs[0]);
    ret = env;
    break;
  }
  
  return ret;
}
