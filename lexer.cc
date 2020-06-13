// Functions necessary for lexing Vidi code

#include "lexer.h"   // Include necessary Token definitions
#include <regex>     // Include regular expression library
#include <string>    // Include string library
#include <vector>    // Include vector library
#include <iostream>  // Include IO stream library

// Main lexing function
std::vector<Token> tokenize (char * input)
{
  // Make a copy of input string to process
  char * dup = input;

  // Declare vector that will be used to store tokens
  std::vector<Token> ret_vector;

  // Declare token to use in some branches
  Token ret_token;
  
  // Declare regex constants for:
  std::regex re_int ("^-?[0-9]+");                   // integers
  std::regex re_str ("^[a-zA-Z][a-zA-Z0-9]*[?]?");   // strings
  std::regex re_id ("^[a-zA-Z]+[0-9]*");             // ids
  std::regex re_ws ("^[ \n\r\t]+");                  // whitespace
  std::regex re_com ("^#.*");                        // comments
  std::regex re_qstr ("^[\"][ a-zA-Z0-9]*[\"]");     // qouted strings
  std::regex re_fl ("^-?[0-9]+[.][0-9]+");           // floats
  std::regex re_dec ("^<-");                         // <-
  std::regex re_endDec ("^->");                      // ->
  std::regex re_plus ("^\\+");                       // +                    
  std::regex re_minus ("^\\-");                      // -
  std::regex re_mult ("^\\*");                       // *
  std::regex re_div ("^/");                          // /
  std::regex re_mod ("^%");                          // %
  std::regex re_and ("^&&");                         // &&
  std::regex re_or ("^\\|\\|");                      // ||
  std::regex re_equal ("^==");                       // ==
  std::regex re_unequal ("^\\!=");                   // !=
  std::regex re_leq ("^<=");                         // <=
  std::regex re_geq ("^>=");                         // >=
  std::regex re_assign ("^=");                       // =
  std::regex re_not ("^\\!");                        // !
  std::regex re_less ("^<");                         // <
  std::regex re_greater ("^>");                      // >
  std::regex re_leftB ("^\\[");                      // [
  std::regex re_rightB ("^\\]");                     // ]
  std::regex re_comma ("^,");                        // ,
  std::regex re_leftP ("^\\(");                      // (
  std::regex re_rightP ("^\\)");                     // )
  
  // Create matching object to store results of regex matching
  std::cmatch match;

  // Main loop for tokenizing input
  while ( *dup != '\0' ) {

    // Skip whitespace
    if ( std::regex_search (dup, match, re_ws) ) {
      dup++;
    } else {

      // Check if token could be keyword or ID
      if ( std::regex_search (dup, match, re_str) ) {

	// Check for each keyword or potential ID token...
	if (match[0].compare("PRINT") == 0) {                  // PRINT
	  ret_vector.push_back ({tok_Print});
	  dup += 5;
	} else if (match[0].compare("IF") == 0) {              // IF
	  ret_vector.push_back ({tok_If});
	  dup += 2;
	} else if (match[0].compare("THEN") == 0) {            // THEN
	  ret_vector.push_back ({tok_Then});
	  dup += 4;
	} else if (match[0].compare("ELSE") == 0) {            // ELSE
	  ret_vector.push_back ({tok_Else});
	  dup += 4;
	} else if (match[0].compare("ENDIF") == 0) {           // ENDIF
	  ret_vector.push_back ({tok_EndIf});
	  dup += 5;
	} else if (match[0].compare("WHILE") == 0) {           // WHILE
	  ret_vector.push_back ({tok_While});
	  dup += 5;
	} else if (match[0].compare("DO") == 0) {              // DO
	  ret_vector.push_back ({tok_Do});
	  dup += 2;
	} else if (match[0].compare("ENDWHILE") == 0) {        // ENDWHILE
	  ret_vector.push_back ({tok_EndWhile});
	  dup += 8;
	} else if (match[0].compare("TRUE") == 0) {            // TRUE
	  ret_vector.push_back ({tok_Bool, true});
	  dup += 4;
	} else if (match[0].compare("FALSE") == 0) {           // FALSE
	  ret_vector.push_back ({tok_Bool, false});
	  dup += 5;
	} else if (match[0].compare("LENGTH") == 0) {          // LENGTH
	  ret_vector.push_back ({tok_Length});
	  dup += 6;
	} else if (match[0].compare("INT") == 0) {             // INT
	  ret_vector.push_back ({tok_IntCast});
	  dup += 3;
	} else if (match[0].compare("FLOAT") == 0) {           // FLOAT
	  ret_vector.push_back ({tok_FloatCast});
	  dup += 5;
	} else if (match[0].compare("STRING") == 0) {          // STRING
	  ret_vector.push_back ({tok_StringCast});
	  dup += 6;
	} else if (match[0].compare("RETURN") == 0) {          // RETURN
	  ret_vector.push_back ({tok_Return});
	  dup += 6;
	} else if (match[0].compare("CALL") == 0) {            // CALL
	  ret_vector.push_back ({tok_Call});
	  dup += 4;
	} else if (match[0].compare("INT?") == 0) {            // INT?
	  ret_vector.push_back ({tok_IntPred});
	  dup += 4;
	} else if (match[0].compare("FLOAT?") == 0) {          // FLOAT?
	  ret_vector.push_back ({tok_FloatPred});
	  dup += 6;
	} else if (match[0].compare("STRING?") == 0) {         // STRING?
	  ret_vector.push_back ({tok_StringPred});
	  dup += 7;
	} else if (match[0].compare("BOOL?") == 0) {           // BOOL?
	  ret_vector.push_back ({tok_BoolPred});
	  dup += 5;
	} else if ( std::regex_search (dup, match, re_id) ) {  // IDs
	  ret_token.type = tok_ID;
	  ret_token.str = match[0].str();
	  ret_vector.push_back (ret_token);
	  dup += match[0].length();
	} else {                                               // lexing error
	  dup++; // line for testing purposes
	}

	// check for floats
      } else if ( std::regex_search (dup, match, re_fl) ) {
	ret_token.type = tok_Float;
	ret_token.f = std::stof(match[0].str());
	ret_vector.push_back (ret_token);
	dup += match[0].length();

	// check for ints
      } else if ( std::regex_search (dup, match, re_int) ) {
	ret_token.type = tok_Int;
	ret_token.i = std::stoi(match[0].str());
	ret_vector.push_back(ret_token);
	dup += match[0].length();

	// check for operators...
      } else if ( std::regex_search (dup, match, re_dec) ) {     // <-
	ret_vector.push_back ({tok_Declare});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_endDec) ) {  // ->
	ret_vector.push_back ({tok_EndDeclare});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_plus) ) {    // +
	ret_vector.push_back ({tok_Plus});
	dup++;	
      } else if ( std::regex_search (dup, match, re_minus) ) {   // -
	ret_vector.push_back ({tok_Minus});
	dup++;	
      } else if ( std::regex_search (dup, match, re_mult) ) {    // *
	ret_vector.push_back ({tok_Mult});
	dup++;	
      } else if ( std::regex_search (dup, match, re_div) ) {     // /
	ret_vector.push_back ({tok_Div});
	dup++;	
      } else if ( std::regex_search (dup, match, re_mod) ) {     // %
	ret_vector.push_back ({tok_Mod});
	dup++;	
      } else if ( std::regex_search (dup, match, re_and) ) {     // &&
	ret_vector.push_back ({tok_And});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_or) ) {      // ||
	ret_vector.push_back ({tok_Or});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_equal) ) {   // ==
	ret_vector.push_back ({tok_Equal});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_unequal) ) { // !=
	ret_vector.push_back ({tok_NotEqual});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_leq) ) {     // <=
	ret_vector.push_back ({tok_LessEqual});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_geq) ) {     // >=
	ret_vector.push_back ({tok_GreaterEqual});
	dup += 2;	
      } else if ( std::regex_search (dup, match, re_assign) ) {  // =
	ret_vector.push_back ({tok_Assign});
	dup++;	
      } else if ( std::regex_search (dup, match, re_not) ) {     // !
	ret_vector.push_back ({tok_Not});
	dup++;	
      } else if ( std::regex_search (dup, match, re_less) ) {    // <
	ret_vector.push_back ({tok_Less});
	dup++;	
      } else if ( std::regex_search (dup, match, re_greater) ) { // >
	ret_vector.push_back ({tok_Greater});
	dup++;	
      } else if ( std::regex_search (dup, match, re_leftB) ) {   // [
	ret_vector.push_back ({tok_LeftBracket});
	dup++;	
      } else if ( std::regex_search (dup, match, re_rightB) ) {  // ]
	ret_vector.push_back ({tok_RightBracket});
	dup++;	
      } else if ( std::regex_search (dup, match, re_comma) ) {   // ,
	ret_vector.push_back ({tok_Comma});
	dup++;	
      } else if ( std::regex_search (dup, match, re_leftP) ) {   // (
	ret_vector.push_back ({tok_LeftParen});
	dup++;	
      } else if ( std::regex_search (dup, match, re_rightP) ) {  // )
	ret_vector.push_back ({tok_RightParen});
	dup++;

	// Check for qouted string
      } else if ( std::regex_search (dup, match, re_qstr) ) {
	ret_token.type = tok_Str;
	ret_token.str =
	  match[0].str().erase(0, 1).erase((match[0].length() - 2),1);
	ret_vector.push_back (ret_token);
	dup += match[0].length();
	// Check for comments
      } else if ( std::regex_search (dup, match, re_com) ) {
	dup += match[0].length();
	// Check for lexing error
      } else {
	std::cout << "error 2\n"; // line for testing purposes
	dup++; // line for testing purposes
      }
    }
  }

  // Add end of file token to return vector
  ret_vector.push_back ({tok_EOF});

  // Return token vector as array pointer
  return ret_vector;
}
