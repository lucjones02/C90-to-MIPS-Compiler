#include "ast.hpp"

#include <string>
#include <map>
#include <iostream>

void Address_of::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    if(lookup.find(id)==lookup.end()){
        lookup.insert(std::make_pair(id, total_block_size));
    }
    int offset = lookup.at(id);
    dst << "\taddiu\t$" << res_register << ",$fp," << offset << std::endl;
}