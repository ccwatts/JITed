datatype binaryOperator =
     BOP_PLUS
   | BOP_MINUS
   | BOP_TIMES
   | BOP_DIVIDE
   | BOP_MOD
   | BOP_EQ
   | BOP_NE
   | BOP_LT
   | BOP_GT
   | BOP_LE
   | BOP_GE
   | BOP_AND
   | BOP_OR
;

datatype unaryOperator =
     UOP_NOT
   | UOP_MINUS
;

datatype expression =
     EXP_ID of string
   | EXP_NUM of int
   | EXP_STRING of string
   | EXP_TRUE
   | EXP_FALSE
   | EXP_UNDEFINED
   | EXP_BINARY of {opr: binaryOperator, lft: expression, rht: expression}
   | EXP_UNARY of {opr: unaryOperator, opnd: expression}
   | EXP_COND of {guard: expression, thenExp: expression, elseExp: expression}
   | EXP_ASSIGN of {lhs: expression, rhs: expression}
   | EXP_ANON of {params: string list, decls: string list, stmts: statement list}
   | EXP_CALL of {func: expression, args: expression list}

and statement =
   ST_EXP of {exp: expression}
 | ST_BLOCK of {stmts: statement list}
 | ST_IF of {guard: expression, th: statement, el: statement}
 | ST_PRINT of {exp: expression}
 | ST_WHILE of {guard: expression, body: statement}
 | ST_RETURN of {value: expression}
;

datatype function =
   FUNCTION of {name: string, code: {params:string list, decls: string list, stmts: statement list}}
;

open HashTable;

datatype typedval =
    TV_UNDEFINED
  | TV_BOOL of bool
  | TV_NUM of int
  | TV_STRING of string
                 (*code: EXP_ANON*)
  | TV_CLOSURE of {code: {params:string list, decls: string list, stmts: statement list}, context: ((string, typedval) hash_table) list, id: unit ref}
;


datatype program =
   PROGRAM of {decls: string list, funcs:function list, stmts: statement list}
;
