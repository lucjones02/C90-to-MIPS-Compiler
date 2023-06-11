#ifndef ast_function_hpp
#define ast_function_hpp

#include "ast_node.hpp"
#include <string>

class Function: public Node{
private:
    std::string type;
    std::string name;
    NodePtr params;
    NodePtr code;
    bool c_brackets;
public:
    Function(std::string _type, std::string _name, NodePtr _params, NodePtr _code, bool _c_brackets):
        type(_type),
        name(_name),
        params(_params),
        code(_code),
        c_brackets(_c_brackets)
    {
        int tmp_size = 8;
        if (code->get_stack_size()==0){
            stack_size = 8;
        }
        else{
            tmp_size += code->get_stack_size();
            tmp_size += 8; // adding 4 because of sw $fp
            stack_size = (tmp_size%8) ? tmp_size+4 : tmp_size;
        }
        unique = code->get_unique();
    }

    virtual ~Function(){
        delete code;
    }

    virtual std::string get_class() const override{
        return "Function";
    }
    virtual std::string get_name() const{
        return name;
    }

    virtual bool get_function_call() const override{
        return code->get_function_call();
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class Function_call : public Node{
private:
    std::string name;
    NodePtr params;

public:
    Function_call(std::string _name, NodePtr _params) : 
        name(_name),
        params(_params)
    {
        stack_size = 16;
        unique = 0;
    }

    virtual ~Function_call(){};

    virtual std::string get_class() const override{
        return "Function_call";
    }

    virtual std::string get_name() const{
        return name;
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
    
    virtual bool get_function_call() const override{
        return true;
    }

};

class Size_of : public Node{
private:
    std::string id;

public:
    Size_of(std::string _id): id(_id){stack_size=0;unique=0;}

    virtual std::string get_class() const override{
        return "Size_of";
    }
    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};


#endif