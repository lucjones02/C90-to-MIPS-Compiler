#include "ast.hpp"

#include <string>
#include <map>
#include <iostream>

void Enum_statement::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    i["enum_c"] = "0";
    code->compile(dst, lookup, res_register, total_block_size, i);
}

void Enum_expression::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int c = std::stoi(i.at("enum_c"));
    int val = value;
    if(value == -1){
        val = c;
    }
    lookup.insert(std::make_pair(id+"_enum_", val));
    i.insert(std::make_pair(id, "int"));
}