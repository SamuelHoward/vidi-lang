This document gives information relevant to the vidi parser

# Expressions

Types:
- Int : int
- Bool : bool
- ID : string
- Str : string
- Float : float
- Array : expr list
- Access : string, expr
- Sum : expr, expr
- Diff : expr, expr
- Mult : expr, expr
- Div : expr, expr
- Mod : expr, expr
- And : expr, expr
- Or : expr, expr
- Not : expr
- Equal : expr, expr
- NotEqual : expr, expr
- Less : expr, expr
- Greater : expr, expr
- LessEqual : expr, expr
- GreaterEqual : expr, expr
- Length : expr
- IntCast : expr
- FloatCast : expr
- StringCast : expr
- FuncCall: string, (expr list)
- IntPred : expr
- FloatPred : expr
- StringPred : expr
- BoolPred : expr

Grammar:
Exp -> [] or [ Exp /(, Exp)*/ ] or OrExp
OrExp -> (AndExp || OrExp) or AndExp
AndExp -> (EqualExp && AndExp) or EqualExp
EqualExp -> (RelationExp (== or !=) EqualExp) or RelationExp
RelationExp -> (AddExp (< or > or <= or >=) RelationExp) or AddExp
AddExp -> (MultExp (+ or -) AddExp) or MultExp
MultExp -> (UnExp (* or / or %) MultExp) or UnExp
UnExp -> (! UnExp) or (Tok_ID(Exp*)) or (LENGTH UnExp) or (INT UnExp) or (FLOAT UnExp) or (STRING UnExp) or (INT? UnExp) or (FLOAT? UnExp) or (STRING? UnExp) or (BOOL? UnExp) or (Tok_ID[AddExp]) or (Tok_Int or Tok_Bool or Tok_ID or Tok_Str or Tok_Float)

# Statements

Types:
- NoStmt
- Seq : stmt, stmt
- Declare: string, expr
- Assign: string, expr
- ArrayAssign : string, expr, expr
- FuncDec : string, (string list), Stmt
- If : expr, stmt, stmt
- While : expr, stmt
- Print : expr
- Return : expr
- Call : expr

Grammar:
Stmt -> (Stmt Stmt) | (DeclareSt or AssignSt or IfSt or WhileSt or PrintSt or ReturnSt or CallSt) 
DeclareSt -> ID <- expr
AssignSt -> ID = expr or ArrayAssignSt or FuncDec
ArrayAssignSt -> ID[int] = expr
FuncDec -> ID(expr*) <- Stmt ->
IfSt -> IF expr THEN stmt (ENDIF or (ELSE stmt ENDIF))
WhileSt -> WHILE expr DO stmt ENDWHILE
PrintSt -> PRINT expr
ReturnSt -> RETURN expr
CallSt -> CALL expr
