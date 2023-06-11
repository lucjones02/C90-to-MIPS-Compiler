#ifndef ast_operators_hpp
#define ast_operators_hpp

#include <string>
#include "ast_node.hpp"

class Operator : public Node{
protected:
    NodePtr left;
    NodePtr right;
public:
    Operator(NodePtr _left, NodePtr _right) : left(_left) , right(_right) {
        stack_size=left->get_stack_size()+right->get_stack_size();
        unique = left->get_unique() + right->get_unique();
    }
    virtual ~Operator(){
        delete left;
        delete right;
    };

    virtual bool get_function_call() const override{
        return right->get_function_call();
    }

};

class A_Operator : public Node{
protected:
    NodePtr id;
    NodePtr code;

    A_Operator(NodePtr _id, NodePtr _code): 
        id(_id),
        code(_code) 
        {
            stack_size=8; 
            unique=code->get_unique();
        }

public:

    virtual ~A_Operator(){
        delete code;
    };

    virtual std::string getId() const override{
        if(id->get_class()!="Nop"){
            return id->getId();
        }
        return "none";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override{}

    virtual bool get_function_call() const override{
        return code->get_function_call();
    }

};

class AddOperator : public Operator{
public:
    AddOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}

    virtual std::string get_class() const override{
        return "AddOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class SubOperator : public Operator{
public:
    SubOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}

    virtual std::string get_class() const override{
        return "SubOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class MulOperator : public Operator{
public:
    MulOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "MulOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class DivOperator : public Operator{
public:
    DivOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "DivOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class ModOperator : public Operator{
public:
    ModOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "ModOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class AndOperator : public Operator{
public:
    AndOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "AndOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class OrOperator : public Operator{
public:
    OrOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "OrOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class EqualCompOperator : public Operator{
public:
    EqualCompOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}

    virtual std::string get_class() const override{
        return "EqualCompOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class GreaterCompOperator : public Operator{
public:
    GreaterCompOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "GreaterCompOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class LesserCompOperator : public Operator{
public:
    LesserCompOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "LesserCompOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};
class NotEqualCompOperator : public Operator{
public:

    NotEqualCompOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "NotEqualOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class GreaterEqualCompOperator : public Operator{
public:
    GreaterEqualCompOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "GreaterEqualCompOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class LesserEqualCompOperator : public Operator{
public:
    LesserEqualCompOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "LesserEqualCompOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class BitwiseAndOperator : public Operator{
public:
    BitwiseAndOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "BAndOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class BitwiseOrOperator : public Operator{
public:
    BitwiseOrOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "BOrOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class BitwiseXorOperator : public Operator{
public:
    BitwiseXorOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "BXorOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class LeftShiftOperator : public Operator{
public:
    LeftShiftOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "LSOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class RightShiftOperator : public Operator{
public:
    RightShiftOperator(NodePtr _left, NodePtr _right) : Operator(_left, _right) {}
    virtual std::string get_class() const override{
        return "RSOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class EqualOperator : public A_Operator{
    public:
        EqualOperator(NodePtr _id, NodePtr _code) : A_Operator(_id, _code) {}

        virtual std::string get_class() const override{
            return "EqOperator";
        }

        virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class PlusEqualOperator : public A_Operator{
public:
    PlusEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "PlEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class MinusEqualOperator : public A_Operator{
public:
    MinusEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "MiEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class MulEqualOperator : public A_Operator{
public:
    MulEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "MulEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class DivideEqualOperator : public A_Operator{
public:
    DivideEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "DivEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class ModEqualOperator : public A_Operator{
public:
    ModEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "ModEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class LeftShiftEqualOperator : public A_Operator{
public:
    LeftShiftEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "LSEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class RightShiftEqualOperator : public A_Operator{
public:
    RightShiftEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "RSEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class AndEqualOperator : public A_Operator{
public:
    AndEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "AndEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class OrEqualOperator : public A_Operator{
public:
    OrEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "OrEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

class XorEqualOperator : public A_Operator{
public:
    XorEqualOperator(NodePtr _id, NodePtr _right) : A_Operator(_id, _right) {}
    virtual std::string get_class() const override{
        return "XorEqOperator";
    }

    virtual void compile(std::ostream &dst, lt& lookup, int res_register, int total_block_size, info& i) const override;
};

// maybe this will work

#endif