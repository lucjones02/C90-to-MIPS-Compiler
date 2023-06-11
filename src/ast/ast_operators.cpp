#include <ast.hpp>

#include <string>
#include <map>
#include <iostream>

void AddOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    std::string type;
    if(right->get_class()=="Identifier"){
        type = i.at(right->getId());
    }
    else if(left->get_class()=="Identifier"){
        type = i.at(left->getId());
    }
    else{
        type = "int";
    }

    if(type=="int"){

        if(right->get_class()=="Access_Array"){
            right->compile(dst, lookup, 16, total_block_size-left->get_stack_size(), i);
            left->compile(dst, lookup, 8, total_block_size, i);
            
            dst << "\tnop" << std::endl;
            dst << "\taddu\t$" << res_register << ",$" << 8 << ",$" << 2 << std::endl;
        }
        else if(left->get_class()=="Access_Array"){
            left->compile(dst, lookup, 16, total_block_size, i);
            right->compile(dst, lookup, 8, total_block_size-left->get_stack_size(), i);
            
            dst << "\tnop" << std::endl;
            dst << "\taddu\t$" << res_register << ",$" << 8 << ",$" << 2 << std::endl;

        }
        else{
            if(right->get_function_call()){        
                right->compile(dst, lookup, 8, total_block_size, i);
                left->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);
            }
            else{
                left->compile(dst, lookup, 9, total_block_size, i);
                right->compile(dst, lookup, 8, total_block_size-left->get_stack_size(), i);
            }
            
            dst << "\tnop" << std::endl;
            dst << "\taddu\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
        }
    }
    else if(type=="float"){
        if(right->get_class()=="Identifier"||right->get_class()=="Number"){
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
            right->compile(dst, lookup, 8, total_block_size, i);     
        }
        else{
            right->compile(dst, lookup, 8, total_block_size, i);
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
        }
        dst << "\tnop" << std::endl;
        dst << "\tadd.s\t$f" << res_register << ",$f" << 8 << ",$f" << 10 << std::endl;        
    }
    else if(type=="double"){
        if(right->get_class()=="Identifier"||right->get_class()=="Number"){
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
            right->compile(dst, lookup, 8, total_block_size, i);     
        }
        else{
            right->compile(dst, lookup, 8, total_block_size, i);
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
        }
        dst << "\tnop" << std::endl;
        dst << "\tadd.d\t$f" << res_register << ",$f" << 8 << ",$f" << 10 << std::endl;        
    }
    else if(type=="* int"){
        right->compile(dst, lookup, 8, total_block_size, i);
        left->compile(dst, lookup, 9, total_block_size-right->get_stack_size(), i);
        dst << "\tnop" << std::endl;
        if(right->get_class()=="Identifier"||right->get_class()=="Number"){
            dst << "\tsll\t$" << 8 << ",$" << 8 << ",2" << std::endl;
        }
        else if(left->get_class()=="Identifier"||left->get_class()=="Number"){
            dst << "\tsll\t$" << 9 << ",$" << 9 << ",2" << std::endl;
        }
        dst << "\taddu\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
    }
    
}

void SubOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    std::string type;
    if(right->get_class()=="Identifier"){
        type = i.at(right->getId());
    }
    else if(left->get_class()=="Identifier"){
        type = i.at(left->getId());
    }
    else{
        type = "int";
    }
    
    if(type=="int"){
        if(right->get_class()=="Access_Array"){
            right->compile(dst, lookup, 16, total_block_size-left->get_stack_size(), i);
            left->compile(dst, lookup, 8, total_block_size, i);
            
            dst << "\tnop" << std::endl;
            dst << "\tsubu\t$" << res_register << ",$" << 8 << ",$" << 2 << std::endl;
        }
        else if(left->get_class()=="Access_Array"){
            left->compile(dst, lookup, 16, total_block_size, i);
            right->compile(dst, lookup, 8, total_block_size-left->get_stack_size(), i);
            
            dst << "\tnop" << std::endl;
            dst << "\tsubu\t$" << res_register << ",$" << 2 << ",$" << 8 << std::endl;

        }
        else{
            if(right->get_function_call()){        
                right->compile(dst, lookup, 9, total_block_size, i);
                left->compile(dst, lookup, 8, total_block_size-left->get_stack_size(), i);
                dst << "\tnop" << std::endl;
                dst << "\tsubu\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
            }
            else{
                left->compile(dst, lookup, 8, total_block_size, i);
                right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);
                dst << "\tnop" << std::endl;
                dst << "\tsubu\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
            }

        }
    }
    else if(type == "float"){
        if(right->get_class()=="Identifier"||right->get_class()=="Number"){
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
            right->compile(dst, lookup, 8, total_block_size, i);     
        }
        else{
            right->compile(dst, lookup, 8, total_block_size, i);
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
        }
        dst << "\tnop" << std::endl;
        dst << "\tsub.s\t$f" << res_register << ",$f" << 10 << ",$f" << 8 << std::endl;        
    }
    else if(type=="double"){
        if(right->get_class()=="Identifier"||right->get_class()=="Number"){
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
            right->compile(dst, lookup, 8, total_block_size, i);     
        }
        else{
            right->compile(dst, lookup, 8, total_block_size, i);
            left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
        }
        dst << "\tnop" << std::endl;
        dst << "\tsub.d\t$f" << res_register << ",$f" << 10 << ",$f" << 8 << std::endl;        
    }

        
}

void MulOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    std::string type;
    if(right->get_class()=="Identifier"){
        type = i.at(right->getId());
    }
    else if(left->get_class()=="Identifier"){
        type = i.at(left->getId());
    }
    else{
        type = "int";
    }

    if(type=="int"){
        left->compile(dst, lookup, 8, total_block_size, i);
        right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);
        
        dst << "\tnop" << std::endl;
        dst << "\tmult\t$" << 8 << ",$" << 9 << std::endl;
        dst << "\tmflo\t$" << res_register << std::endl;
    }
    else if(type == "float"){
        right->compile(dst, lookup, 8, total_block_size, i);
        left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
        dst << "\tnop" << std::endl;
        dst << "\tmul.s\t$f" << res_register << ",$f" << 8 << ",$f" << 10 << std::endl;       
    }
    else if(type == "double"){
        right->compile(dst, lookup, 8, total_block_size, i);
        left->compile(dst, lookup, 10, total_block_size-left->get_stack_size(), i);
        dst << "\tnop" << std::endl;
        dst << "\tmul.d\t$f" << res_register << ",$f" << 8 << ",$f" << 10 << std::endl;       
    }
    // godbolt only has mflo, not sure if we need mfhi, and if we do not sure how it'd be implemented 
}

void DivOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);
    
    dst << "\tnop" << std::endl;
    dst << "\tdiv\t$" << 8 << ",$" << 9 << std::endl;
    dst << "\tmflo\t$" << res_register << std::endl;
}

void ModOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);
    
    dst << "\tnop" << std::endl;
    dst << "\tdiv\t$" << 8 << ",$" << 9 << std::endl;
    dst << "\tmfhi\t$" << res_register << std::endl;
}

void AndOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);
    
    dst << "\tnop" << std::endl;
    dst << "\tsltu\t$" << 8 << ",$0,$" << 8 << std::endl;
    dst << "\tsltu\t$" << 9 << ",$0,$" << 9 << std::endl;
    dst << "\tand\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
}

void OrOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);
    
    dst << "\tnop" << std::endl;
    dst << "\tsltu\t$" << 8 << ",$0,$" << 8 << std::endl;
    dst << "\tsltu\t$" << 9 << ",$0,$" << 9 << std::endl;
    dst << "\tor\t$" << res_register << ",$" << 9 << ",$" << 8 << std::endl;
}

void EqualCompOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\txor\t$" << 9 << ",$" << 8 << ",$" << 9 << std::endl;
    dst << "\tsltu\t$" << res_register << ",$"<< 9 << ",1" << std::endl;
}

void GreaterCompOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tslt\t$" << res_register << ",$" << 9 << ",$" << 8 << std::endl;
}

void LesserCompOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tslt\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
}

void NotEqualCompOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\txor\t$" << 9 << ",$" << 8 << ",$" << 9 << std::endl;
    dst << "\tsltu\t$" << res_register << ",$0" << ",$" << 9 << std::endl;
}

void GreaterEqualCompOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tslt\t$" << 8 << ",$" << 9 << ",$" << 8 << std::endl;
    dst << "\txori\t$" << res_register << ",$" << 8 << ",0x1" << std::endl;
}

void LesserEqualCompOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tslt\t$" << 9 << ",$" << 9 << ",$" << 8 << std::endl;
    dst << "\txori\t$" << 9 << ",$" << 9 << ",0x1" << std::endl;
    dst << "\tandi\t$2,$" << 9 << ",0x00ff" << std::endl;
}

void BitwiseAndOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tand\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
}

void BitwiseOrOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tor\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
}

void BitwiseXorOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\txor\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
}

void LeftShiftOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tsll\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
}

void RightShiftOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    left->compile(dst, lookup, 8, total_block_size, i);
    right->compile(dst, lookup, 9, total_block_size-left->get_stack_size(), i);

    dst << "\tnop" << std::endl;
    dst << "\tsra\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
}

void EqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();
        std::string type = i.at(var);
        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        if(i.find("function_name")==i.end()){
            dst << "\t.word\t";
            code->compile(dst, lookup, res_register, total_block_size-tmp, i);
        }
        else{
            if (type=="int"){
                code->compile(dst, lookup, res_register, total_block_size-tmp, i);
                dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
            }
            else if(type == "float"){
                code->compile(dst, lookup, res_register, total_block_size-tmp, i);
                dst << "\tswc1\t$f" << res_register << "," << offset << "($fp)" << std::endl;
            }
            else if(type=="double"){
                code->compile(dst, lookup, res_register, total_block_size-tmp, i);
                dst << "\tsdc1\t$f" << res_register << "," << offset << "($fp)" << std::endl;
            }
            else if(type=="* int"){
                code->compile(dst, lookup, res_register, total_block_size-tmp, i);
                dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
            }
        }
    }
    else{
        id->compile(dst, lookup, 16, total_block_size, i);
        code->compile(dst, lookup, 2, total_block_size, i);
        dst << "\tsw\t$" << res_register << "," << 0 << "($16)" << std::endl;
    }

}

void PlusEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
            var = id->getId();


        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tadd\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void MinusEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tsub\t$" << res_register << ",$" << 8 << ",$" << 9 << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void MulEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tmult\t$" << 8 << ",$" << 9 << std::endl;
        dst << "\tmflo\t$" << res_register << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void DivideEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tdiv\t$" << 9 << ",$" << 8 << std::endl;
        dst << "\tmflo\t$" << res_register << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void ModEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tdiv\t$" << 9 << ",$" << 8 << std::endl;
        dst << "\tmfhi\t$" << res_register << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void LeftShiftEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tsll\t$" << res_register << ",$" << 9 << ",$" << 8 << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void RightShiftEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tsra\t$" << res_register << ",$" << 9 << ",$" << 8 << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void AndEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tand\t$" << res_register << ",$" << 9 << ",$" << 8 << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void OrEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\tor\t$" << res_register << ",$" << 9 << ",$" << 8 << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void XorEqualOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    int tmp;
    int offset;
    std::string var;

    if(id->get_class()=="Identifier"){
        var = id->getId();

        if(lookup.find(var) != lookup.end()){
            tmp = 0;
            offset = lookup.at(var);
        }
        else {
            tmp = 4;
            offset = total_block_size;
            lookup.insert(std::make_pair(var, offset));
        }
        code->compile(dst, lookup, 8, total_block_size-tmp, i);
        dst << "\tlw\t$" << 9 << "," << offset << "($fp)" << std::endl;
        dst << "\txor\t$" << res_register << ",$" << 9 << ",$" << 8 << std::endl;
        dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    }
}

void NegOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    node->compile(dst, lookup, 8, total_block_size, i);

    dst << "\tnop" << std::endl;
    dst << "\tsubu\t$" << res_register << ",$0,$" << 8 << std::endl;
}

void PosOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    node->compile(dst, lookup, res_register, total_block_size, i);

    dst << "\tnop" << std::endl;
}

void ComplOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    node->compile(dst, lookup, 8, total_block_size, i);

    dst << "\tnop" << std::endl;
    dst << "\tnot\t$" << res_register << ",$" << 8 << std::endl;
}

void PosIterateOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    //node->compile(dst, lookup, 8, total_block_size, i);
    
    int offset = lookup.at(id);

    // doesnt work, need to fix
    dst << "\tnop" << std::endl;
    dst << "\tlw\t$" << 8 << "," << offset << "($fp)" << std::endl;
    dst << "\taddiu\t$" << res_register << ",$" << 8 << ",1" << std::endl;
    dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
}

void NegIterateOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    //node->compile(dst, lookup, 8, total_block_size, i);
    
    int offset = lookup.at(id);

    // doesnt work, need to fix
    dst << "\tnop" << std::endl;
    dst << "\tlw\t$" << 8 << "," << offset << "($fp)" << std::endl;
    dst << "\taddiu\t$" << res_register << ",$" << 8 << ",-1" << std::endl;
    dst << "\tsw\t$" << res_register << "," << offset << "($fp)" << std::endl;
}

void PostPosIterateOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    //node->compile(dst, lookup, 8, total_block_size, i);
    
    int offset = lookup.at(id);

    // doesnt work, need to fix
    dst << "\tnop" << std::endl;
    dst << "\tlw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    dst << "\taddiu\t$" << 9 << ",$" << res_register << ",1" << std::endl;
    dst << "\tsw\t$" << 9 << "," << offset << "($fp)" << std::endl;
}

void PostNegIterateOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    //node->compile(dst, lookup, 8, total_block_size, i);
    
    int offset = lookup.at(id);

    // doesnt work, need to fix
    dst << "\tnop" << std::endl;
    dst << "\tlw\t$" << res_register << "," << offset << "($fp)" << std::endl;
    dst << "\taddiu\t$" << 9 << ",$" << res_register << ",-1" << std::endl;
    dst << "\tsw\t$" << 9 << "," << offset << "($fp)" << std::endl;
}

void NotOperator::compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
    node->compile(dst, lookup, 8, total_block_size, i);

    dst << "\tnop" << std::endl;
    dst << "\tsltu\t$" << 9 << ",$" << 8 << ",1" << std::endl;
    dst << "\tandi\t$" << res_register << ",$" << 9 << ",0x00ff" << std::endl;
}
