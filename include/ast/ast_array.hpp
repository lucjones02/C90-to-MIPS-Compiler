#ifndef ast_array_hpp
#define ast_array_hpp

#include "ast_node.hpp"
#include <string>


class Declare_Array : public Node{
private:
    std::string name;
    int size;
    int dim;
public:
    
    Declare_Array(const std::string& _name, int _size, int _dim) : name(_name), size(_size), dim(_dim){
            stack_size = size*8;
            unique = 0;
    }
    
    virtual ~Declare_Array(){};

    virtual void change_size(int new_size) override{
        this->size *= new_size;
    }

    virtual void dim1() override{
        this->dim++;
    }

    virtual bool get_function_call() const override{
        return false;
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class Access_Array : public Node{
private:
    std::string name;
    NodePtr element;
public:
    Access_Array(std::string _name, NodePtr _element): name(_name), element(_element){
        stack_size = 0;
        unique = 0;
    }

    virtual ~Access_Array(){
        delete element;
    }

    virtual std::string get_class() const override{
        return "Access_Array";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

#endif