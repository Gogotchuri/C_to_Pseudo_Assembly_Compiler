# C_to_Pseudo_Assembly_Compiler
Compiler is in constructing space. Final program will be able to compile C code to pseudo asembly.

Currently, Tokenizer is finished (some tweaking left for unclear syntax);
Syntax (grammar) is almost finished, will add detailed description and flow chart soon;
Parser is in progress, basically, only syntax building part has been taken care of;

Detailed description of project and class documantations will be added soon;

Program should work following way: 
1)Tokenizer Slices given code file into tokens and assigns each token a type;
2)Parser uses tokenizer and Grammar rules to check code and build AST(abstract syntax tee)
3)Generator takes AST and generates Pseudo Assemly code from it
