#include "ast.hpp"

#include <iostream>
#include <string>
#include <unordered_map>

void Return::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    std::string function_name = i.at("function_name");
    std::string type = i.at(function_name);
    int reg=0;
    if(type=="int"){
        reg=2;
    }
    else if(type=="float"){
        reg = 0;
    }
    value->compile(dst, lookup, reg, stack_size, i);
    dst << "\tb\tEND_OF_FUNCTION_" << function_name << std::endl;
}

void If::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{

    // compile the code for the condition and store it in register 2
    condition->compile(dst, lookup, 2, total_block_size, i);
    
    // using these to make labels unique
    int end_num = unique;
    int else_num = unique-1;
    int unique_c = std::stoi(i["unique_c"]);
    i["unique_c"] = std::to_string(unique_c+1);

    //using the function name to make the labels function specific
    std::string function_name = i.at("function_name");

    // the compiled will be different if we have an else or not
    if(elsecode->get_class()=="Nop"){
        // compilation of the code if we don't have an else
        dst << "\tnop" << std::endl; // having a nop in case the condition needs to lw
        dst << "\tbeq\t$2,$0,$END" << function_name << unique_c << end_num << std::endl; // register 2 will be 1 if the condition is true
        dst << "\tnop" << std::endl;

        //compile the code if the consition is true
        ifcode->compile(dst, lookup, res_register, total_block_size, i);

    }
    else{
        // compilation of the code if there is an else statement
        dst << "\tnop" << std::endl;
        dst << "\tbeq\t$2,$0,$ELSE" << function_name << unique_c << else_num << std::endl;
        dst << "\tnop" << std::endl;

        //compile the code if the consition is true
        ifcode->compile(dst, lookup, res_register, total_block_size, i);

        dst << "\tnop" << std::endl;
        dst << "\tb\t$END" << function_name << unique_c << end_num << std::endl;
        dst << "\tnop" << std::endl;

        dst << "\t$ELSE" << function_name << unique_c << else_num << ":" << std::endl;

        //compile the code if the condition is false
        elsecode->compile(dst, lookup, res_register, total_block_size, i);
        dst << "\tnop" << std::endl;

    }
    dst << "\t$END" << function_name << unique_c << end_num << ":" << std::endl;
}

void While::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    // using these to make labels unique
    int start_num = unique;
    int end_num = unique-1;
    int unique_c = std::stoi(i["unique_c"]);
    i["unique_c"] = std::to_string(unique_c+1);
    //using the function name to make the labels function specific
    std::string function_name = i.at("function_name");
    i["current_loop_end"] = "$END" + function_name + std::to_string(unique_c) + std::to_string(end_num);
    i["current_loop_start"] = "$START" + function_name + std::to_string(unique_c) + std::to_string(start_num);


    dst << "\t$START" << function_name << unique_c << start_num << ":" << std::endl; //create a label for the start
    
    // compile the condition
    condition->compile(dst, lookup, 2, total_block_size, i);

    dst << "\tnop" << std::endl;
    dst << "\tbeq\t$2,$0,$END" << function_name << unique_c << end_num << std::endl;
    dst << "\tnop" << std::endl;

    code->compile(dst, lookup, res_register, total_block_size, i);
    dst << "\tnop" << std::endl;
    dst << "\tb\t$START" << function_name << unique_c << start_num << std::endl;

    dst << "\tnop" << std::endl;

    dst << "\t$END" << function_name << unique_c << end_num << ":" << std::endl;
    
}

void For::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    int start_num = unique;
    int end_num = unique-1;
    int unique_c = std::stoi(i["unique_c"]);
    i["unique_c"] = std::to_string(unique_c+1);
    std::string function_name = i.at("function_name");
    i["current_loop_end"] = "$END" + function_name + std::to_string(unique_c) + std::to_string(end_num);
    i["current_loop_start"] = "$ITERATE" + function_name + std::to_string(unique_c) + std::to_string(start_num);

    declare->compile(dst, lookup, res_register, total_block_size, i);

    dst << "\t$START" << function_name << unique_c << start_num << ":" << std::endl; //create a label for the start

    condition->compile(dst, lookup, 2, total_block_size, i);

    dst << "\tnop" << std::endl;
    dst << "\tbeq\t$2,$0,$END" << function_name << unique_c << end_num << std::endl;
    dst << "\tnop" << std::endl;

    code->compile(dst, lookup, res_register, total_block_size, i);
    dst << "\t$ITERATE" << function_name << unique_c << start_num << ":" << std::endl; //create a label for the iteration
    iterator->compile(dst, lookup, res_register, total_block_size, i);
    dst << "\tnop" << std::endl;
    dst << "\tb\t$START" << function_name << unique_c << start_num << std::endl;


    dst << "\tnop" << std::endl;

    dst << "\t$END" << function_name << unique_c << end_num << ":" << std::endl;

}

void Switch::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    
    std::string function_name = i.at("function_name");
    i["current_loop_end"] = "$ENDSWITCH";

    // int caseno;

    // if(cases->get_class()=="Nop"){
    //     caseno = 0;
    // }
    // else{
    //     NodePtr next_case = cases;
    //     caseno = 1;
    //     while(next_case->get_class()=="Succession"){
    //         next_case = next_case->get_second_succession();
    //         caseno += 1;
    //     }
    // }

    expression->compile(dst, lookup, 17, total_block_size, i);
    cases->compile(dst, lookup, 8, total_block_size, i);
    dst << "\t$ENDSWITCH:" << std::endl;

}

void Case::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    int skip_num = unique-1;
    int unique_c = std::stoi(i["unique_c"]);
    i["unique_c"] = std::to_string(unique_c+1);
    std::string function_name = i.at("function_name");

    compared->compile(dst, lookup, 8, total_block_size, i);

    dst << "\tnop" << std::endl;
    dst << "\tbne\t$" << 8 << ",$" << 17 << ",$SKIP" << function_name << unique_c << skip_num << std::endl;

    code->compile(dst, lookup, res_register, total_block_size, i);

    dst << "\t$SKIP" << function_name << unique_c << skip_num << ":" << std::endl;
}

void Break::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    std::string label = i.at("current_loop_end");
    dst << "\tb\t" << label << std::endl;
}

void Continue::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    std::string label =  i.at("current_loop_start");
    dst << "\tb\t" << label << std::endl;
  
}

void Typedef::compile(std::ostream &dst, lt &lookup, int res_register, int total_block_size, info& i) const{
    i.insert(std::make_pair(new_type, type));
}