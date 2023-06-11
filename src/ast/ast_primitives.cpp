#include "ast.hpp"

#include <string>
#include <iostream>
#include <map>

void Identifier::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    
    std::string type = i.at(id);

    if(type=="int"||type=="* int"){
        if(i.find("function_name")==i.end()){
            dst << id << ":" << std::endl;
        }
        else{
            int offset=total_block_size;
            if(lookup.find(id) != lookup.end()){
                offset = lookup.at(id);
                dst << "\tlw\t$" << res_register << "," << offset << "($fp)" << std::endl;
            }
            else if(lookup.find(id+"_enum_") != lookup.end()){
                dst << "\tli\t$" << res_register << "," << lookup.at(id+"_enum_") << std::endl;
            }
            else{
                dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
            }
        }
    }
    else if(type=="float"){
            int offset=total_block_size;
            if(lookup.find(id) != lookup.end()){
                offset = lookup.at(id);
                dst << "\tlwc1\t$f" << res_register << "," << offset << "($fp)" << std::endl;
            }
            else{
                dst << "\tswc1\t$f" << res_register << "," << offset << "($fp)" << std::endl;
            }       
    }
    else if(type=="double"){
            int offset=total_block_size;
            if(lookup.find(id) != lookup.end()){
                offset = lookup.at(id);
                dst << "\tldc1\t$f" << res_register << "," << offset << "($fp)" << std::endl;
            }
            else{
                dst << "\tsdc1\t$f" << res_register << "," << offset << "($fp)" << std::endl;
            }       

    }
}

void Declare::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
        if(i.find(id) == i.end()){
            if(i.find(type)==i.end()){
                i.insert(std::make_pair(id, type));
            }
            else{
                i.insert(std::make_pair(id, i.at(type)));
            }
        }
}

void Number::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    if(i.find("function_name")==i.end()){
        dst << value << std::endl;
    }
    else{
        dst << "\tli\t$" << res_register << "," << value << std::endl;
    }    
}

void Float::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    if(i.find("function_name")==i.end()){
        dst << value << std::endl;
    }
    else{
        dst << "\tli.s\t$f" << res_register << "," << value << std::endl;
    }    
}

