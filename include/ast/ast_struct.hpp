#ifndef ast_struct_hpp
#define ast_struct_hpp

#include "ast_node.hpp"
#include <iostream>

class Struct_define : public Node{
private:
    std::string name;
    NodePtr code;

public:
    Struct_define(std::string _name, NodePtr _code): name(_name), code(_code) {
        stack_size = code->get_stack_size();
        unique = code->get_unique();
    }

    virtual std::string get_class() const override{
        return "Struct_define";
    }
    
    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class Struct_constructor : public Node{
private:
    std::string struct_name;
    std::string object_name;

public:
    Struct_constructor(std::string _struct_name, std::string _object_name):
        struct_name(_struct_name),
        object_name(_object_name){
            stack_size = 0;
            unique = 0;
        }
    
    virtual std::string get_class() const override{
        return "Struct_define";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class Struct_in : public Node{
private: 
    std::string type;
    std::string id;

public:
    Struct_in(std::string _type, std::string _id): type(_type), id(_id){
        stack_size = 0;
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Struct_in";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class Struct_get : public Node{
private:
    std::string object_name;
    std::string attribute_name;

public:
    Struct_get(std::string _object_name, std::string _attribute_name):
    object_name(_object_name),
    attribute_name(_attribute_name){
        stack_size = 8;
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Struct_get";
    }

    virtual std::string getId() const override{
        return "struct_"+object_name+"_"+attribute_name;
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

#endif