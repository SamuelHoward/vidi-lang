// Main script for interpreting Vidi code from file

#include <iostream>        // Include iostream
#include "lexer.h"         // Include definitions for lexing
#include "parser.h"        // Include definitions for parsing
#include "interpreter.h"   // Include definitions for interpreting
#include <fstream>         // Include fstream for taking files as input

// The main function reads the local file whose name/path is given as a
// command-line argument and lexes, parses, and interprets it as a Vidi
// program. If additional command-line arguments are given, they are
// instantiated as Vidi string values in a Vidi array called `ARGV`,
// accessible in the Vidi progam.
int main(int argc, char** argv)
{

  // Open the Vidi file given as a command-line argument and store it in
  // a single string.
  std::ifstream ifs(argv[1]);
  std::string program_content( (std::istreambuf_iterator<char>(ifs) ),
			       (std::istreambuf_iterator<char>()    ) );

  // Declare variables to use for the Vidi program, the ARGV Vidi array,
  // and the initial Vidi environment to use in the Vidi program.
  char * vidi_program = const_cast<char*> (program_content.c_str());
  Environment mt;
  Val argvVal;
  Val tmpVal;
  Environment mainEnv = mt;
  argvVal.type = val_array;
  tmpVal.type = val_str;

  // Iterate through the command line arguments and add them as Vidi strings
  // to the ARGV Vidi array.
  for(int i = 1; i < argc; i++) {
    tmpVal.str = argv[i];
    argvVal.elements.push_back(tmpVal);
  }

  // Add the ARGV Vidi array to the main environment.
  mainEnv.push_back(std::make_tuple("ARGV",argvVal));

  // Lex, parse, and evaluate the Vidi program, starting the main environment
  // with the ARGV array.
  eval_stmt("", mainEnv, ((parse_stmt(tokenize(vidi_program))).st));

  return 0;
}
