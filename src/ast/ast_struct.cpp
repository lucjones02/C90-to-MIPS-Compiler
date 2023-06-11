#include "ast.hpp"

#include <string>
#include <map>
#include <iostream>

void Struct_define::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    i.insert(std::make_pair(name, "struct"));
    i.insert(std::make_pair("current_struct", name));
    i.insert(std::make_pair(name+"_struct_size", "0"));
    code->compile(dst, lookup, res_register, total_block_size, i);
    i.erase("current_struct");
}

void Struct_constructor::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    i.insert(std::make_pair(object_name, "struct_"+struct_name));
}

void Struct_in::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    std::string struct_name = i.at("current_struct");
    int current_size = std::stoi(i.at(struct_name+"_struct_size"));
    int add_size = 0;
    if(type == "int" || type=="float" ){
        add_size = 4;
    }
    else if (type == "double"){
        add_size = 8;
    }
    else if (type == "char"){
        add_size = 1;
    }
    i.insert(std::make_pair(struct_name+"_"+id, type));
    i[struct_name+"_struct_size"] = std::to_string(current_size+add_size);
}

void Struct_get::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    std::string id = i.at(object_name);
    std::string struct_name = id.substr(7, id.length());
    if(i.find("struct_"+object_name+"_"+attribute_name)==i.end()){
        i.insert(std::make_pair("struct_"+object_name+"_"+attribute_name, i.at(struct_name+"_"+attribute_name)));
    }
    if(lookup.find("struct_"+object_name+"_"+attribute_name)==lookup.end()){
        lookup.insert(std::make_pair("struct_"+object_name+"_"+attribute_name, total_block_size));
    }
}