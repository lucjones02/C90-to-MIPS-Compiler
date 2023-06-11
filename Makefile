CPPFLAGS += -std=c++11 -W -Wall -g -Wno-unused-parameter
CPPFLAGS += -I include

AST = src/ast_compile.cpp src/ast/ast_function.cpp src/ast/ast_operators.cpp src/ast/ast_primitives.cpp
AST += src/ast/ast_statements.cpp src/ast/ast_array.cpp src/ast/ast_enum.cpp src/ast/ast_struct.cpp
AST += src/ast/ast_pointer.cpp


all: bin/c_compiler

src/parser.tab.cpp src/parser.tab.hpp: src/parser.y
	bison -v -d src/parser.y -o src/parser.tab.cpp

src/lexer.yy.cpp : src/lexer.flex src/parser.tab.cpp
	flex -o src/lexer.yy.cpp src/lexer.flex


bin/c_compiler: src/compiler.cpp src/parser.tab.o src/lexer.yy.o
	mkdir -p bin
	g++ $(CPPFLAGS) -o bin/c_compiler $^ $(AST)


clean :
	rm -f src/*.o
	rm -f src/*.tab.cpp
	rm -f src/*.yy.cpp
	rm -f src/parser.output
	rm -f src/parser.tab.hpp
	rm -rf test_outputs/*
	rm -rf bin/*