#include "ast.hpp"

#include <string>
#include <map>
#include <iostream>

void Function::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& inf) const{
    // adding the function name to the info passed down
    info new_info = inf;
    lt new_lookup = lookup;
    new_info["function_name"] = name;
    new_info[name] = type;
    new_info["unique_c"] = std::to_string(0);
    bool function_call = code->get_function_call();
    int tmp = 4;

    int arguments;
    if(params->get_class()=="Nop"){
        arguments = 0;
    }
    else{
        NodePtr next_argument = params;
        arguments = 1;
        while(next_argument->get_class()=="Succession"){
            next_argument = next_argument->get_second_succession();
            arguments += 1;
        }
    }


    // declaring the function as global for the linker
    if(c_brackets){
        dst << ".global\t" << name << std::endl;
        dst << std::endl;
        dst << name << ":" << std::endl;
        dst << "\taddiu\t$sp,$sp," << -stack_size << std::endl;
        // if there is a function call we want to store the return address
        if(function_call){
            dst << "\tsw\t$31," << stack_size-tmp << "($sp)" << std::endl;
            tmp += 4;
        }
        dst << "\tsw\t$fp," << stack_size-tmp << "($sp)" << std::endl;
        dst << "\tmove\t$fp,$sp" << std::endl;
        params->compile(dst, new_lookup, res_register, stack_size, new_info);
        if(arguments<=4){
            NodePtr next_argument = params;
            for(int i=0; i<arguments; i++){
                std::string type;
                if(next_argument->get_class()=="Declare"){
                    type = new_info.at(next_argument->getId());
                    if(type=="double"){
                        new_lookup.insert(std::make_pair(next_argument->getId(), stack_size + i*8));
                    }
                    else{
                        new_lookup.insert(std::make_pair(next_argument->getId(), stack_size + i*4));
                    }
                }
                else{
                    new_lookup.insert(std::make_pair(next_argument->get_first_succession()->getId(), stack_size+i*4));
                    type=new_info.at(next_argument->get_first_succession()->getId());
                    next_argument=next_argument->get_second_succession();
                }
                if ((type=="int")||(i>1)){
                    dst << "\tsw\t$" << 4+i << "," << stack_size + i*4 << "($fp)" << std::endl;
                }
                else if(type=="* int"){
                    dst << "\tsw\t$" << 4+i << "," << stack_size + i*4 << "($fp)" << std::endl;
                }
                else if (type=="float"){
                    dst << "\tswc1\t$f" << (4+i)*2 + 4 << "," << stack_size +i*4 << "($fp)" << std::endl;
                }
                else if(type=="double"){
                    dst << "\tsdc1\t$f" << (4+i)*2 +4 << "," << stack_size + i*8 << "($fp)" << std::endl;
                }
            }
        }

        // compiling the code inside the function
        code->compile(dst, new_lookup, res_register, stack_size-12, new_info);

        // implementing a label to mark the end of the function so that return statement can branch to it
        // we need it to be unique so that when we define multiple functions the end labels are different
        dst << "\tEND_OF_FUNCTION_" << name << ":" <<  std::endl;

        dst << "\tmove\t$sp,$fp" << std::endl;
        //TODO: add the line if there is a function call inside the function
        dst << "\tlw\t$fp," << stack_size-tmp << "($sp)" << std::endl;
        if(function_call){
            tmp -=4 ;
            dst << "\tlw\t$31," << stack_size-tmp << "($sp)" << std::endl;
        }
        dst << "\taddiu\t$sp,$sp," << stack_size << std::endl;
        dst << "\tjr\t$31" << std::endl;
        dst << "\tnop" << std::endl;
    }
    
}

void Function_call::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& inf) const{ 
    int arguments;
    if(params->get_class()=="Nop"){
        arguments = 0;
    }
    else{
        NodePtr next_argument = params;
        arguments = 1;
        while(next_argument->get_class()=="Succession"){
            next_argument = next_argument->get_second_succession();
            arguments += 1;
        }
    }

    if(arguments<=4){
        NodePtr next_argument = params;
        for(int i=0; i<arguments; i++){
            if(next_argument->get_class()!="Succession"){
                next_argument->compile(dst, lookup, 2, total_block_size, inf);
                dst << "\taddiu\t$" << 4+i << ",$2,0" << std::endl;
            }
            else{
                next_argument->get_first_succession()->compile(dst, lookup, 2, total_block_size, inf);
                dst << "\taddiu\t$" << 4+i << ",$2,0" << std::endl;
                next_argument=next_argument->get_second_succession();
            }
        }
    }
    
    // without the .option the assembler was throwing a warning 
    // so I compiled the call_constant_internal with the build in gnu-gcc
    // and found that the jal had .option before and afte
    dst << "\t.option\tpic0" << std::endl;
    dst << "\tjal\t" << name << std::endl;
    dst << "\tnop" << std::endl;
    dst << "\t.option\tpic2" << std::endl;
    if (res_register!=2) {
        dst << "\tmove\t$" << res_register << ",$2" << std::endl;
    }
}

void Size_of::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& inf) const{
    std::string type;
    if(inf.find(id)==inf.end()){
        type = id;
    }
    else{
        type = inf.at(id);
    }
    
    if(type=="int"){
        dst << "\tli\t$" << res_register << "," << 4 << std::endl;
    }
    else if(type=="float"){
        dst << "\tli\t$" << res_register << "," << 4 << std::endl;
    }
    else if(type=="double"){
        dst << "\tli\t$" << res_register << "," << 8 << std::endl;
    }
    else if(type=="char"){
        dst << "\tli\t$" << res_register << "," << 1 << std::endl;
    }
    else if(type.substr(0,6)=="struct"){
        std::string struct_name = type.substr(7,type.length()-7);
        dst << "\tli\t$" << res_register << "," << inf.at(struct_name+"_struct_size") << std::endl;
    }
}