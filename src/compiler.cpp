#include "ast.hpp"
#include <fstream>
#include <iostream>

int main(int argc, char *argv[]){

    std::ofstream outfile(argv[4]);

    yyset_in(fopen(argv[2], "r"));
    
    const Node *ast = parseAST();

    lt lookup;
    info i;

    ast->compile(outfile, lookup, 2, 0, i);

}