#ifndef ast_primitives_hpp
#define ast_primitives_hpp

#include "ast_node.hpp"
#include <string>

class Identifier : public Node{
private:
    std::string id;
    
public:
    Identifier(const std::string &_id) : id(_id) {stack_size=0;unique=0;}

    virtual std::string getId() const override{
        return id;
    }

    virtual std::string get_class() const override {
        return "Identifier";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

class Declare: public Node{
private:
    std::string type;
    std::string id;

public:
    Declare(const std::string &_type, const std::string &_id): type(_type), id(_id){ stack_size=8; unique=0;}

    virtual std::string getId() const override{
        return id;
    }

    virtual std::string get_class() const override{
        return "Declare";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};



class Number : public Node{
private:
    double value;
public:
    Number(double _value) : value(_value) {stack_size=0;unique=0;};

    virtual ~Number(){};

    double getValue() const{
        return value;
    };

    virtual std::string get_class() const override{
        return "Number";
    };

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class Float : public Node{
private:
    double value;
public:
    Float(double _value): value(_value){stack_size=0;unique=0;}

    virtual ~Float(){}

    double getValue() const{
        return value;
    }

    virtual std::string get_class() const override{
        return "Float";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

// maybe this works

#endif