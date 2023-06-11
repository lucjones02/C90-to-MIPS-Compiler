#ifndef ast_pointer_hpp 
#define ast_pointer_hpp

#include "ast_node.hpp"
#include <string>

class Address_of : public Node{
private:
    std::string id;

public:
    Address_of(std::string _id): id(_id){
        stack_size = 8;
        unique = 0;
    }

    virtual std::string get_class() const override{
        return "Address_of";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;

};

#endif