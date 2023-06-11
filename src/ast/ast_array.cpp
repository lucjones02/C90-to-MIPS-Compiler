#include "ast.hpp"

#include <string>
#include <map>
#include <iostream>


void Declare_Array::compile(std::ostream& dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    bool global = i.find("function_name")==i.end();
    lookup.insert(std::make_pair(name, total_block_size));
    if(global){
        dst << "\t.comm\t" << name << "," << 4*size << "," << 4 << std::endl;
    }
}

void Access_Array::compile(std::ostream& dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int offset = lookup.at(name);
    element->compile(dst, lookup, res_register, total_block_size, i);
    
    dst << "\tsll\t$" << res_register << ",$" << res_register << ",2" << std::endl;
    dst << "\taddiu\t$" << res_register << ",$" << res_register << "," << -offset << std::endl;
    dst << "\taddu\t$" << res_register << ",$" << res_register << ",$fp" << std::endl;
    dst << "\tlw\t$2,0($" << res_register << ")" << std::endl;
}