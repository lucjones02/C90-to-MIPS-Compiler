#include <iostream>
#include "ast.hpp"

void Succession::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    
    first->compile(dst, lookup, res_register, total_block_size, i);
    second->compile(dst, lookup, res_register, total_block_size-first->get_stack_size(), i);
}