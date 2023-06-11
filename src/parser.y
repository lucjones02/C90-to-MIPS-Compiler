%code requires{

  #include "ast.hpp"
  #include <string>
  #include <stdio.h>

  extern const Node *g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);

}

%union{
    Node *node;
    double number;
    std::string *str;
}

%token INT FLOAT DOUBLE CHAR UNSIGNED
%token PLUS MINUS TIMES DIVIDE MODULO QUOTIENT BITFLIP DPLUS DMINUS
%token AND OR XOR LESS GREATER SHIFT_L SHIFT_R DAND DOR
%token LBRACKET RBRACKET LCBRACKET RCBRACKET LSBRACKET RSBRACKET
%token SEMICOLON EQUAL COMMA EXCLMARK COLON DOT
%token IF ELSE WHILE FOR RETURN ENUM SWITCH CASE BREAK DEFAULT CONTINUE
%token SIZEOF STRUCT TYPEDEF
%token NUMBER NUMBER_F IDENTIFIER 

%type <node> CODE LINE FUNCTION EXPR IF_STATEMENT WHILE_LOOP MATH_EXPRESSION SWITCH_STATEMENT CASES ONE_CASE
%type <node> RETURN_STATEMENT ASSIGN DECLARE TERM UNARY FACTOR 
%type <node> COMPARISON BITWISE_OR BITWISE_XOR BITWISE_AND EQUAL_COMPARISON OR_OP AND_OP
%type <node> ADDSUB SHIFTERS FUNCTION_CALL FUNCTION_ARGUMENTS FOR_LOOP CALL_ARGS
%type <node> DECLARE_ARRAY ACCESS_ARRAY ENUM_STMT ENUM_EXPR SIZEOF_FUNC
%type <node> STRUCT_DEFINE STRUCT_CONSTRUCT STRUCT_IN STRUCT_GET
%type <number> NUMBER NUMBER_F
%type <str> IDENTIFIER TYPE 

%start ROOT


%%

ROOT : CODE {g_root = $1;}
;

CODE : LINE { fprintf(stderr, "line\n") ; $$ = $1; }
     | FUNCTION { $$ = $1; }
     | CODE CODE { $$ = new Succession($1, $2); }
  // remember to remove Nop() here when implementing global variables
     | LCBRACKET CODE RCBRACKET { $$ = new Nop();}
     ;

LINE : EXPR SEMICOLON { $$ = $1; }
    | BREAK SEMICOLON { $$ = new Break(); }
    | CONTINUE SEMICOLON { $$ = new Continue(); }
    | IF_STATEMENT { $$ = $1; }
    | SWITCH_STATEMENT { $$ = $1; }
    | WHILE_LOOP { $$ = $1; }
    | FOR_LOOP { $$ = $1; }
    | DECLARE_ARRAY SEMICOLON { $$ = $1; }
    | SEMICOLON { $$ = new Nop(); }
    | ENUM_STMT { $$ = $1; }
    | STRUCT_DEFINE { $$ = $1; }
    | TYPEDEF TYPE IDENTIFIER SEMICOLON { $$ = new Typedef(*$2 ,*$3); }
    | TYPEDEF TYPE TIMES IDENTIFIER SEMICOLON { $$ = new Typedef(*$2 ,"* " + *$4); }
    | TYPEDEF IDENTIFIER TIMES IDENTIFIER SEMICOLON { $$ = new Typedef(*$2 ,"* " + *$4); }
    ;

FUNCTION : TYPE IDENTIFIER LBRACKET RBRACKET LCBRACKET CODE RCBRACKET { $$ = new Function(*$1, *$2, new Nop(), $6, true); }
         | TYPE IDENTIFIER LBRACKET RBRACKET LCBRACKET RCBRACKET { $$ = new Function(*$1, *$2, new Nop(), new Nop(), true); }
         | TYPE IDENTIFIER LBRACKET FUNCTION_ARGUMENTS RBRACKET LCBRACKET CODE RCBRACKET { fprintf(stderr, "new function\n"); $$ = new Function(*$1, *$2, $4, $7, true); }
         | TYPE IDENTIFIER LBRACKET RBRACKET { $$ = new Function(*$1, *$2, new Nop(), new Nop(), false); }
         | TYPE IDENTIFIER LBRACKET FUNCTION_ARGUMENTS RBRACKET { $$ = new Function(*$1, *$2, $4, new Nop(), false); }
         ;

FUNCTION_ARGUMENTS : TYPE IDENTIFIER { $$ = new Declare(*$1, *$2); }
                   | TYPE IDENTIFIER COMMA FUNCTION_ARGUMENTS { $$ = new Succession(new Declare(*$1, *$2), $4); }

RETURN_STATEMENT : RETURN MATH_EXPRESSION { $$ = new Return($2); }
                 | RETURN IDENTIFIER { NodePtr tmp = new Identifier(*$2); $$ = new Return(tmp); }
                 | RETURN TIMES IDENTIFIER { NodePtr tmp = new Identifier(*$3); $$ = new Return(tmp); }
                 | RETURN FUNCTION_CALL { $$ = new Return($2); }
                 | RETURN ACCESS_ARRAY { $$ = new Return($2); }
                 | RETURN SIZEOF_FUNC { $$ = new Return($2); }
                 | RETURN STRUCT_GET { $$ = new Return(new Identifier($2->getId()));}
                 ;

IF_STATEMENT : IF LBRACKET MATH_EXPRESSION RBRACKET LCBRACKET CODE RCBRACKET ELSE LCBRACKET CODE RCBRACKET { $$ = new If($3, $6, $10); }
             | IF LBRACKET MATH_EXPRESSION RBRACKET LCBRACKET CODE RCBRACKET { $$ = new If($3, $6, new Nop()); }
             | IF LBRACKET MATH_EXPRESSION RBRACKET LCBRACKET RCBRACKET { $$ = new If($3, new Nop(), new Nop()); }
             ;
            
ONE_CASE : CASE MATH_EXPRESSION COLON CODE { fprintf(stderr, "new case\n"); $$ = new Case($2, $4); }
         ;

CASES : ONE_CASE CASES { $$ = new Succession($1, $2); }
      | ONE_CASE { $$ = $1; }
      | DEFAULT COLON CODE { $$ = $3; }
      ;

SWITCH_STATEMENT : SWITCH LBRACKET MATH_EXPRESSION RBRACKET LCBRACKET CASES RCBRACKET { $$ = new Switch($3, $6); }
                 | SWITCH LBRACKET MATH_EXPRESSION RBRACKET LCBRACKET RCBRACKET { $$ = new Nop(); }
                 ;

WHILE_LOOP : WHILE LBRACKET MATH_EXPRESSION RBRACKET LCBRACKET CODE RCBRACKET { fprintf(stderr, "new while\n") ; $$ = new While($3, $6); }
           | WHILE LBRACKET MATH_EXPRESSION RBRACKET LCBRACKET RCBRACKET { $$ = new While($3, new Nop()); }
           ;

FOR_LOOP : FOR LBRACKET EXPR SEMICOLON COMPARISON SEMICOLON ASSIGN RBRACKET LCBRACKET CODE RCBRACKET {  fprintf(stderr, "new for loop\n"); $$ = new For($3, $5, $7, $10); }
         | FOR LBRACKET EXPR SEMICOLON COMPARISON SEMICOLON ASSIGN RBRACKET LCBRACKET RCBRACKET { $$ = new For($3, $5, $7, new Nop()); }
     
ENUM_STMT : ENUM IDENTIFIER LCBRACKET ENUM_EXPR RCBRACKET SEMICOLON { $$ = new Enum_statement(*$2, $4); }

ENUM_EXPR : IDENTIFIER { $$ = new Enum_expression(*$1, -1); }
          | IDENTIFIER EQUAL NUMBER { $$ = new Enum_expression(*$1, $3); }
          | ENUM_EXPR COMMA ENUM_EXPR { $$ = new Succession($1, $3); }

DECLARE_ARRAY : TYPE IDENTIFIER LSBRACKET NUMBER RSBRACKET { fprintf(stderr, "new array\n"); $$ = new Declare_Array(*$2, $4, 1); }
              | DECLARE_ARRAY LSBRACKET NUMBER RSBRACKET { $$->change_size($3); $$->dim1(); }

SIZEOF_FUNC : SIZEOF LBRACKET IDENTIFIER RBRACKET { $$ = new Size_of(*$3); }
            | SIZEOF LBRACKET TYPE RBRACKET { $$ = new Size_of(*$3); }

STRUCT_IN : TYPE IDENTIFIER SEMICOLON{ $$ = new Struct_in(*$1, *$2); }
          | STRUCT_IN STRUCT_IN { $$ = new Succession($1, $2); }

STRUCT_DEFINE : STRUCT IDENTIFIER LCBRACKET STRUCT_IN RCBRACKET SEMICOLON { $$ = new Struct_define(*$2, $4); }

STRUCT_CONSTRUCT : STRUCT IDENTIFIER IDENTIFIER { $$ = new Struct_constructor(*$2, *$3); }


EXPR : ASSIGN { $$ = $1; } 
     | EXPR COMMA ASSIGN { $$ = new Succession($1, $3); }
     | DECLARE { $$ = $1; }
     | FUNCTION_CALL { $$ = $1; }
     | SIZEOF_FUNC { $$ = $1; }
     | RETURN_STATEMENT { $$ = $1;}
     | ACCESS_ARRAY { $$ = $1; }
     | STRUCT_CONSTRUCT { $$ = $1; }
     ;

ASSIGN : STRUCT_GET EQUAL MATH_EXPRESSION { fprintf(stderr, "heeeere %s\n", $1->getId().c_str());$$ =new Succession( $1, new EqualOperator(new Identifier(($1->getId())), $3)); }
       | STRUCT_GET PLUS EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new PlusEqualOperator(new Identifier($1->getId()), $4)); }
       | STRUCT_GET MINUS EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new MinusEqualOperator(new Identifier($1->getId()), $4)); }
       | STRUCT_GET DIVIDE EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new DivideEqualOperator(new Identifier($1->getId()), $4)); }
       | STRUCT_GET MODULO EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new ModEqualOperator(new Identifier($1->getId()), $4)); }
       | STRUCT_GET TIMES EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new MulEqualOperator(new Identifier($1->getId()), $4)); }
       | STRUCT_GET LESS LESS EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new LeftShiftEqualOperator(new Identifier($1->getId()), $5)); }
       | STRUCT_GET GREATER GREATER EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new RightShiftEqualOperator(new Identifier($1->getId()), $5)); }
       | STRUCT_GET AND EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new AndEqualOperator(new Identifier($1->getId()), $4)); }
       | STRUCT_GET XOR EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new XorEqualOperator(new Identifier($1->getId()), $4)); }
       | STRUCT_GET OR EQUAL MATH_EXPRESSION { $$ =new Succession( $1, new OrEqualOperator(new Identifier($1->getId()), $4)); }
       | IDENTIFIER EQUAL MATH_EXPRESSION { fprintf(stderr, "recognises equal\n");$$ = new EqualOperator(new Identifier(*$1), $3); }
       | IDENTIFIER PLUS EQUAL MATH_EXPRESSION { $$ = new PlusEqualOperator(new Identifier(*$1), $4); }
       | IDENTIFIER MINUS EQUAL MATH_EXPRESSION { $$ = new MinusEqualOperator(new Identifier(*$1), $4); }
       | IDENTIFIER DIVIDE EQUAL MATH_EXPRESSION { $$ = new DivideEqualOperator(new Identifier(*$1), $4); }
       | IDENTIFIER MODULO EQUAL MATH_EXPRESSION { $$ = new ModEqualOperator(new Identifier(*$1), $4); }
       | IDENTIFIER TIMES EQUAL MATH_EXPRESSION { $$ = new MulEqualOperator(new Identifier(*$1), $4); }
       | IDENTIFIER LESS LESS EQUAL MATH_EXPRESSION { $$ = new LeftShiftEqualOperator(new Identifier(*$1), $5); }
       | IDENTIFIER GREATER GREATER EQUAL MATH_EXPRESSION { $$ = new RightShiftEqualOperator(new Identifier(*$1), $5); }
       | IDENTIFIER AND EQUAL MATH_EXPRESSION { $$ = new AndEqualOperator(new Identifier(*$1), $4); }
       | IDENTIFIER XOR EQUAL MATH_EXPRESSION { $$ = new XorEqualOperator(new Identifier(*$1), $4); }
       | IDENTIFIER OR EQUAL MATH_EXPRESSION { $$ = new OrEqualOperator(new Identifier(*$1), $4); }
       | ACCESS_ARRAY EQUAL MATH_EXPRESSION { fprintf(stderr, "recognises array\n"); $$ = new EqualOperator($1, $3); }
       | ACCESS_ARRAY PLUS EQUAL MATH_EXPRESSION { $$ = new PlusEqualOperator($1, $4); }
       | ACCESS_ARRAY MINUS EQUAL MATH_EXPRESSION { $$ = new MinusEqualOperator($1, $4); }
       | ACCESS_ARRAY DIVIDE EQUAL MATH_EXPRESSION { $$ = new DivideEqualOperator($1, $4); }
       | ACCESS_ARRAY MODULO EQUAL MATH_EXPRESSION { $$ = new ModEqualOperator($1, $4); }
       | ACCESS_ARRAY TIMES EQUAL MATH_EXPRESSION { $$ = new MulEqualOperator($1, $4); }
       | ACCESS_ARRAY LESS LESS EQUAL MATH_EXPRESSION { $$ = new LeftShiftEqualOperator($1, $5); }
       | ACCESS_ARRAY GREATER GREATER EQUAL MATH_EXPRESSION { $$ = new RightShiftEqualOperator($1, $5); }
       | ACCESS_ARRAY AND EQUAL MATH_EXPRESSION { $$ = new AndEqualOperator($1, $4); }
       | ACCESS_ARRAY XOR EQUAL MATH_EXPRESSION { $$ = new XorEqualOperator($1, $4); }
       | ACCESS_ARRAY OR EQUAL MATH_EXPRESSION { $$ = new OrEqualOperator($1, $4); }
       | MATH_EXPRESSION { fprintf(stderr, "new MATH_EXPRESSION\n"); $$ = $1;}
       ;


STRUCT_GET : IDENTIFIER DOT IDENTIFIER { fprintf(stderr, "new get\n"); $$ = new Struct_get(*$1, *$3); }

ACCESS_ARRAY : IDENTIFIER LSBRACKET MATH_EXPRESSION RSBRACKET { fprintf(stderr, "new acces to array\n"); $$ = new Access_Array(*$1, $3); }

DECLARE : TYPE ASSIGN  { $$ = new Succession(new Declare(*$1, $2->getId()), $2); }
        | TYPE IDENTIFIER { $$ = new Declare(*$1, *$2); }
        | IDENTIFIER IDENTIFIER { $$ = new Declare(*$1, *$2); }
        | DECLARE COMMA ASSIGN { $$ = new Succession($1, $3); }
        ;

MATH_EXPRESSION : OR_OP { $$ = $1; }
                ;

OR_OP : OR_OP DOR AND_OP { $$ = new OrOperator($1, $3); }
      | AND_OP { $$ = $1; }
      ;

AND_OP : AND_OP DAND BITWISE_OR  { $$ = new AndOperator($1, $3); }
       | BITWISE_OR  { $$ = $1; }
       ;

BITWISE_OR : BITWISE_OR OR BITWISE_XOR  { $$ = new BitwiseOrOperator($1, $3); }
           | BITWISE_XOR { $$ = $1; }
           ;

BITWISE_XOR : BITWISE_XOR XOR BITWISE_AND { $$ = new BitwiseXorOperator($1, $3); }
            | BITWISE_AND { $$ = $1; }
            ;

BITWISE_AND : BITWISE_AND AND SHIFTERS  { $$ = new BitwiseAndOperator($1, $3); }
            | EQUAL_COMPARISON { $$ = $1; }
            ;

EQUAL_COMPARISON : EQUAL_COMPARISON EQUAL EQUAL COMPARISON  { $$ = new EqualCompOperator($1, $4); } 
                 | EQUAL_COMPARISON EXCLMARK EQUAL COMPARISON { $$ = new NotEqualCompOperator($1, $4); }
                 | COMPARISON {fprintf(stderr, "new equal_comp\n"); $$ = $1; }
                 ;

COMPARISON : SHIFTERS { fprintf(stderr, "new shifter\n");$$ = $1; }
           | SHIFTERS LESS SHIFTERS { fprintf(stderr, "new less\n");$$ = new LesserCompOperator($1, $3); }
           | SHIFTERS GREATER SHIFTERS { fprintf(stderr, "new greater\n"); $$ = new GreaterCompOperator($1, $3); }
           | SHIFTERS LESS EQUAL SHIFTERS { $$ = new LesserEqualCompOperator($1, $4); }
           | SHIFTERS GREATER EQUAL SHIFTERS { $$ = new GreaterEqualCompOperator($1, $4); }
           ;

SHIFTERS : ADDSUB { fprintf(stderr, "new addsub\n");$$ = $1; }
         | ADDSUB SHIFT_L ADDSUB { $$ = new LeftShiftOperator($1, $3); }
         | ADDSUB SHIFT_R ADDSUB { $$ = new RightShiftOperator($1, $3); }
         ;

ADDSUB : ADDSUB PLUS TERM { $$ = new AddOperator($1, $3); }
       | ADDSUB PLUS ACCESS_ARRAY { $$ = new AddOperator($1, $3); }
       | ADDSUB MINUS TERM  { $$ = new SubOperator($1, $3); }
       | TERM { fprintf(stderr, "new term\n");$$ = $1; }
       ;

TERM : TERM TIMES UNARY { $$ = new MulOperator($1, $3); }
     | TERM DIVIDE UNARY { $$ = new DivOperator($1, $3); }
     | TERM MODULO UNARY { $$ = new ModOperator($1, $3); }
     | UNARY {fprintf(stderr, "new unary\n");$$ = $1; }
     ;

UNARY : MINUS FACTOR { $$ = new NegOperator($2); }
      | PLUS FACTOR  { $$ = new PosOperator($2); }
      | BITFLIP FACTOR { $$ = new ComplOperator($2); }
      | DPLUS IDENTIFIER { $$ = new PosIterateOperator(*$2); }
      | DMINUS IDENTIFIER { $$ = new NegIterateOperator(*$2); }
      | IDENTIFIER DPLUS { $$ = new PostPosIterateOperator(*$1); }
      | IDENTIFIER DMINUS { $$ = new PostNegIterateOperator(*$1); }
      | EXCLMARK UNARY { $$ = new NotOperator($2); }
      | FACTOR {fprintf(stderr, "new factor\n"); $$ = $1; }
      ;

FACTOR : NUMBER { fprintf(stderr, "new num\n");$$ = new Number($1); }
    | NUMBER_F { $$ = new Float($1); }
    | LBRACKET MATH_EXPRESSION RBRACKET { $$ = $2; }
    | IDENTIFIER { fprintf(stderr, "new id\n");$$ = new Identifier(*$1); }
    | AND IDENTIFIER { $$ = new Address_of(*$2); }
    | STRUCT_GET { $$ = new Succession($1, new Identifier(($1->getId()))); }
    | FUNCTION_CALL { $$ = $1; }
    ;

FUNCTION_CALL : IDENTIFIER LBRACKET CALL_ARGS RBRACKET { $$ = new Function_call(*$1, $3); }
              | IDENTIFIER LBRACKET RBRACKET { $$ = new Function_call(*$1, new Nop()); }
;

CALL_ARGS : MATH_EXPRESSION { $$ = $1; }
          | CALL_ARGS COMMA CALL_ARGS { $$ = new Succession($1, $3); }

TYPE : INT { fprintf(stderr, "INT \n"); $$ = new std::string("int"); }
     | FLOAT { $$ = new std::string("float"); }
     | DOUBLE { $$ = new std::string("double"); }
     | CHAR { $$ = new std::string("char"); }
     | UNSIGNED { $$ = new std::string("int"); }
     | INT TIMES { fprintf(stderr, "POINTING\n"); $$ = new std::string("* int"); }
;

%%

const Node *g_root; // Definition of variable (to match declaration earlier)

const Node *parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}