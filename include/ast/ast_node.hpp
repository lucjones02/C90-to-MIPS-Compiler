#ifndef ast_node_hpp
#define ast_node_hpp

// #include <memory>
#include <string>
#include <unordered_map>

class Node;

typedef const Node *NodePtr;

// typedef for our lookup table
typedef std::unordered_map<std::string, u_int64_t> lt;

// typedef for information being passed down
typedef std::unordered_map<std::string, std::string> info;

class Node{
protected:
    int stack_size;
    int unique;
public:
    virtual ~Node() {};

    virtual std::string get_class() const{
        throw std::runtime_error("get_type not implemented");
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const{
        throw std::runtime_error("Compiler not implemented");
    }

    virtual int get_stack_size() const{
        return stack_size;
    }

    virtual void change_size(int new_size){}
    virtual void dim1(){}

    virtual NodePtr get_second_succession() const{
        throw std::runtime_error("Node isn't a succession");
    }
    virtual NodePtr get_first_succession() const{
        throw std::runtime_error("Node isn't a succession");
    }

    virtual std::string getId() const{
        throw std::runtime_error("no id");
    }

    virtual int get_unique() const{
        return unique;
    }
    virtual bool get_function_call() const{
        return false;
    }
};

class Succession: public Node{
private:
    NodePtr first;
    NodePtr second;
public:
    Succession(NodePtr _first, NodePtr _second): first(_first), second(_second) {
        stack_size = first->get_stack_size() + second->get_stack_size();
        unique = first->get_unique() + second->get_unique();
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

    virtual std::string get_class() const override{
        return "Succession";
    }

    virtual NodePtr get_second_succession() const override{
        return second;
    }
    virtual NodePtr get_first_succession() const override{
        return first;
    }

    virtual bool get_function_call() const override{
        return (first->get_function_call() || second->get_function_call());
    }
};

class Nop: public Node{
public:
    Nop(){
        stack_size=0;
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Nop";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override{}
    
    virtual bool get_function_call() const override{
        return false;
    }

};

#endif