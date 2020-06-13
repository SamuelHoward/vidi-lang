// Header file defining the types for Vidi values

// Header guard
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <vector>
#include <tuple>
#include <tuple>
#include "parser.h"

// enumeration for all value types
enum val_type {
  val_int,
  val_bool,
  val_str,
  val_float,
  val_array,
  val_func
};

// Type definition for expression tree
struct Val;

struct Val {
  val_type type;
  union {
    bool b;
    int i;
    float f;
  };
  std::string str;
  std::vector<Val> elements;
  std::vector<std::string> args;
  StmtTree funcBody;
};

// Type definition for environment
typedef std::vector<std::tuple<std::string, Val>> Environment;

// Function prototypes

// Function prototype for evaluating expressions
Val eval_expr (Environment env, ExprTree expr);

// Function prototype for evaluating statements
Environment eval_stmt (std::string sc, Environment env, StmtTree stmt);

// Function prototype for finding ID value in environment
Val find_id(Environment env, std::string str);

// Function prototype for evaluating a Vidi array
std::vector<Val> eval_array(Environment env, std::vector<ExprTree> elements);

// Function prototype for converting Vidi expressions to Vidi strings
std::vector<std::string> exprs_to_strings(std::vector<ExprTree> exprs);

// Function for printing an array
Environment print_array(Environment env, std::vector<Val> elements);

// Function prototype for zipping together vector of strings and expressions
// and adding them to the environment
Environment zip (Environment env, std::vector<std::string> names,
		 std::vector<ExprTree> input);

// Function for determining if two Vidi values are equal
bool val_equal (Val val1, Val val2);
  
// Function prototype for determining if two Vidi arrays are equal
bool array_equal (std::vector<Val> arr1, std::vector<Val> arr2);

#endif
