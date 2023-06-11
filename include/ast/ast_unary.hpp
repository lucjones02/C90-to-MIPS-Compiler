#ifndef ast_unary_hpp
#define ast_unary_hpp

#include "ast_node.hpp"
#include <string>

class Unary : public Node{
protected:
    NodePtr node;
public:
    Unary(NodePtr _node) : node(_node) {
        stack_size = 0;
        unique = node->get_unique();
    }

    virtual ~Unary() {delete node;}
    virtual bool get_function_call() const override{
        return node->get_function_call();
    }
};

class A_Operator2 : public Node{
protected:
    std::string id;

    A_Operator2(std::string _id): id(_id) {stack_size=8; unique=0;};

public:

    virtual ~A_Operator2(){
    };

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override{}

};

class NegOperator : public Unary{
public:
    NegOperator(const NodePtr _node) : Unary(_node) {}

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class PosOperator : public Unary{
public:
    PosOperator(const NodePtr _node) : Unary(_node) {}

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class ComplOperator : public Unary{
public:
    ComplOperator(const NodePtr _node) : Unary(_node) {}

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class PosIterateOperator : public A_Operator2{
public:
    PosIterateOperator(std::string _id) : A_Operator2(_id) {}
    
    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class NegIterateOperator : public A_Operator2{
public:
    NegIterateOperator(std::string _id) : A_Operator2(_id) {}
    
    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class PostPosIterateOperator : public A_Operator2{
public:
    PostPosIterateOperator(std::string _id) : A_Operator2(_id) {}
    
    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class PostNegIterateOperator : public A_Operator2{
public:
    PostNegIterateOperator(std::string _id) : A_Operator2(_id) {}
    
    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class NotOperator : public Unary{
public:
    NotOperator(const NodePtr _node) : Unary(_node) {}

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

#endif
