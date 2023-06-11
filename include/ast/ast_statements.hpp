#ifndef ast_statements_hpp
#define ast_statements_hpp

#include "ast_node.hpp"
#include <string>

class If : public Node {
private:
    NodePtr condition;
    NodePtr ifcode;
    NodePtr elsecode;
public:
    If(NodePtr _condition, NodePtr _ifcode, NodePtr _elsecode):
        condition(_condition), 
        ifcode(_ifcode), 
        elsecode(_elsecode) 
    {
        stack_size = condition->get_stack_size() + ifcode->get_stack_size() + elsecode->get_stack_size();
        unique = 2 +ifcode->get_unique() + elsecode->get_unique();
    }
    virtual std::string get_class() const override{
        return "If";
    }
    virtual bool get_function_call() const override{
        return (ifcode->get_function_call() || elsecode->get_function_call());
    }
    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class While : public Node {
private:
    NodePtr condition;
    NodePtr code;
public:
    While(NodePtr _condition, NodePtr _code):
        condition(_condition), 
        code(_code) 
    {
        stack_size = condition->get_stack_size() + code->get_stack_size();
        unique = 2 + code->get_unique();
    }

    virtual bool get_function_call() const override{
        return (code->get_function_call() || condition->get_function_call());
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class For : public Node{
private: 
    NodePtr declare;
    NodePtr condition;
    NodePtr iterator;
    NodePtr code;
public:
    For(NodePtr _declare, NodePtr _condition, NodePtr _iterator, NodePtr _code):
        declare(_declare),
        condition(_condition),
        iterator(_iterator),
        code(_code)
    {
        stack_size = 8 + code->get_stack_size();
        unique = 2 + code->get_unique();
    }

    virtual std::string get_class() const override{
        return "For";
    }

    virtual bool get_function_call() const override{
        return (declare->get_function_call() || condition->get_function_call() || iterator->get_function_call() || code->get_function_call());
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class Return : public Node {
private:
    NodePtr value;
public:
    Return(NodePtr _value):
        value(_value) 
    {
        stack_size = value->get_stack_size();
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Return";
    }

    virtual bool get_function_call() const override{
        return value->get_function_call();
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class Switch : public Node {
private:
    NodePtr expression;
    NodePtr cases;
public:
    Switch(NodePtr _expression, NodePtr _cases): 
        expression(_expression), 
        cases(_cases)
    {
        // no idea what I did with these
        stack_size = expression->get_stack_size() + cases->get_stack_size();
        unique = 2 + cases->get_unique();
    }

    virtual std::string get_class() const override{
        return "Switch";
    }

    virtual bool get_function_call() const override{
        return (expression->get_function_call() || cases->get_function_call());
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class Case : public Node {
private:
    NodePtr compared;
    NodePtr code;
public:
    Case(NodePtr _compared, NodePtr _code):
        compared(_compared),
        code(_code)
    {
        // no idea what I did with these
        stack_size = compared->get_stack_size() + code->get_stack_size();
        unique = 2 + code->get_unique();
    }

    virtual std::string get_class() const override{
        return "Case";
    }

    virtual bool get_function_call() const override{
        return (compared->get_function_call() || code->get_function_call());
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class Break : public Node {
public:
    Break(){
        stack_size=0;
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Break";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

    virtual bool get_function_call() const override{
        return false;
    }
};

class Continue : public Node {
public:
    Continue(){
        stack_size=0;
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Continue";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

    virtual bool get_function_call() const override{
        return false;
    }
};

class Typedef: public Node{
private:
    std::string type;
    std::string new_type;

public:
    Typedef(std::string _type, std::string _new_type): type(_type), new_type(_new_type){
        stack_size = 0;
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Typedef";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

#endif