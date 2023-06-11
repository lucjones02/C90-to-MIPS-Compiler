#ifndef ast_enum_hpp
#define ast_enum_hpp

#include "ast_node.hpp"
#include <string>

class Enum_statement : public Node{
private:
    std::string name;
    NodePtr code;

public:
    Enum_statement(std::string _name, NodePtr _code):
        name(_name),
        code(_code){
            stack_size=0;
            unique=0;
        }

    virtual std::string get_class() const override{
        return "Enum_statement";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class Enum_expression : public Node{
private:
    std::string id;
    int value;
public:
    Enum_expression(std::string _id, int _value):
        id(_id),
        value(_value){
            stack_size = 0;
            unique = 0;
        }
    
    virtual std::string get_class() const override{
        return "Enum_expression";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

#endif