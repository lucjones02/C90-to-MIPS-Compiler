#ifndef ast_hpp
#define ast_hpp

#include "ast/ast_array.hpp"
#include "ast/ast_enum.hpp"
#include "ast/ast_function.hpp"
#include "ast/ast_node.hpp"
#include "ast/ast_operators.hpp"
#include "ast/ast_pointer.hpp"
#include "ast/ast_primitives.hpp"
#include "ast/ast_statements.hpp"
#include "ast/ast_struct.hpp"
#include "ast/ast_unary.hpp"

const extern Node *parseAST();
extern void yyset_in(FILE *file);

#endif