Initial Milestones:

-   Create basic compiler, only implementing certain relatively simple
    features

    -   Create basic lexer (Luc)

    -   Create basic parser (Nick)

    -   Create ast.hpp files (class definitions) (Nick)

    -   Create ast.cpp files (codegen) (Luc)

    -   Create makefile (Luc)

-   Improve compiler to have more features by updating above files

Actual timeline, effort and description of tasks

| Week | Actual Tasks Completed  | Description  | Planned Effort  | Actual Effort  | Reason for effort difference/comments | Who worked on it |
| :----: | :----: | :----: | :----: | :----: | :----: | :----: |
| Week 1  | Create basic lexer | Lexer for only basic operators and tokens required for basic compiler  | 2h | 1h | Not much code, and only few features to implement | Luc – 90% Nick – 10%   |
| Week 1 | Create basic parser | Get the parse tree structure for only a few compiler features | 5h | 10h | Getting priority of operations right, took some thinking and planning | Luc – 20% Nick – 80% |
| Week 2 | Create ast.hpp files for basic compiler | Create the classes for each node in the parser tree | 10h | 10h | Straightforward but quite tedious | Luc – 60% Nick – 40% | 
| Week 2 | Updated missed features and code in lexer and parser | Furthered the functionality of the parser and lexer | N/A | 8h | Constantly found functionalities or features that we forgot to implement or didn’t know about | Luc – 40% Nick – 60% |
| Week 3 | Create ast.cpp file structure with some of the ast.cpp files for basic compiler | Created some of the code necessary for MIPS code generation | 10h | 30h | Ast.cpp files took longer than expected, due to many small necessary lines of code that weren’t in lectures, but were important for MIPS assembler (e.g .global f) | Luc – 80% Nick – 20% |
| Week 3 | Create Makefile | Combines all the files to allow compilation | 1h | 30mins | Was pretty similar to lab2 and lecture outlining makefiles | Luc |
| Week 3 | Create Shellscript for running tests | Had to create a shellscript to run provided tests on our compiler | N/A | 30mins | Was very useful for testing compiler and different features | Luc |
| Week 3 | Add personal tests | This was done to test operators/functionality not in the given tests | N/A | 1h | N/A | Luc – 50% Nick – 50% |
| Week 4 | Finish ast.cpp files for basic compiler | Added more MIPS code generation code | 10h | 20h | Overall it took us almost twice amount of time expected to finish basic compiler | Nick |
| Week 4 | Add more features to compiler across all files | Built on our basic compiler, by adding more features to hpp, cpp files and parser and lexer | 60h | 100h | Already had a lot of the basic infrastructure, but was harder to implement these features, therefore still took longer | Luc – 80% Nick – 20% |